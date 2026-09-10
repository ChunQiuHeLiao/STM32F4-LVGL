#ifndef __UTILITY_H
#define __UTILITY_H
#include"main.h"
#include<stdbool.h>
#include<string.h>

/*搜索 USER_CONFIG  可以看看需要配置什么*/

//启用参数
#define USE_DEBUG       //使用调试信息
#define USE_FreeRTOS    //使用FreeRTOS
#define USE_SAVE_APP    //使用APP的备份(SD卡双固件 A/B 区), 防止升级失败变砖


#define RET_OK 0
#define RET_TIMEOUT -1
#define RET_ERR -2


#ifdef USE_FreeRTOS

#include"FreeRTOS.h"
#include"task.h"
#include"semphr.h"
#include"queue.h"

#define TickToMs(tick)  (tick*1000/configTICK_RATE_HZ) //tick/tickRate*1000
#define TaskGetCurTimeMs()  (TickToMs(xTaskGetTickCount()))   //获取当前的时间(ms)
#define TaskGetCurTimeUs()  (TaskGetCurTimeMs()*1000)   //获取当前的时间(us)
//获取剩余栈大小
#define Task_PrintMinStackSize() printf("%s:%ldW\r\n",pcTaskGetName(NULL),(uxTaskGetStackHighWaterMark(NULL)))
#define Task_PrintMinStackSize2(x) printf("%s:%ldW\r\n",pcTaskGetName(x),(uxTaskGetStackHighWaterMark(x)))
#define Print_MinHeapSize()  printf(" min heap size:%ldB\r\n",(uint32_t)xPortGetMinimumEverFreeHeapSize())

#define Task_PrintStart() printf("%s start\r\n",pcTaskGetName(NULL))
#endif


/*--------------------原有工程使用的类型定义(不要删)--------------------*/

typedef enum __TxStatus
{
    TX_STATUS_FREE=0,
    TX_STATUS_OVER,
    TX_STATUS_BUSY    
}TxStatus;

typedef enum __RxStatus
{
    RX_STATUS_BUSY=0,
    RX_STATUS_OVER,
    RX_STATUS_FREE
}RxStatus;


typedef enum
{
    MODULE_ON=0,
    MODULE_OFF
}MODULE_STATE;


/**************调试用的************/
#define MODULE_NAME_NONE "none"
#define MODULE_NAME_SPI "spi"
#define MODULE_NAME_I2C "i2c"
#define MODULE_NAME_I2S "i2s"


#define Error_GetInfo(moduleName,errCode) Err_GetInfo(moduleName,errCode,__FILE__,__LINE__) /*获取错误信息*/
#define Error_Print() Err_Print(__FILE__,__LINE__) /*打印错误信息*/
#define PrintErr(errCode) PrintError(__FILE__,__LINE__,errCode)

void Err_GetInfo(const char* moduleName,uint8_t errCode,const char* file,uint16_t line);
void Err_Print(const char* file,uint16_t line);
void PrintError(const char* file,uint16_t line,uint8_t errCode);

/*------------------------- */


#define Printf(fmt,...) my_printf(fmt,##__VA_ARGS__)
#define Sprintf(buf,fmt,...) my_sprintf(buf,fmt,##__VA_ARGS__)

void my_printf(const char *fmt, ...);
uint8_t my_sprintf(char* buf, const char* fmt, ...);
char* my_strcat(const char* str1, const char* str2);


#define DELAY_MODE_SYSTICK 0 //使用系统滴答器用于延迟 

//如果用定时器延迟，则需要配置下面三个参数
#define DELAY_MODE_TIMER 1 //使用定时器用于延迟 
#define DELAY_TIMx TIM5
#define DELAY_RCC_APB1ENR_TIMxEN RCC_APB1ENR_TIM5EN  //定时器使能


typedef void (*JumpApp)(void); //用于跳转到App的函数指针

/*注意: FatFs 的 diskio.h 里已经有 RES_OK/RES_ERROR 等枚举,
 * 这里加 UTIL_ 前缀避免重复定义*/
typedef enum
{
    UTIL_RES_OK=0,
    UTIL_RES_ERR=-1,
    UTIL_RES_TIMEOUT=-2,
    UTIL_RES_UNKNOWED=-3
}ResultStatus_t;


void delay_init(uint16_t sysclk);
void delay_us(uint16_t time);
void delay_ms(uint16_t time);


void SYS_SetMSP(uint32_t appBaseAddr);
void SYS_Set_Vector_Table(uint32_t appBaseAddr,uint32_t offset);
uint8_t SYS_JumpApp(uint32_t appBaseAddr);
void Sys_SoftReset();


/*--------------环形缓冲区(带互斥锁, 可用于中断/任务间)-------------*/
//head   tail
//  0    0
//  1    10
/// @brief 环形缓存区 tail指向下一个字节数据位置，head 指向读取头部的数据  
typedef struct  
{
    uint8_t* data; //4字节
    uint16_t size; //当前缓存区最大大小 2
    uint16_t head; //缓存区索引头部 2
    uint16_t tail; // 2
    uint16_t cnt; //当前存的数据大小 2

    #ifdef USE_FreeRTOS
    SemaphoreHandle_t rwMutex; //读写互斥锁
    #endif
    
}RingBuffer_t;

int  RingBuffer_Init(RingBuffer_t* rb,uint16_t size);
int  RingBuffer_Write(RingBuffer_t* rb,uint8_t* data,uint16_t size);
int  RingBuffer_Read(RingBuffer_t* rb,uint8_t* data,uint16_t size);
void RingBuffer_DeInit(RingBuffer_t* rb);
void RingBuffer_Clear(RingBuffer_t* rb);


/*--------------CRC 校验-----------------*/
typedef struct {
    unsigned long crc; //初始值
} CRC32_CTX;

void CRC32_Init(CRC32_CTX *ctx);
void CRC32_Update(CRC32_CTX *ctx, const unsigned char *data, size_t len);
void CRC32_Final(CRC32_CTX *ctx, uint32_t *finalCrcVal);
uint32_t CRC32_Calculate(uint8_t* data,uint32_t size);

uint16_t CRC16_Calculate(uint8_t *ptr, uint32_t len);


/*--------------------LOG------------------------*/
/*注意: 变参必须放在 err 之前, 否则 fmt 里的 %d/%s 会错位*/
#define LOG_I(fmt,...)  printf("[%s:%d] "fmt,__FILE_NAME__, __LINE__,##__VA_ARGS__)
#define LOG_E(err,fmt,...)  printf("[%s:%d] "fmt" fail:%d\r\n",__FILE_NAME__, __LINE__,##__VA_ARGS__,(int)(err))
#define LOG_E2(err,fmt,...) printf("[%s:%d] err=%d "fmt,__FILE_NAME__, __LINE__,(int)(err),##__VA_ARGS__)

#ifdef USE_DEBUG

#define DEBUG_I(fmt,...)  printf("[%s:%d] "fmt,__FILE_NAME__, __LINE__,##__VA_ARGS__)
#define DEBUG_E(err,fmt,...)  printf("[%s:%d] "fmt" fail:%d\r\n",__FILE_NAME__, __LINE__,err,##__VA_ARGS__)
#define DEBUG_E2(err,fmt,...)  printf("[%s:%d] err=%d "fmt,__FILE_NAME__, __LINE__,err,##__VA_ARGS__)

#endif

#endif
