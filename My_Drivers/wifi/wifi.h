#ifndef __WIFI_H
#define __WIFI_H
#include"main.h"
#include"main_lib.h"
#define WIFI_USE_UART_ID 1
#define WIFI_USE_LVGL 1


uint8_t WIFI_SendData(char* data,uint16_t size);
uint8_t WIFI_RevData(char* rxData,uint16_t size);
uint8_t WIFI_RevDataPointer(char** rxData);
uint16_t WIFI_RevDataSize();

uint8_t WIFI_SendCmd(uint8_t* cmd,uint8_t* resPart,uint16_t timeout);
uint8_t WIFI_SendCmd_Detail(uint8_t* cmd,uint8_t* resPart,char* response,uint16_t size,uint16_t timeout);




#endif