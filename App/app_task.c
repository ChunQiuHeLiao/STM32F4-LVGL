#include"app_task.h"
#include"ota_ab.h"
#include"app_update.h"
#include"app_frame_parse.h"
#include"transfer_protocol.h"
#include"debug.h"
#include"iwdg.h"
#include"fs_api.h"

/*LVGL*/
#include"lvgl.h"
#include"lv_port_disp_template.h"
#include"lv_port_indev_template.h"
#include"ui.h"


/*=================================================================
 * 共享资源: 互斥锁 / 信号量 / 消息队列
 *=================================================================*/
SemaphoreHandle_t g_sdMutex    = NULL;  /* SD 卡互斥锁 */
SemaphoreHandle_t g_uartRxSem  = NULL;  /* 串口中断 -> 接收任务 */
SemaphoreHandle_t g_frameSem   = NULL;  /* 接收任务 -> 解析任务 */
SemaphoreHandle_t g_uiReadySem = NULL;  /* LVGL 任务 -> 看门狗任务 */
QueueHandle_t     g_frameQueue = NULL;  /* 解析任务 -> OTA 任务 */


/*=================================================================
 * 任务句柄
 *=================================================================*/
TaskHandle_t uartRxTaskHandle     = NULL;
TaskHandle_t frameParseTaskHandle = NULL;
TaskHandle_t otaTaskHandle        = NULL;
TaskHandle_t lvglTaskHandle       = NULL;
TaskHandle_t debugTaskHandle      = NULL;
TaskHandle_t iwdgTaskHandle       = NULL;


/* 任务栈大小(单位: word, 1word = 4Byte) */
#define UART_RX_TASK_STACK      (160)
#define FRAME_PARSE_TASK_STACK  (160)
#define OTA_TASK_STACK          (192)
#define LVGL_TASK_STACK         (768)
#define DEBUG_TASK_STACK        (160)
#define IWDG_TASK_STACK         (128)

/* 任务优先级 */
#define UART_RX_TASK_PRIO       (5)
#define FRAME_PARSE_TASK_PRIO   (4)
#define OTA_TASK_PRIO           (4)
#define LVGL_TASK_PRIO          (3)
#define DEBUG_TASK_PRIO         (2)
#define IWDG_TASK_PRIO          (6)

/* 新固件稳定运行多久后确认升级(ms) */
#define OTA_CONFIRM_DELAY_MS    (15000)


static uint8_t  gUartRxBuf[256];   /* 串口接收缓冲(放全局, 减小任务栈压力) */
static uint8_t  gConfirmDone = 0;


/*=================================================================
 * 任务 1: 串口(ESP8266)接收
 *  中断里释放二值信号量, 该任务被唤醒后把数据搬进环形缓冲区
 *=================================================================*/
static void UartRx_Task(void* args)
{
    Task_PrintStart();

    while(1)
    {
        /* 等中断通知(信号量实现中断与任务的同步) */
        if(xSemaphoreTake(g_uartRxSem,portMAX_DELAY)!=pdTRUE) continue;

        uint16_t rxSize = UART_GetRxDataSize(OTA_UART_ID);
        if(rxSize==0) continue;
        if(rxSize>sizeof(gUartRxBuf)) rxSize=sizeof(gUartRxBuf);

        if(UART_GetRxData((char*)gUartRxBuf,rxSize,OTA_UART_ID)!=0) continue;

        /* 环形缓冲区自带互斥锁 */
        RingBuffer_Write(&uartRingBuf,gUartRxBuf,rxSize);

        /* 通知解析任务 */
        xSemaphoreGive(g_frameSem);
    }
}


/*=================================================================
 * 任务 2: OTA 升级
 *  从消息队列里取解析好的帧, 交给升级流程处理
 *=================================================================*/
static DataFrame_t otaDf;

static void Ota_Task(void* args)
{
    AppFrameMsg_t msg;
    Task_PrintStart();

    while(1)
    {
        /* 等消息队列(任务间数据通信) */
        if(xQueueReceive(g_frameQueue,&msg,portMAX_DELAY)!=pdTRUE) continue;

        memset(&otaDf,0,sizeof(DataFrame_t));
        otaDf.head.len        = msg.len;
        otaDf.body.funcCode   = msg.funcCode;
        if(msg.len>DATA_PACKET_MAX_SIZE) msg.len=DATA_PACKET_MAX_SIZE;
        memcpy(otaDf.body.data,msg.data,msg.len);

        int ret = RET_ERR;
        switch(msg.funcCode)
        {
            case FUNC_CODE_UPDATE_START:
                ret = AppUpdate_Start(&otaDf);
                break;

            case FUNC_CODE_UPDATING:
                ret = AppUpdate_Updating(&otaDf);
                break;

            case FUNC_CODE_UPDAT_FINISHED:
                ret = AppUpdate_Finished(&otaDf);
                if(ret==RET_OK)
                {
                    LOG_I("update ok,resetting\r\n");
                    updateInfo.isUpdating = 0;
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    Sys_SoftReset();   /* 复位后由 Boot 阶段烧写新固件 */
                }
                break;

            default:
                break;
        }

        if(ret!=RET_OK)
        {
            /* 失败: OTA_AB 内部已做回滚(丢弃B区, 保留A区) */
            updateInfo.isUpdating = 0;
        }
    }
}


/*=================================================================
 * 任务 3: LVGL 心跳 + UI 业务
 *=================================================================*/
static void Lvgl_Task(void* args)
{
    Task_PrintStart();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    ui_init();

    /* UI 就绪, 释放信号量通知其它任务 */
    xSemaphoreGive(g_uiReadySem);

    uint16_t tick=0;
    while(1)
    {
        Flag_Handler();

        uint32_t nextTime = lv_timer_handler();
        if(nextTime<1)  nextTime=1;
        if(nextTime>50) nextTime=50;

        vTaskDelay(pdMS_TO_TICKS(nextTime));

        if(++tick>=200)
        {
            tick=0;
            printf("running...\n");
        }
    }
}


/*=================================================================
 * 任务 4: 看门狗喂狗 + 升级确认
 *=================================================================*/
static void Iwdg_Task(void* args)
{
    Task_PrintStart();

    uint32_t runMs=0;
    uint8_t  uiReady=0;

    while(1)
    {
        IWDG_Reflesh();

        vTaskDelay(pdMS_TO_TICKS(500));
        runMs += 500;

        if(!uiReady && g_uiReadySem!=NULL)
        {
            /* 信号量同步: 等 UI 起来后再开始计时确认 */
            if(xSemaphoreTake(g_uiReadySem,0)==pdTRUE) uiReady=1;
        }

        /* 新固件稳定运行一段时间后确认升级(B->A) */
        if(uiReady && !gConfirmDone && runMs>=OTA_CONFIRM_DELAY_MS)
        {
            gConfirmDone = 1;
            if(OTA_Confirm()==RET_OK)
            {
                LOG_I("ota confirm done\r\n");
            }
        }
    }
}


/*=================================================================
 * 资源与任务创建
 *=================================================================*/
void App_Task_Init(void)
{
    BaseType_t ret;

    /*-------- 1. 共享资源 --------*/
    g_sdMutex = xSemaphoreCreateMutex();          /* SD 卡互斥锁 */
    if(g_sdMutex==NULL) LOG_E(-1,"xSemaphoreCreateMutex sd");

    g_uartRxSem = xSemaphoreCreateBinary();       /* 中断同步信号量 */
    if(g_uartRxSem==NULL) LOG_E(-1,"xSemaphoreCreateBinary uartRx");

    g_frameSem = xSemaphoreCreateBinary();        /* 解析同步信号量 */
    if(g_frameSem==NULL) LOG_E(-1,"xSemaphoreCreateBinary frame");

    g_uiReadySem = xSemaphoreCreateBinary();      /* UI 就绪同步信号量 */
    if(g_uiReadySem==NULL) LOG_E(-1,"xSemaphoreCreateBinary uiReady");

    /* 协议帧消息队列: 解析任务 -> OTA 任务 */
    g_frameQueue = xQueueCreate(2,sizeof(AppFrameMsg_t));
    if(g_frameQueue==NULL) LOG_E(-1,"xQueueCreate frame");

    /*-------- 2. OTA 与协议解析 --------*/
    OTA_Init();
    AppFrameParse_Init();
    Debug_Init();

    /*-------- 3. 任务 --------*/
    ret=xTaskCreate(UartRx_Task,"uart_rx",UART_RX_TASK_STACK,NULL,UART_RX_TASK_PRIO,&uartRxTaskHandle);
    if(ret!=pdPASS) LOG_E(ret,"xTaskCreate uart_rx");

    ret=xTaskCreate(Ota_Task,"ota",OTA_TASK_STACK,NULL,OTA_TASK_PRIO,&otaTaskHandle);
    if(ret!=pdPASS) LOG_E(ret,"xTaskCreate ota");

    ret=xTaskCreate(Lvgl_Task,"lvgl",LVGL_TASK_STACK,NULL,LVGL_TASK_PRIO,&lvglTaskHandle);
    if(ret!=pdPASS) LOG_E(ret,"xTaskCreate lvgl");

    ret=xTaskCreate(Iwdg_Task,"iwdg",IWDG_TASK_STACK,NULL,IWDG_TASK_PRIO,&iwdgTaskHandle);
    if(ret!=pdPASS) LOG_E(ret,"xTaskCreate iwdg");

    LOG_I("all task created,min heap:%ldB\r\n",(uint32_t)xPortGetFreeHeapSize());
}
