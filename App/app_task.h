#ifndef __APP_TASK_H
#define __APP_TASK_H

#include"main.h"
#include"transfer_protocol.h"


/*====================================================================
 * FreeRTOS 多任务划分
 *   UartRx_Task      : 串口(ESP8266)接收, 由中断释放二值信号量唤醒
 *   FrameParse_Task  : 协议解析, 解析出的帧通过消息队列发给 OTA 任务
 *   Ota_Task         : OTA 升级, 分块写入 SD 卡 B 区(packet_2.bin)
 *   Lvgl_Task        : LVGL 心跳 + UI 业务
 *   Debug_Task       : 串口命令调试
 *   Iwdg_Task        : 看门狗喂狗 + 升级确认
 *
 * 任务间通信与同步:
 *   消息队列 g_frameQueue  : FrameParse_Task -> Ota_Task (数据通信)
 *   互斥锁   g_sdMutex     : 保护 SD 卡这一共享资源
 *   信号量   g_uartRxSem   : 串口中断 -> UartRx_Task (同步)
 *   信号量   g_frameSem    : UartRx_Task -> FrameParse_Task (同步)
 *   信号量   g_uiReadySem  : Lvgl_Task -> Iwdg_Task (UI就绪后确认升级)
 *====================================================================*/

/* ESP8266 / OTA 使用的串口号(见 usart.h, 1=USART6, 2=USART2) */
#define OTA_UART_ID  1

/* 调试命令使用的串口号(printf 也走这个口) */
#define DEBUG_UART_ID 2


/* 任务句柄 */
extern TaskHandle_t uartRxTaskHandle;
extern TaskHandle_t frameParseTaskHandle;
extern TaskHandle_t otaTaskHandle;
extern TaskHandle_t lvglTaskHandle;
extern TaskHandle_t debugTaskHandle;
extern TaskHandle_t iwdgTaskHandle;

/* 共享资源 */
extern SemaphoreHandle_t g_sdMutex;     /* SD 卡互斥锁 */
extern SemaphoreHandle_t g_uartRxSem;   /* 串口接收(中断->任务) */
extern SemaphoreHandle_t g_frameSem;    /* 有新数据待解析 */
extern SemaphoreHandle_t g_uiReadySem;  /* UI 就绪 */
extern QueueHandle_t     g_frameQueue;  /* 协议帧队列 */


/// @brief 通过消息队列传递的帧(只传必要部分, 节省队列内存)
typedef struct
{
    uint8_t  funcCode;
    uint16_t len;
    uint8_t  data[DATA_PACKET_MAX_SIZE];
}AppFrameMsg_t;


void App_Task_Init(void);

#endif
