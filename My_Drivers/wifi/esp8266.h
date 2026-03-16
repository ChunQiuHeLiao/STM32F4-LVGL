/*
1. WIFI 模块各个指令最好最好超时等待位为10s,连接WIFI的指令到20s
*/


#ifndef __ESP8266_H
#define __ESP8266_H
#include"main.h"
#include"wifi.h"




#define ESP8266_SYS_IP "192.168.50.109"  //该系统更新连接的服务器的IP
#define ESP8266_SYS_PORT 9527 //该系统更新连接的服务器的端口

#define ESP8266_BASE_TIMEOUT_TIME 10000 //(ms)

#define ESP8266_GPIO_EN_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE() //高电平有效
#define ESP8266_GPIO_EN_PORT GPIOA
#define ESP8266_GPIO_EN_PIN GPIO_PIN_10

#define ESP8266_GPIO_RST_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE() //复位高低高即可复位完成
#define ESP8266_GPIO_RST_PORT GPIOA
#define ESP8266_GPIO_RST_PIN GPIO_PIN_11



void ESP8266_Init(uint32_t baudRate);



/***************命令函数***************/

uint8_t ESP8266_Cmd_ScanAP(uint8_t* rxData,uint16_t size);
char* ESP8266_GetWifiName(char* response,char* wifiName,uint8_t size);
uint8_t ESP8266_Cmd_ConnectAP(uint8_t* wifiName,uint8_t* wifiPwd);
uint8_t ESP8266_SelectConnAP(char* conWifiName);
uint8_t ESP8266_Cmd_Set_CIPMODE(uint8_t mode);


uint8_t ESP8266_Cmd_ConnectServer(const uint8_t* protol,const uint8_t* ip,const uint16_t port);
uint8_t ESP8266_Cmd_DisConnectServer();
uint8_t ESP8266_Cmd_EN_SendData();
uint8_t ESP8266_Cmd_EXIT_SendData();

void ESP8266_Delay();

#endif
