#ifndef __WIFI_APP_H
#define __WIFI_APP_H
#include"main.h"
#include"esp8266.h"


/*WIFI 数据处理周期(ms)*/
#define WIFI_APP_HANDLE_PERIOD 10 

/*发送命令等待的时间 ms*/
#define WIFI_APP_CMD_WAIT_TIME 5000 


uint8_t WIFI_APP_Init(uint32_t baudrate);
void WIFI_APP_Handler();


uint8_t WIFI_APP_CheckUpdateFlag_FromServer(uint8_t* updateFlag);
uint8_t WIFI_APP_GetPackageSize(uint32_t* allByte);
uint8_t WIFI_APP_Download_Pack(const char *filePath,uint32_t allByte,uint32_t* revAllByte);



/*一些常用的指令，如果没有，自行使用 WIFI_SendCmd会 WIFI_SendCmd_Detail 来自行编写**/

uint8_t WIFI_APP_ScanAP(char* response,uint16_t size);
char*   WIFI_APP_GetWifiName(char* response,char* wifiName,uint8_t size);
uint8_t WIFI_APP_ConnectAP(const char* wifiName,const char* wifiPwd);
uint8_t WIFI_APP_GetConnWifiName(char* conWifiName);
uint8_t WIFI_APP_Set_CIPMODE(uint8_t mode);


uint8_t WIFI_APP_ConnectServer(const char* protol,const char* ip,const uint16_t port);
uint8_t WIFI_APP_DiconnectServer();
uint8_t WIFI_APP_EN_SendData();
uint8_t WIFI_APP_EXIT_SendData();

void WIFI_APP_Delay();

uint8_t WIFI_APP_GetAPIResponse(const char* host, const char* path, char** response);
#endif