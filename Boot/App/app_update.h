#ifndef __APP_UPDATE_H
#define __APP_UPDATE_H
#include"main.h"
#include"stm_flash.h"


#ifndef USE_BOOT

#include"transfer_protocol.h"

typedef struct
{
    uint32_t rxAllSize; //当前接收到的总数据
    uint32_t allSize; //固件的总数据大小
    uint8_t* readFlashBuf; //最后校验更新的整个固件时，读取一页数据的缓冲区
    uint8_t isUpdating; //
}AppUpdate_Info_t;


extern AppUpdate_Info_t updateInfo;

int AppUpdate_Start(DataFrame_t* df);
int AppUpdate_Updating(DataFrame_t* df);
int AppUpdate_Finished(DataFrame_t* df);

#else 

int AppUpdate_UpdateFirmware();

#endif


#endif