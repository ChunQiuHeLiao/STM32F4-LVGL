#include"ota_boot.h"
#include"ota_ab.h"
#include"debug.h"


/*!
 * 系统配置区读写 + 片上备份区(SAVE_APP)写入
 * 配置区/备份区都与 Boot 工程共用同一套地址, 详见 ota_boot.h
 */


/// @brief 从 Flash 配置区读系统配置
int OTA_SysConf_Get(OtaSysConf_t* cfg)
{
    if(cfg==NULL) return RET_ERR;

    memcpy(cfg,(const void*)OTA_SYS_CFG_ADDR,sizeof(OtaSysConf_t));

    /* 配置区为空(0xFF) -> 首次运行 */
    if(cfg->factoryFlag==0xff)
    {
        memset(cfg,0,sizeof(OtaSysConf_t));
        cfg->factoryFlag = 0xff;   /* 交给调用者填出厂信息 */
    }

    return RET_OK;
}


/// @brief 把系统配置写回 Flash 配置区(先擦后写)
int OTA_SysConf_Set(OtaSysConf_t* cfg)
{
    if(cfg==NULL) return RET_ERR;

    int ret = OTA_Flash_Erase(OTA_SYS_CFG_ADDR,OTA_SYS_CFG_SIZE);
    if(ret!=RET_OK)
    {
        LOG_E(ret,"sys cfg erase");
        return ret;
    }

    ret = OTA_Flash_Write(OTA_SYS_CFG_ADDR,(uint8_t*)cfg,sizeof(OtaSysConf_t));
    if(ret!=RET_OK)
    {
        LOG_E(ret,"sys cfg write");
        return ret;
    }

    return RET_OK;
}


/// @brief 擦除片上备份区并把 SD 卡镜像写进去(Boot 会从这里搬到 APP 区)
int OTA_SaveSlot_Write(const char* path,uint32_t size,uint32_t crc32)
{
#if OTA_HAS_SAVE_SLOT
    if(size>OTA_SAVE_APP_SIZE)
    {
        LOG_E(-1,"fw too big for save slot:%lu>%lu\r\n",size,OTA_SAVE_APP_SIZE);
        return RET_ERR;
    }
    return OTA_ProgramImageFromSdEx(path,size,crc32,OTA_SAVE_APP_ADDR);
#else
    LOG_E(-1,"no on-chip save slot(F411 APP too big), Boot should read %s from SD\r\n",path);
    return RET_ERR;
#endif
}
