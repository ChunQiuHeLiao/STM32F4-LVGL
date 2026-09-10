#ifndef __OTA_AB_H
#define __OTA_AB_H

#include"main.h"
#include"ff.h"
#include"ota_boot.h"   /*与 Boot 工程共享的分区/配置约定*/


/*====================================================================
 * OTA  A/B 区升级  (固件分块接收 + SD卡双固件存储 + Flash擦写 + CRC校验
 *                   + 升级确认 + 失败回滚)
 *
 *  存储层次:
 *   1) SD 卡双固件(AB区)
 *        A区 : S:/ota/packet_1.bin   上一次"已确认可用"的固件, 用于失败回滚
 *        B区 : S:/ota/packet_2.bin   本次 OTA 接收到的新固件
 *   2) 升级状态机: S:/ota/ota.cfg (带 CRC 自校验, 掉电不丢)
 *   3) Flash 侧与 Boot 的握手(见 ota_boot.h):
 *        - 把 B 区固件写进片上备份区 SAVE_APP(若空间足够)
 *        - 把 updateFireware/isUpdate=1 写进 Boot 尾部的配置区
 *        - 复位后由 Boot 把备份区搬到 APP 区并校验, 成功后清标志
 *   4) App 侧 OTA_Boot_Run(): 复位后处理"确认 / 失败回滚"
 *====================================================================*/

/*------------------- SD 卡双固件存储 -------------------*/
#define OTA_DIR          "S:/ota"
#define OTA_FILE_A       "S:/ota/packet_1.bin"   /* A区: 稳定版/回滚用 */
#define OTA_FILE_B       "S:/ota/packet_2.bin"   /* B区: 本次接收的新固件 */
#define OTA_FILE_CFG     "S:/ota/ota.cfg"        /* 升级状态机(掉电不丢) */

/*------------------- 内部 FLASH 分区 -------------------*/
/* 具体地址定义见 ota_boot.h(与 Boot/Core/Inc/utility.h 一一对应) */
#define OTA_FLASH_BASE       ((uint32_t)0x08000000)
#define OTA_FLASH_SIZE       (512*1024)
#define OTA_APP_ADDR         OTA_RUN_APP_ADDR            /* 本固件 APP 运行区起始 */
#define OTA_IMAGE_MAX_SIZE   OTA_RUN_APP_MAX_SIZE        /* APP 镜像最大尺寸 */

/* 擦写/校验时使用的块大小, 越小越省 RAM */
#define OTA_BLOCK_SIZE       (512)

/* 新固件连续启动失败多少次后自动回滚 */
#define OTA_MAX_RETRY        (3)


/* 是否启用与 ./Boot 工程的分区/配置握手, 见 ota_boot.h(OTA_APP_RELOCATED / OTA_BOOT_HANDSHAKE) */

#define OTA_CFG_MAGIC  0x4F544131UL /* "OTA1" */


/// @brief 升级状态机
typedef enum
{
    OTA_STATE_IDLE = 0,      /* 无升级任务 */
    OTA_STATE_READY,         /* B区固件已收齐且CRC通过, 等待Boot搬运 */
    OTA_STATE_PENDING,       /* 已烧写完成, 等待新固件运行后确认 */
    OTA_STATE_CONFIRMED,     /* 新固件确认成功 */
    OTA_STATE_ROLLBACK       /* 需要/正在回滚 */
}OtaState_t;


/// @brief 升级状态(存 S:/ota/ota.cfg, 带 CRC 自校验防止写坏)
typedef struct
{
    uint32_t magic;
    uint32_t state;
    uint32_t size;      /* B区(新固件)大小 */
    uint32_t crc32;     /* B区(新固件)CRC32 */
    uint32_t aSize;     /* A区(稳定固件)大小 */
    uint32_t aCrc32;    /* A区(稳定固件)CRC32 */
    uint32_t retry;     /* 连续启动失败次数 */
    uint32_t reserved;
    uint32_t crc;       /* 以上字段的 CRC32, 防止状态区写坏 */
}OtaCfg_t;


/*---------------------- 对外接口 ----------------------*/

/// @brief 上电Boot阶段调用(在 FreeRTOS 启动之前), 完成烧写/确认/回滚
int OTA_Boot_Run(void);

/// @brief OTA 业务初始化(创建B区文件等由 OTA_Start 完成)
int OTA_Init(void);

/// @brief 开始一次升级, 会清空并新建 B区(packet_2.bin)
/// @param fwSize 本次固件的总大小
int OTA_Start(uint32_t fwSize);

/// @brief 分块写入 B区(SD卡)
int OTA_WritePacket(uint8_t* data,uint16_t len);

/// @brief 收完调用: 校验大小 + CRC32, 备份当前固件到A区, 置 READY
/// @param crc32 上位机下发的整包 CRC32
int OTA_Finish(uint32_t crc32);

/// @brief 传输/校验失败时调用, 丢弃B区, 保持A区不变(回滚)
void OTA_Abort(void);

/// @brief 新固件跑起来并稳定后调用, 确认升级, 并把 B区提升为新的 A区
int OTA_Confirm(void);

/// @brief 主动回滚到 A 区固件(会重新烧写并复位)
int OTA_Rollback(void);

/// @brief 手动触发: 立即擦除并把 B 区固件烧写到 APP 区(烧完需复位)
int OTA_ApplyFromB(void);

uint8_t  OTA_IsUpdating(void);
uint32_t OTA_GetRxSize(void);
uint32_t OTA_GetTotalSize(void);
uint32_t OTA_GetProgress(void);   /* 0~100 */
uint32_t OTA_GetRunningImageSize(void); /* 当前正在运行固件在 Flash 中的大小 */

/*---------------------- Flash 擦写 + 校验 ----------------------*/

/// @brief 擦除 [addr,addr+size) 覆盖到的所有扇区
int OTA_Flash_Erase(uint32_t addr,uint32_t size);

/// @brief 写入数据(不擦除), 内部关中断 + RAM 中执行编程时序
int OTA_Flash_Write(uint32_t addr,uint8_t* data,uint32_t size);

/// @brief 计算 Flash 中内容的 CRC32
int OTA_Flash_Crc32(uint32_t addr,uint32_t size,uint32_t* crc32);

/// @brief 计算 SD 卡文件的 CRC32
int OTA_File_Crc32(const char* path,uint32_t size,uint32_t* crc32);

/// @brief 把 SD 卡镜像烧写到 APP 区(擦除+写入+回读CRC校验)。BootLoader 使用
int OTA_ProgramImageFromSd(const char* path,uint32_t size,uint32_t crc32);

#endif
