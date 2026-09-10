#include"app_frame_parse.h"
#include"transfer_protocol.h"
#include"app_update.h"
#include"ota_ab.h"
#include"debug.h"


RingBuffer_t uartRingBuf={0};
DataFrame_t uartDf={0};

static AppFrameMsg_t frameMsg;


/// @brief 协议解析任务: 从环形缓冲区解帧, 通过消息队列发给 OTA 任务
static void UART_FrameParse_Task(void* args)
{
    Task_PrintStart();

    int ret=RingBuffer_Init(&uartRingBuf,1024);
    if(ret!=RET_OK)
    {
        LOG_E(ret,"RingBuffer_Init");
        vTaskDelete(NULL);
    }

    while(1)
    {
        /* 等接收任务通知(信号量同步) */
        xSemaphoreTake(g_frameSem,portMAX_DELAY);

        while(uartRingBuf.cnt >= sizeof(FrameHead_t))
        {
            /*解析一帧, 内部已做 魔术头/头CRC/体CRC 校验*/
            if(DataFrame_Handler(&uartRingBuf,&uartDf)!=RET_OK) break;

            /*只把必要的部分放进队列, 减小队列内存占用*/
            memset(&frameMsg,0,sizeof(AppFrameMsg_t));
            frameMsg.funcCode = uartDf.body.funcCode;
            frameMsg.len      = uartDf.head.len;
            if(frameMsg.len>DATA_PACKET_MAX_SIZE) frameMsg.len=DATA_PACKET_MAX_SIZE;
            memcpy(frameMsg.data,uartDf.body.data,frameMsg.len);

            if(xQueueSend(g_frameQueue,&frameMsg,pdMS_TO_TICKS(1000))!=pdPASS)
            {
                LOG_E(-1,"frame queue full\r\n");
            }
        }
    }
}


void AppFrameParse_Init()
{
    BaseType_t ret=xTaskCreate(UART_FrameParse_Task,"frame_parse",(160),NULL,4,&frameParseTaskHandle);
    if(ret==pdFALSE)
    {
        LOG_E(ret,"xTaskCreate");
    }
}
