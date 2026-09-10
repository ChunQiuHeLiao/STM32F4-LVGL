#include"ota_ab.h"
#include"fs_api.h"
#include"debug.h"

/*!
 * OTA A/B 区升级实现
 *   - SD 卡双固件: packet_1.bin(A区, 稳定版/回滚用) / packet_2.bin(B区, 新固件)
 *   - 分块接收     : OTA_Start / OTA_WritePacket / OTA_Finish
 *   - CRC32 校验   : 收包时累加 + 收完回读文件校验 + 烧写后回读 Flash 校验
 *   - Flash 擦写   : OTA_Flash_Erase / OTA_Flash_Write (编程时序在 RAM 中执行)
 *   - 升级确认/回滚: OTA_Confirm / OTA_Rollback (状态机存在 S:/ota/ota.cfg)
 */

/* SD 卡互斥锁, 由 App_Task_Init() 创建 */
extern SemaphoreHandle_t g_sdMutex;

#define OTA_FILE_CFG  "S:/ota/ota.cfg"

/* 盘符(与 main_lib.h 里的 DRIVER_DIR 保持一致, 这里做兜底避免耦合 LVGL 头文件) */
#ifndef OTA_DRIVER_DIR
#define OTA_DRIVER_DIR  "S:"
#endif
#ifndef DRIVER_DIR
#define DRIVER_DIR  OTA_DRIVER_DIR
#endif


/*------------------- 内部变量 -------------------*/
static uint8_t  otaBuf[OTA_BLOCK_SIZE];  /* 擦写/校验用的分块缓冲 */
static FIL      otaFp;
static uint8_t  otaFpOpened = 0;
static uint32_t otaTotalSize = 0;
static uint32_t otaRxSize = 0;
static uint8_t  otaUpdating = 0;
static CRC32_CTX otaCrcCtx;
static OtaCfg_t  gOtaCfg;


/* 当前运行固件在 Flash 中的结束地址(链接脚本符号) */
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;


/*=================================================================
 *  1. 内部 Flash 底层擦写(编程时序运行在 RAM 中)
 *=================================================================*/

/* FLASH 寄存器位, 直接用寄存器操作, 保证 RAM 函数中不调用任何 Flash 里的代码 */
#define OTA_FCR_PG      (0x00000001UL)
#define OTA_FCR_SER     (0x00000002UL)
#define OTA_FCR_SNB_MSK (0x00000078UL)
#define OTA_FCR_SNB_POS (3UL)
#define OTA_FCR_STRT    (0x00010000UL)
#define OTA_FCR_PSIZE_MSK (0x00000300UL)
#define OTA_FCR_LOCK    (0x80000000UL)

#define OTA_FSR_EOP     (0x00000001UL)
#define OTA_FSR_OPERR   (0x00000002UL)
#define OTA_FSR_WRPERR  (0x00000010UL)
#define OTA_FSR_PGAERR  (0x00000020UL)
#define OTA_FSR_PGPERR  (0x00000040UL)
#define OTA_FSR_PGSERR  (0x00000080UL)
#define OTA_FSR_ERR_MSK (OTA_FSR_OPERR|OTA_FSR_WRPERR|OTA_FSR_PGAERR|OTA_FSR_PGPERR|OTA_FSR_PGSERR)
#define OTA_FSR_BSY     (0x00010000UL)

/* STM32F411xE 扇区表: 16K*4 + 64K + 128K*3 */
static const uint32_t otaSectorAddr[8] =
{
    0x08000000UL,0x08004000UL,0x08008000UL,0x0800C000UL,
    0x08010000UL,0x08020000UL,0x08040000UL,0x08060000UL
};
static const uint32_t otaSectorSize[8] =
{
    16*1024,16*1024,16*1024,16*1024,64*1024,128*1024,128*1024,128*1024
};


/// @brief 获取地址所在的扇区号, 0xFF 表示越界
static uint32_t OTA_GetSector(uint32_t addr)
{
    if(addr < OTA_FLASH_BASE) return 0xFF;
    for(uint32_t i=0;i<8;i++)
    {
        if(addr < otaSectorAddr[i]+otaSectorSize[i]) return i;
    }
    return 0xFF;
}

static uint32_t OTA_SectorAddr(uint32_t sector)
{
    if(sector>=8) return 0xFFFFFFFFUL;
    return otaSectorAddr[sector];
}


/*!
 * 扇区擦除 —— 必须在 RAM 中执行(擦除期间不能从 Flash 取指)。
 * long_call 是必须的: Flash(0x08000000) 到 RAM(0x20000000) 的距离超过 BL 的
 * ±16MB 范围, 普通 bl 指令跳不过去。
 */
__attribute__((section(".RamFunc"), noinline, long_call, used))
static int RamFlash_EraseSector(uint32_t sector)
{
    volatile uint32_t timeout = 0x1000000;

    FLASH->SR = (OTA_FSR_EOP|OTA_FSR_ERR_MSK);      /* 清掉遗留错误标志 */

    while((FLASH->SR & OTA_FSR_BSY) && --timeout);
    if(timeout==0) return RET_TIMEOUT;

    FLASH->CR &= ~(OTA_FCR_PSIZE_MSK|OTA_FCR_SNB_MSK);
    FLASH->CR |= OTA_FCR_SER | ((sector & 0x0FUL) << OTA_FCR_SNB_POS);
    FLASH->CR |= OTA_FCR_STRT;

    timeout = 0x1000000;
    while((FLASH->SR & OTA_FSR_BSY) && --timeout);

    FLASH->CR &= ~OTA_FCR_SER;

    if(timeout==0) return RET_TIMEOUT;
    if(FLASH->SR & OTA_FSR_ERR_MSK) return RET_ERR;

    return RET_OK;
}


/*!
 * 字节编程 —— 必须在 RAM 中执行。src 必须指向 RAM。
 */
__attribute__((section(".RamFunc"), noinline, long_call, used))
static int RamFlash_Program(uint32_t dst,const uint8_t* src,uint32_t len)
{
    volatile uint32_t timeout;

    FLASH->SR = (OTA_FSR_EOP|OTA_FSR_ERR_MSK);

    /* PSIZE = 00 --> 按字节编程 */
    FLASH->CR &= ~(OTA_FCR_PSIZE_MSK|OTA_FCR_SNB_MSK);
    FLASH->CR |= OTA_FCR_PG;

    for(uint32_t i=0;i<len;i++)
    {
        *(volatile uint8_t*)(dst+i) = src[i];

        timeout = 0x100000;
        while((FLASH->SR & OTA_FSR_BSY) && --timeout);
        if(timeout==0) { FLASH->CR &= ~OTA_FCR_PG; return RET_TIMEOUT; }
        if(FLASH->SR & OTA_FSR_ERR_MSK) { FLASH->CR &= ~OTA_FCR_PG; return RET_ERR; }
    }

    FLASH->CR &= ~OTA_FCR_PG;
    return RET_OK;
}


/// @brief 擦除 [addr,addr+size) 覆盖到的所有扇区
int OTA_Flash_Erase(uint32_t addr,uint32_t size)
{
    uint32_t sector = OTA_GetSector(addr);
    if(sector==0xFF) return RET_ERR;

    if(HAL_FLASH_Unlock()!=HAL_OK) return RET_ERR;

    int ret=RET_OK;
    uint32_t end = addr+size;
    while(1)
    {
        __disable_irq();
        ret = RamFlash_EraseSector(sector);
        __enable_irq();
        if(ret!=RET_OK) break;

        sector++;
        if(sector>=8) break;
        if(OTA_SectorAddr(sector) >= end) break;
    }

    HAL_FLASH_Lock();
    return ret;
}


/// @brief 写数据到 Flash(不擦除)
int OTA_Flash_Write(uint32_t addr,uint8_t* data,uint32_t size)
{
    if(size==0) return RET_OK;
    if(OTA_GetSector(addr)==0xFF) return RET_ERR;
    if(OTA_GetSector(addr+size-1)==0xFF) return RET_ERR;

    if(HAL_FLASH_Unlock()!=HAL_OK) return RET_ERR;

    int ret=RET_OK;
    uint32_t offset=0;
    while(offset<size)
    {
        uint32_t chunk = size-offset;
        if(chunk>OTA_BLOCK_SIZE) chunk=OTA_BLOCK_SIZE;

        /* 源数据必须在 RAM 中 */
        memcpy(otaBuf,data+offset,chunk);

        __disable_irq();
        ret = RamFlash_Program(addr+offset,otaBuf,chunk);
        __enable_irq();
        if(ret!=RET_OK) break;

        offset += chunk;
    }

    HAL_FLASH_Lock();
    return ret;
}


/// @brief 计算 Flash 内容的 CRC32
int OTA_Flash_Crc32(uint32_t addr,uint32_t size,uint32_t* crc32)
{
    if(crc32==NULL) return RET_ERR;
    if(OTA_GetSector(addr)==0xFF) return RET_ERR;

    CRC32_CTX ctx;
    CRC32_Init(&ctx);

    uint32_t offset=0;
    while(offset<size)
    {
        uint32_t chunk = size-offset;
        if(chunk>OTA_BLOCK_SIZE) chunk=OTA_BLOCK_SIZE;

        memcpy(otaBuf,(const void*)(addr+offset),chunk);
        CRC32_Update(&ctx,otaBuf,chunk);

        offset += chunk;
    }

    CRC32_Final(&ctx,crc32);
    return RET_OK;
}


/// @brief 把 SD 卡镜像烧写到指定 Flash 地址(擦除+写入+回读CRC)
int OTA_ProgramImageFromSdEx(const char* path,uint32_t size,uint32_t crc32,uint32_t baseAddr)
{
    FIL fp;
    UINT br=0;

    if(size==0)
    {
        LOG_E(-1,"image size invalid:%lu\r\n",size);
        return RET_ERR;
    }
    if(OTA_GetSector(baseAddr)==0xFF || OTA_GetSector(baseAddr+size-1)==0xFF)
    {
        LOG_E(-1,"target addr out of flash:0x%08x\r\n",(unsigned)baseAddr);
        return RET_ERR;
    }
    if(f_open(&fp,path,FA_READ)!=FR_OK)
    {
        LOG_E(-1,"open %s fail\r\n",path);
        return RET_ERR;
    }

    LOG_I("program %s,%luB -> 0x%08x\r\n",path,size,(unsigned)baseAddr);

    if(HAL_FLASH_Unlock()!=HAL_OK)
    {
        f_close(&fp);
        return RET_ERR;
    }

    int ret=RET_OK;
    uint32_t offset=0;
    while(offset<size)
    {
        uint32_t chunk = size-offset;
        if(chunk>OTA_BLOCK_SIZE) chunk=OTA_BLOCK_SIZE;

        if(f_read(&fp,otaBuf,chunk,&br)!=FR_OK || br!=chunk)
        {
            LOG_E(-1,"read sd fail,offset:%lu\r\n",offset);
            ret=RET_ERR;
            break;
        }

        uint32_t dst = baseAddr+offset;
        uint32_t sector = OTA_GetSector(dst);
        if(sector==0xFF){ ret=RET_ERR; break; }

        /* 每个扇区的第一块才擦除, 之后只编程, 避免重复擦除丢数据 */
        uint8_t erase = (dst==OTA_SectorAddr(sector))?1:0;

        __disable_irq();
        if(erase) ret = RamFlash_EraseSector(sector);
        if(ret==RET_OK) ret = RamFlash_Program(dst,otaBuf,chunk);
        __enable_irq();

        if(ret!=RET_OK)
        {
            LOG_E(ret,"flash program fail,offset:0x%08x\r\n",(unsigned)dst);
            break;
        }

        offset += chunk;
    }

    HAL_FLASH_Lock();
    f_close(&fp);

    if(ret==RET_OK)
    {
        uint32_t crc=0;
        if(OTA_Flash_Crc32(baseAddr,size,&crc)!=0 || crc!=crc32)
        {
            LOG_E2(-1,"flash verify fail,rx:%08lx,cal:%08lx\r\n",crc32,crc);
            ret = RET_ERR;
        }
        else
        {
            LOG_I("program ok,crc32:%08lx\r\n",crc);
        }
    }

    return ret;
}


/// @brief 把 SD 卡镜像烧写到 APP 区(擦除+写入+回读CRC)
int OTA_ProgramImageFromSd(const char* path,uint32_t size,uint32_t crc32)
{
    return OTA_ProgramImageFromSdEx(path,size,crc32,OTA_APP_ADDR);
}


/*=================================================================
 *  2. SD 卡 双固件(A/B区) 读写与校验
 *=================================================================*/

/// @brief 计算 SD 卡文件的 CRC32
int OTA_File_Crc32(const char* path,uint32_t size,uint32_t* crc32)
{
    FIL fp;
    UINT br=0;

    if(crc32==NULL) return RET_ERR;
    if(f_open(&fp,path,FA_READ)!=FR_OK) return RET_ERR;

    CRC32_CTX ctx;
    CRC32_Init(&ctx);

    int ret=RET_OK;
    uint32_t offset=0;
    while(offset<size)
    {
        uint32_t chunk = size-offset;
        if(chunk>OTA_BLOCK_SIZE) chunk=OTA_BLOCK_SIZE;

        if(f_read(&fp,otaBuf,chunk,&br)!=FR_OK || br!=chunk)
        {
            ret=RET_ERR;
            break;
        }
        CRC32_Update(&ctx,otaBuf,chunk);
        offset += chunk;
    }

    f_close(&fp);

    if(ret==RET_OK) CRC32_Final(&ctx,crc32);
    return ret;
}


/// @brief 把 Flash 中的固件备份到 SD 卡(A 区), 同时算出 CRC32
static int OTA_BackupFlashToSd(uint32_t addr,uint32_t size,const char* path,uint32_t* crc32)
{
    FIL fp;
    UINT bw=0;

    if(f_open(&fp,path,FA_WRITE|FA_CREATE_ALWAYS)!=FR_OK) return RET_ERR;

    CRC32_CTX ctx;
    CRC32_Init(&ctx);

    int ret=RET_OK;
    uint32_t offset=0;
    while(offset<size)
    {
        uint32_t chunk = size-offset;
        if(chunk>OTA_BLOCK_SIZE) chunk=OTA_BLOCK_SIZE;

        memcpy(otaBuf,(const void*)(addr+offset),chunk);
        CRC32_Update(&ctx,otaBuf,chunk);

        if(f_write(&fp,otaBuf,chunk,&bw)!=FR_OK || bw!=chunk)
        {
            ret=RET_ERR;
            break;
        }
        offset += chunk;
    }

    f_sync(&fp);
    f_close(&fp);

    if(ret==RET_OK && crc32!=NULL) CRC32_Final(&ctx,crc32);
    return ret;
}


/// @brief 文件拷贝(用于把 B 区提升为新的 A 区)
static int OTA_CopyFile(const char* src,const char* dst)
{
    FIL fsrc,fdst;
    UINT br=0,bw=0;

    if(f_open(&fsrc,src,FA_READ)!=FR_OK) return RET_ERR;
    if(f_open(&fdst,dst,FA_WRITE|FA_CREATE_ALWAYS)!=FR_OK)
    {
        f_close(&fsrc);
        return RET_ERR;
    }

    int ret=RET_OK;
    while(1)
    {
        if(f_read(&fsrc,otaBuf,OTA_BLOCK_SIZE,&br)!=FR_OK) { ret=RET_ERR; break; }
        if(br==0) break;
        if(f_write(&fdst,otaBuf,br,&bw)!=FR_OK || bw!=br) { ret=RET_ERR; break; }
    }

    f_sync(&fdst);
    f_close(&fdst);
    f_close(&fsrc);

    return ret;
}


/// @brief 当前正在运行的固件在 Flash 中占用的大小
uint32_t OTA_GetRunningImageSize(void)
{
    uint32_t end = (uint32_t)(&_sidata) + ((uint32_t)(&_edata) - (uint32_t)(&_sdata));
    uint32_t size = end - OTA_APP_ADDR;

    if(size==0 || size>OTA_IMAGE_MAX_SIZE) size = OTA_IMAGE_MAX_SIZE;
    return size;
}


/*=================================================================
 *  3. 升级状态机(存在 SD 卡, 掉电不丢)
 *=================================================================*/

static void OTA_CfgCrcFill(OtaCfg_t* cfg)
{
    cfg->crc = CRC32_Calculate((uint8_t*)cfg,(uint32_t)sizeof(OtaCfg_t)-(uint32_t)sizeof(uint32_t));
}

static int OTA_CfgCheck(OtaCfg_t* cfg)
{
    if(cfg->magic != OTA_CFG_MAGIC) return RET_ERR;
    uint32_t crc = CRC32_Calculate((uint8_t*)cfg,(uint32_t)sizeof(OtaCfg_t)-(uint32_t)sizeof(uint32_t));
    if(crc != cfg->crc) return RET_ERR;
    return RET_OK;
}

static int OTA_CfgRead(OtaCfg_t* cfg)
{
    FIL fp;
    UINT br=0;

    if(f_open(&fp,OTA_FILE_CFG,FA_READ)!=FR_OK) return RET_ERR;
    FRESULT res = f_read(&fp,cfg,sizeof(OtaCfg_t),&br);
    f_close(&fp);

    if(res!=FR_OK || br!=sizeof(OtaCfg_t)) return RET_ERR;
    if(OTA_CfgCheck(cfg)!=RET_OK) return RET_ERR;

    return RET_OK;
}

static int OTA_CfgWrite(OtaCfg_t* cfg)
{
    FIL fp;
    UINT bw=0;

    cfg->magic = OTA_CFG_MAGIC;
    OTA_CfgCrcFill(cfg);

    if(f_open(&fp,OTA_FILE_CFG,FA_WRITE|FA_CREATE_ALWAYS)!=FR_OK) return RET_ERR;
    FRESULT res = f_write(&fp,cfg,sizeof(OtaCfg_t),&bw);
    f_sync(&fp);
    f_close(&fp);

    if(res!=FR_OK || bw!=sizeof(OtaCfg_t)) return RET_ERR;
    return RET_OK;
}


/*=================================================================
 *  4. Boot 阶段: 与 BootLoader 握手 / 确认 / 回滚
 *=================================================================*/

#if OTA_BOOT_HANDSHAKE
/// @brief 首次运行时补齐出厂的固件信息(与 Boot 的 SysConfInfo_Get 行为对齐)
static void OTA_SysConfInitIfEmpty(OtaSysConf_t* sc,uint32_t runSize,uint32_t runCrc)
{
    if(sc->factoryFlag==0xff)
    {
        sc->factoryFirmware.size  = runSize;
        sc->factoryFirmware.crc32 = runCrc;
        sc->updateFireware.size   = 0;
        sc->updateFireware.crc32  = 0;
        sc->isUpdate              = 0;
        sc->factoryFlag           = 0;
    }
}
#endif


static int OTA_DoRollback(OtaCfg_t* cfg)
{
    LOG_I("---- OTA rollback ----\r\n");

    int ret = RET_ERR;
    uint8_t needReset = 0;

    if(cfg->aSize>0 && cfg->aSize<=OTA_IMAGE_MAX_SIZE)
    {
        uint32_t crc=0;
        if(OTA_File_Crc32(OTA_FILE_A,cfg->aSize,&crc)==RET_OK && crc==cfg->aCrc32)
        {
            LOG_I("A区固件有效(%luB,crc:%08lx)\r\n",cfg->aSize,crc);

#if OTA_BOOT_HANDSHAKE
            /* 1. 把 A 区(稳定版)固件写进片上备份区 */
            if(OTA_SaveSlot_Write(OTA_FILE_A,cfg->aSize,cfg->aCrc32)==RET_OK)
            {
                /* 2. 置更新标志, 复位后由 Boot 用备份区覆盖 APP 区 */
                OtaSysConf_t sc;
                memset(&sc,0,sizeof(sc));
                OTA_SysConf_Get(&sc);
                sc.updateFireware.size  = cfg->aSize;
                sc.updateFireware.crc32 = cfg->aCrc32;
                sc.isUpdate             = 1;
                if(OTA_SysConf_Set(&sc)==RET_OK)
                {
                    needReset = 1;
                    ret = RET_OK;
                }
            }
#else
            LOG_I("未启用 Boot 握手, 回滚镜像保留在 %s\r\n",OTA_FILE_A);
            ret = RET_OK;
#endif
        }
        else
        {
            LOG_E(-1,"A区固件无效, 无法回滚\r\n");
        }
    }
    else
    {
        LOG_E(-1,"A区固件不存在, 无法回滚\r\n");
    }

    cfg->state = OTA_STATE_IDLE;
    cfg->retry = 0;
    OTA_CfgWrite(cfg);

    if(needReset)
    {
        LOG_I("回滚已交给 BootLoader, 复位\r\n");
        delay_ms(100);
        Sys_SoftReset();
    }

    return ret;
}


/// @brief 上电 Boot 阶段调用(FreeRTOS 启动之前)
int OTA_Boot_Run(void)
{
    OtaCfg_t cfg;
    OtaSysConf_t sc;

    /* SD 卡初始化(f_mount 内部会初始化 SDIO) */
    if(FS_API_Init((const uint8_t*)DRIVER_DIR)!=0)
    {
        LOG_E(-1,"sd mount fail\r\n");
        return RET_ERR;
    }
    f_mkdir(OTA_DIR);   /* FF_FS_MINIMIZE==0 时可用 */

    /* 读 Boot 侧共享配置(Boot 搬运成功后会把 isUpdate 清 0) */
    memset(&sc,0,sizeof(sc));
    OTA_SysConf_Get(&sc);
    if(sc.factoryFlag!=0xff)
    {
        LOG_I("boot cfg: isUpdate:%d,factory:%lu/%08lx,update:%lu/%08lx\r\n",
            sc.isUpdate,sc.factoryFirmware.size,sc.factoryFirmware.crc32,
            sc.updateFireware.size,sc.updateFireware.crc32);
    }

    if(OTA_CfgRead(&cfg)!=RET_OK)
    {
        LOG_I("ota:no upgrade task\r\n");
        return RET_OK;
    }

    LOG_I("ota state:%lu,size:%lu,crc:%08lx,retry:%lu\r\n",
        cfg.state,cfg.size,cfg.crc32,cfg.retry);

    switch(cfg.state)
    {
        /* B 区固件已收齐, 等 BootLoader 搬运到 APP 区 */
        case OTA_STATE_READY:
        {
            uint32_t crc=0;

            if(cfg.size==0 || cfg.size>OTA_IMAGE_MAX_SIZE ||
               OTA_File_Crc32(OTA_FILE_B,cfg.size,&crc)!=RET_OK || crc!=cfg.crc32)
            {
                LOG_E(-1,"B区固件校验失败, 放弃本次升级\r\n");
                cfg.state = OTA_STATE_IDLE;
                break;
            }

            LOG_I("B区固件校验通过(crc:%08lx)\r\n",crc);

#if OTA_BOOT_HANDSHAKE
            OTA_SysConf_Get(&sc);
            if(sc.isUpdate==0)
            {
                /* Boot 已经搬运完成并跳转过来 -> 进入待确认 */
                LOG_I("Boot 已搬运新固件, 进入待确认\r\n");
                cfg.state = OTA_STATE_PENDING;
                cfg.retry = 0;
            }
            else
            {
                /* Boot 还没搬(可能上次复位太早), 复位重试几次, 避免死循环 */
                cfg.retry++;
                if(cfg.retry > OTA_MAX_RETRY)
                {
                    LOG_E(-1,"Boot 未搬运, 放弃等待\r\n");
                    cfg.state = OTA_STATE_PENDING;
                    cfg.retry = 0;
                    break;
                }
                LOG_E2(-1,"Boot 未搬运(isUpdate=1), 复位重试:%lu\r\n",cfg.retry);
                OTA_CfgWrite(&cfg);
                delay_ms(100);
                Sys_SoftReset();
            }
#else
            /* 没有烧 Boot / APP 仍在 0x08000000:
             * 片上搬运由 BootLoader 负责, 这里只推进状态机 */
            LOG_I("APP工程未启用Boot握手, 直接进入待确认\r\n");
            cfg.state = OTA_STATE_PENDING;
            cfg.retry = 0;
#endif
            break;
        }

        /* 新固件已生效, 等待运行确认 */
        case OTA_STATE_PENDING:
        {
            cfg.retry++;
            if(cfg.retry >= OTA_MAX_RETRY)
            {
                LOG_E(-1,"新固件连续 %lu 次未确认, 自动回滚\r\n",cfg.retry);
                cfg.state = OTA_STATE_ROLLBACK;
            }
            break;
        }

        case OTA_STATE_ROLLBACK:
        {
            OTA_DoRollback(&cfg);
            return RET_OK;
        }

        case OTA_STATE_CONFIRMED:
        default:
        {
            /* 升级已完成, 清掉状态 */
            if(cfg.state==OTA_STATE_CONFIRMED) LOG_I("ota:last upgrade confirmed\r\n");
            cfg.state = OTA_STATE_IDLE;
            break;
        }
    }

    OTA_CfgWrite(&cfg);

    if(cfg.state==OTA_STATE_ROLLBACK)
    {
        OTA_DoRollback(&cfg);
    }

    return RET_OK;
}


/*=================================================================
 *  5. 分块接收 + CRC + 确认接口(由 OTA 任务调用)
 *=================================================================*/

int OTA_Init(void)
{
    memset(&gOtaCfg,0,sizeof(OtaCfg_t));
    otaRxSize = 0;
    otaTotalSize = 0;
    otaUpdating = 0;
    otaFpOpened = 0;
    return RET_OK;
}


int OTA_Start(uint32_t fwSize)
{
    if(fwSize==0 || fwSize>OTA_IMAGE_MAX_SIZE)
    {
        LOG_E(-1,"fw size invalid:%lu\r\n",fwSize);
        return RET_ERR;
    }

    /* 关掉上一次可能残留的文件句柄 */
    if(otaFpOpened){ f_close(&otaFp); otaFpOpened=0; }

    if(f_mkdir(OTA_DIR)!=FR_OK && f_stat(OTA_DIR,NULL)!=FR_OK)
    {
        LOG_E(-1,"create %s fail\r\n",OTA_DIR);
        return RET_ERR;
    }

    /* 新建(清空) B 区文件 */
    if(f_open(&otaFp,OTA_FILE_B,FA_WRITE|FA_CREATE_ALWAYS)!=FR_OK)
    {
        LOG_E(-1,"create %s fail\r\n",OTA_FILE_B);
        return RET_ERR;
    }
    otaFpOpened = 1;

    otaTotalSize = fwSize;
    otaRxSize = 0;
    otaUpdating = 1;
    CRC32_Init(&otaCrcCtx);

    LOG_I("ota start,fw size:%lu,save to %s\r\n",fwSize,OTA_FILE_B);
    return RET_OK;
}


int OTA_WritePacket(uint8_t* data,uint16_t len)
{
    UINT bw=0;

    if(!otaUpdating || !otaFpOpened || data==NULL || len==0) return RET_ERR;
    if(otaRxSize+len > otaTotalSize)
    {
        LOG_E(-1,"rx overflow:%lu/%lu\r\n",otaRxSize+len,otaTotalSize);
        return RET_ERR;
    }

    if(g_sdMutex!=NULL) xSemaphoreTake(g_sdMutex,portMAX_DELAY);

    FRESULT res = f_write(&otaFp,data,len,&bw);

    if(g_sdMutex!=NULL) xSemaphoreGive(g_sdMutex);

    if(res!=FR_OK || bw!=len)
    {
        LOG_E(-1,"write sd fail\r\n");
        return RET_ERR;
    }

    /* 接收时同步累加 CRC, 收完后再回读文件校验一次 */
    CRC32_Update(&otaCrcCtx,data,len);
    otaRxSize += len;

    return RET_OK;
}


int OTA_Finish(uint32_t crc32)
{
    uint32_t crc=0;

    if(!otaUpdating) return RET_ERR;

    if(otaFpOpened)
    {
        if(g_sdMutex!=NULL) xSemaphoreTake(g_sdMutex,portMAX_DELAY);
        f_sync(&otaFp);
        f_close(&otaFp);
        if(g_sdMutex!=NULL) xSemaphoreGive(g_sdMutex);
        otaFpOpened = 0;
    }

    /* 1. 大小校验 */
    if(otaRxSize != otaTotalSize)
    {
        LOG_E2(-1,"size not match,rx:%lu,need:%lu\r\n",otaRxSize,otaTotalSize);
        OTA_Abort();
        return RET_ERR;
    }

    /* 2. 回读 SD 卡 B 区文件做 CRC32 校验 */
    if(OTA_File_Crc32(OTA_FILE_B,otaTotalSize,&crc)!=RET_OK || crc!=crc32)
    {
        LOG_E2(-1,"firmware crc fail,rx:%08lx,cal:%08lx\r\n",crc32,crc);
        OTA_Abort();
        return RET_ERR;
    }
    LOG_I("firmware crc ok:%08lx,%luB\r\n",crc,otaTotalSize);

    /* 3. 把当前正在运行的固件备份到 A 区(用于失败回滚) */
    uint32_t runSize = OTA_GetRunningImageSize();
    uint32_t aCrc = 0;
    if(OTA_BackupFlashToSd(OTA_APP_ADDR,runSize,OTA_FILE_A,&aCrc)!=RET_OK)
    {
        LOG_E(-1,"backup current firmware to A fail\r\n");
        aCrc = 0;
        runSize = 0;
    }
    else
    {
        LOG_I("backup current firmware to %s,%luB,crc:%08lx\r\n",OTA_FILE_A,runSize,aCrc);
    }

    /* 4. 与 BootLoader 握手: 写片上备份区(SAVE_APP) + 置更新标志
     *    Boot 会: 擦 APP 区 -> 从 SAVE_APP 搬过去 -> CRC 校验 -> isUpdate=0 -> 跳转
     *    只有备份区真的写成功才置 isUpdate, 否则 Boot 会一直搬运失败 */
#if OTA_BOOT_HANDSHAKE
    {
        OtaSysConf_t sc;
        memset(&sc,0,sizeof(sc));
        OTA_SysConf_Get(&sc);
        OTA_SysConfInitIfEmpty(&sc,runSize,aCrc);

        if(OTA_SaveSlot_Write(OTA_FILE_B,otaTotalSize,crc32)==RET_OK)
        {
            sc.updateFireware.size  = otaTotalSize;
            sc.updateFireware.crc32 = crc32;
            sc.isUpdate             = 1;
            if(OTA_SysConf_Set(&sc)!=RET_OK)
            {
                LOG_E(-1,"write boot sys conf fail\r\n");
                OTA_Abort();
                return RET_ERR;
            }
            LOG_I("boot handshake ok,reboot to take effect\r\n");
        }
        else
        {
            LOG_E(-1,"片上备份区不可用, 本次只完成 SD 卡 B 区(%s)存储\r\n",OTA_FILE_B);
        }
    }
#endif

    /* 5. 写入升级状态, 复位后由 Boot 阶段/状态机处理 */
    memset(&gOtaCfg,0,sizeof(OtaCfg_t));
    gOtaCfg.state  = OTA_STATE_READY;
    gOtaCfg.size   = otaTotalSize;
    gOtaCfg.crc32  = crc32;
    gOtaCfg.aSize  = runSize;
    gOtaCfg.aCrc32 = aCrc;
    gOtaCfg.retry  = 0;
    OTA_CfgWrite(&gOtaCfg);

    otaUpdating = 0;
    return RET_OK;
}


void OTA_Abort(void)
{
    LOG_I("ota abort,rollback to A\r\n");

    if(otaFpOpened)
    {
        f_close(&otaFp);
        otaFpOpened = 0;
    }

    otaUpdating = 0;
    otaRxSize = 0;

    /* 丢掉坏的 B 区镜像, A 区(稳定版)保持不变 */
    f_unlink(OTA_FILE_B);

    memset(&gOtaCfg,0,sizeof(OtaCfg_t));
    gOtaCfg.state = OTA_STATE_IDLE;
    OTA_CfgWrite(&gOtaCfg);
}


int OTA_Confirm(void)
{
    OtaCfg_t cfg;

    if(OTA_CfgRead(&cfg)!=RET_OK) return RET_OK;
    if(cfg.state != OTA_STATE_PENDING) return RET_OK;

    /* 新固件稳定运行 -> 把 B 区提升为新的 A 区(下一次的回滚基点) */
    if(OTA_CopyFile(OTA_FILE_B,OTA_FILE_A)!=RET_OK)
    {
        LOG_E(-1,"promote B to A fail\r\n");
        return RET_ERR;
    }

    cfg.state    = OTA_STATE_CONFIRMED;
    cfg.aSize    = cfg.size;
    cfg.aCrc32   = cfg.crc32;
    cfg.retry    = 0;
    OTA_CfgWrite(&cfg);

#if OTA_BOOT_HANDSHAKE
    /* 同步 Boot 侧配置: 出厂(稳定)固件更新为本次升级后的版本 */
    {
        OtaSysConf_t sc;
        memset(&sc,0,sizeof(sc));
        OTA_SysConf_Get(&sc);
        sc.factoryFirmware.size  = cfg.aSize;
        sc.factoryFirmware.crc32 = cfg.aCrc32;
        sc.isUpdate              = 0;
        sc.factoryFlag           = 0;
        OTA_SysConf_Set(&sc);
    }
#endif

    LOG_I("ota confirmed,B->A done\r\n");
    return RET_OK;
}


/*!
 * 手动触发: 把 B 区(packet_2.bin)写进片上备份区并置更新标志,
 * 复位后由 BootLoader 搬运到 APP 区(不会在 App 里擦写自身代码)。
 */
int OTA_ApplyFromB(void)
{
    OtaCfg_t cfg;
    uint32_t crc=0;

    if(OTA_CfgRead(&cfg)!=RET_OK)
    {
        LOG_E(-1,"no ota cfg\r\n");
        return RET_ERR;
    }
    if(cfg.size==0 || cfg.size>OTA_IMAGE_MAX_SIZE)
    {
        LOG_E(-1,"cfg size invalid:%lu\r\n",cfg.size);
        return RET_ERR;
    }
    if(OTA_File_Crc32(OTA_FILE_B,cfg.size,&crc)!=RET_OK || crc!=cfg.crc32)
    {
        LOG_E2(-1,"B crc fail,rx:%08lx,cal:%08lx\r\n",cfg.crc32,crc);
        return RET_ERR;
    }

#if OTA_BOOT_HANDSHAKE
    if(OTA_SaveSlot_Write(OTA_FILE_B,cfg.size,cfg.crc32)!=RET_OK)
    {
        LOG_E(-1,"save slot write fail\r\n");
        return RET_ERR;
    }

    {
        OtaSysConf_t sc;
        memset(&sc,0,sizeof(sc));
        OTA_SysConf_Get(&sc);
        OTA_SysConfInitIfEmpty(&sc,cfg.aSize,cfg.aCrc32);
        sc.updateFireware.size  = cfg.size;
        sc.updateFireware.crc32 = cfg.crc32;
        sc.isUpdate             = 1;
        if(OTA_SysConf_Set(&sc)!=RET_OK)
        {
            LOG_E(-1,"write boot sys conf fail\r\n");
            return RET_ERR;
        }
    }

    cfg.state = OTA_STATE_PENDING;
    cfg.retry = 0;
    OTA_CfgWrite(&cfg);

    LOG_I("apply ok,please reboot\r\n");
    return RET_OK;
#else
    LOG_E(-1,"未启用 Boot 握手, 无法应用 (可执行 ota_rollback 或直接烧写)\r\n");
    return RET_ERR;
#endif
}


int OTA_Rollback(void)
{
    OtaCfg_t cfg;

    if(OTA_CfgRead(&cfg)!=RET_OK)
    {
        LOG_E(-1,"no ota cfg\r\n");
        return RET_ERR;
    }

    cfg.state = OTA_STATE_ROLLBACK;
    OTA_CfgWrite(&cfg);

    return OTA_DoRollback(&cfg);
}


uint8_t  OTA_IsUpdating(void)   { return otaUpdating; }
uint32_t OTA_GetRxSize(void)    { return otaRxSize; }
uint32_t OTA_GetTotalSize(void) { return otaTotalSize; }

uint32_t OTA_GetProgress(void)
{
    if(otaTotalSize==0) return 0;
    return (uint32_t)(otaRxSize*100/otaTotalSize);
}
