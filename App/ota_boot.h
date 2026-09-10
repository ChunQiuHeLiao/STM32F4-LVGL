#ifndef __OTA_BOOT_H
#define __OTA_BOOT_H

#include"main.h"

/*!
 * =====================================================================
 *  与独立 BootLoader 工程(./Boot) 共享的分区与配置约定
 * =====================================================================
 *  本文件必须与 Boot/Core/Inc/utility.h 里下面这些宏保持一致:
 *      BOOT_SIZE / APP_START_ADDR / APP_SIZE / SAVE_APP_START_ADDR
 *      SYS_CFG_SIZE / SYS_CFG_START_ADDR / SysConfigInfo_t
 *
 *  Boot 侧流程(不需要改动):
 *      1. SysConfInfo_Get()  从 SYS_CFG_START_ADDR 读配置
 *      2. sysConfInfo.isUpdate==1 时:
 *         AppUpdate_UpdateFirmware():
 *            - STM_FLASH_Erase(APP_START_ADDR, 新固件页数)
 *            - 从 SAVE_APP_START_ADDR 逐页搬到 APP_START_ADDR
 *            - CRC32 校验, 失败则不改标志(下次继续尝试)
 *            - 成功 -> isUpdate=0, SysConfInfo_Set() 写回
 *      3. SYS_JumpApp(APP_START_ADDR)
 *
 * ---------------------------------------------------------------------
 *  STM32F411xE 的扇区: 16K*4 + 64K + 128K*3 = 512K
 *  由于擦除粒度是扇区(最小16KB), 配置区必须独占一个扇区, 否则擦配置会把
 *  Boot 或 APP 一起擦掉。所以这里 BOOT_SIZE 取 0x8000(32KB):
 *      扇区0 (0x08000000~0x08003FFF, 16KB) : Boot 代码
 *      扇区1 (0x08004000~0x08007FFF, 16KB) : 系统配置扇区(配置放在本扇区末尾)
 *      扇区2~7(0x08008000~0x0807FFFF,480KB): APP
 *  Boot 工程里只要把 BOOT_SIZE 改成 0x8000 即可, 其余代码完全不用动。
 * =====================================================================
 */

#define OTA_CHIP_FLASH_SIZE   (512)                  /*芯片 FLASH 大小(KB)*/

/* Boot 区总大小(含配置扇区), 必须 == Boot/Core/Inc/utility.h 的 BOOT_SIZE */
#define OTA_BOOT_SIZE         (0x8000)               /*32KB*/

/* 系统配置区: Boot 区最后 0x50 字节, 必须 == Boot 的 SYS_CFG_START_ADDR */
#define OTA_SYS_CFG_SIZE      (0x50)
#define OTA_SYS_CFG_ADDR      (FLASH_BASE + OTA_BOOT_SIZE - OTA_SYS_CFG_SIZE)

/* APP 区, 必须 == Boot 的 APP_START_ADDR */
#define OTA_BOOT_APP_ADDR     (FLASH_BASE + OTA_BOOT_SIZE)

/* APP 区大小, 必须 == Boot 的 APP_SIZE(这里把剩下的都给 APP) */
#define OTA_BOOT_APP_SIZE     (OTA_CHIP_FLASH_SIZE*1024 - OTA_BOOT_SIZE)

/* 片上备份区(SAVE_APP), 必须 == Boot 的 SAVE_APP_START_ADDR
 * 注意: 这几个宏会在 #if 里使用, 所以不能带类型转换 */
#define OTA_SAVE_APP_ADDR     (OTA_BOOT_APP_ADDR + OTA_BOOT_APP_SIZE)
#define OTA_SAVE_APP_SIZE     (OTA_CHIP_FLASH_SIZE*1024 - OTA_BOOT_SIZE - OTA_BOOT_APP_SIZE)

/* 片上备份区是否真的有空间(F411 上当前 APP 太大, 一般为 0, 此时用 SD 卡 A/B 区暂存) */
#define OTA_HAS_SAVE_SLOT     (OTA_SAVE_APP_SIZE >= (64*1024))


/*!
 * 当前这份 APP 固件是否已经重定位到 OTA_BOOT_APP_ADDR。
 * 0: 仍然链接在 0x08000000(没有烧 Boot, 配置区/备份区落在 APP 自己身上, 只能关掉握手)
 * 1: 已链接到 APP 区, 完整的 Boot 握手生效
 * 重定位方法见文件末尾注释。
 */
#ifndef OTA_APP_RELOCATED
#define OTA_APP_RELOCATED     0
#endif

/* 是否启用与 Boot 的握手(写代码到配置区 + 片上备份区) */
#ifndef OTA_BOOT_HANDSHAKE
#define OTA_BOOT_HANDSHAKE    OTA_APP_RELOCATED
#endif

#if OTA_BOOT_HANDSHAKE && !OTA_APP_RELOCATED
#error "OTA_BOOT_HANDSHAKE=1 需要先把 APP 重定位到 OTA_BOOT_APP_ADDR, 见 ota_boot.h 末尾说明"
#endif

#if OTA_APP_RELOCATED
#define OTA_RUN_APP_ADDR      OTA_BOOT_APP_ADDR      /*本固件运行基址*/
#define OTA_RUN_APP_MAX_SIZE  OTA_BOOT_APP_SIZE
#else
#define OTA_RUN_APP_ADDR      FLASH_BASE
#define OTA_RUN_APP_MAX_SIZE  (OTA_CHIP_FLASH_SIZE*1024)
#endif


/*!
 * 系统配置结构体 —— 必须与 Boot/Core/Inc/utility.h 的 SysConfigInfo_t 完全一致
 * (自然对齐, 共 20 字节)
 */
typedef struct
{
    uint32_t size;    /*固件大小*/
    uint32_t crc32;   /*CRC32校验码*/
}OtaFwInfo_t;

typedef struct
{
    OtaFwInfo_t factoryFirmware;  /*出厂/上一次确认可用的固件(回滚基点, 对应 SD 卡 packet_1.bin)*/
    OtaFwInfo_t updateFireware;   /*本次待升级的固件(对应 SD 卡 packet_2.bin)*/

    uint8_t isUpdate;             /*1: Boot 需要把备份区搬到 APP 区*/
    uint8_t factoryFlag;          /*0xff: 配置区为空(首次运行), 0: 已初始化*/
    uint8_t reverser1;
    uint8_t reverser2;
}OtaSysConf_t;


int OTA_SysConf_Get(OtaSysConf_t* cfg);
int OTA_SysConf_Set(OtaSysConf_t* cfg);

/// @brief 擦除片上备份区(SAVE_APP)并把 SD 卡镜像写进去, 写完回读 CRC 校验
int OTA_SaveSlot_Write(const char* path,uint32_t size,uint32_t crc32);

/// @brief 把 SD 卡镜像烧写到指定 Flash 地址(擦除+写入+回读CRC)
int OTA_ProgramImageFromSdEx(const char* path,uint32_t size,uint32_t crc32,uint32_t baseAddr);


#endif

/*
 * ===================== 把 APP 重定位到 APP 区 =====================
 * 1. STM32F411XX_FLASH.ld:
 *        FLASH (rx) : ORIGIN = 0x8008000, LENGTH = 480K
 * 2. Core/Src/system_stm32f4xx.c 顶部放开:
 *        #define USER_VECT_TAB_ADDRESS
 *        并在其下方定义 #define VECT_TAB_OFFSET 0x00008000U
 *    (或直接在 main() 开头调用 SYS_Set_Vector_Table(FLASH_BASE, 0x8000))
 * 3. 本文件(或编译宏)里把 OTA_APP_RELOCATED 置 1
 * 4. Boot/Core/Inc/utility.h 里 BOOT_SIZE 改成 0x8000
 * 5. 先烧 Boot.hex, 再烧 App.hex(地址 0x08008000)
 * ================================================================
 */
