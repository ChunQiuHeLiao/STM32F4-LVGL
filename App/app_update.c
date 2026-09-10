#include"app_update.h"
#include"ota_ab.h"
#include"debug.h"


AppUpdate_Info_t updateInfo={0};


/*!
 * 升级开始帧: 携带本次固件的总大小。
 * 收到后清空并新建 SD 卡 B 区(S:/ota/packet_2.bin), 等待分块数据。
 */
int AppUpdate_Start(DataFrame_t* df)
{
    updateInfo.isUpdating = 1;
    updateInfo.allSize    = 0;
    updateInfo.rxAllSize  = 0;
    updateInfo.crc32      = 0;

    /*1.获取本次传输的固件大小(小端)*/
    updateInfo.allSize = ((uint32_t)df->body.data[3]<<24) |
                         ((uint32_t)df->body.data[2]<<16) |
                         ((uint32_t)df->body.data[1]<<8)  |
                         ((uint32_t)df->body.data[0]);

    LOG_I("rx fw size:%lu\r\n",updateInfo.allSize);

    /*2.新建 B 区文件, 准备分块接收*/
    if(OTA_Start(updateInfo.allSize)!=RET_OK)
    {
        updateInfo.isUpdating = 0;
        DataFrame_Response(ERR_CODE_UNKONW);
        return RET_ERR;
    }

    DataFrame_Response(ERR_CODE_OK);
    return RET_OK;
}


/*!
 * 升级数据帧: 分块写入 SD 卡 B 区, 并累加 CRC32。
 * 异常时 OTA_WritePacket 返回错误, 这里上报错误码, 上位机应重发或终止。
 */
int AppUpdate_Updating(DataFrame_t* df)
{
    if(OTA_WritePacket(df->body.data,df->head.len)!=RET_OK)
    {
        DataFrame_Response(ERR_CODE_WRITE_FLASH_ERR);
        return RET_ERR;
    }

    updateInfo.rxAllSize = OTA_GetRxSize();

    printf("\r\n%lu/%lu,%lu%%\r\n",
        updateInfo.rxAllSize,updateInfo.allSize,OTA_GetProgress());

    DataFrame_Response(ERR_CODE_OK);
    return RET_OK;
}


/*!
 * 升级结束帧: 携带整包 CRC32。
 *  1) 校验接收长度
 *  2) 回读 SD 卡 B 区文件做 CRC32 校验
 *  3) 备份当前运行固件到 A 区(packet_1.bin), 作为回滚基点
 *  4) 置状态为 READY, 复位后由 Boot 阶段擦写 Flash 生效
 * 任一步失败 -> OTA_Abort() 丢弃 B 区, 保留 A 区(失败回滚)
 */
int AppUpdate_Finished(DataFrame_t* df)
{
    updateInfo.crc32 = ((uint32_t)df->body.data[3]<<24) |
                       ((uint32_t)df->body.data[2]<<16) |
                       ((uint32_t)df->body.data[1]<<8)  |
                       ((uint32_t)df->body.data[0]);

    if(OTA_Finish(updateInfo.crc32)!=RET_OK)
    {
        updateInfo.isUpdating = 0;
        DataFrame_Response(ERR_CODE_FIRMWARE_CRC_ERR);
        return RET_ERR;
    }

    DataFrame_Response(ERR_CODE_OK);

    updateInfo.isUpdating = 0;
    return RET_OK;
}
