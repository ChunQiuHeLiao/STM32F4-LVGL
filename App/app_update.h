#ifndef __APP_UPDATE_H
#define __APP_UPDATE_H

#include"main.h"
#include"transfer_protocol.h"


typedef struct
{
    uint32_t rxAllSize;   /*当前接收到的总数据*/
    uint32_t allSize;     /*固件的总数据大小*/
    uint32_t crc32;       /*本次固件的CRC32*/
    uint8_t  isUpdating;  /*是否正在升级*/
}AppUpdate_Info_t;


extern AppUpdate_Info_t updateInfo;


int AppUpdate_Start(DataFrame_t* df);
int AppUpdate_Updating(DataFrame_t* df);
int AppUpdate_Finished(DataFrame_t* df);

#endif
