#ifndef __UTILITY_H
#define __UTILITY_H
#include"main.h"
#include<stdbool.h>
#include<string.h>


/*搜索 USER_CONFIG  可以看看需要配置什么*/

//启用参数
#define USE_DEBUG //使用调试信息
// #define USE_FreeRTOS   //使用FreeRTOS
#define USE_SAVE_APP //使用APP的备份，和APP一样的程序，注意防止升级失败变砖。也可用OTA时存下载的升级固件
#define USE_BOOT //当前这个程序是boot



#define RET_OK 0
#define RET_TIMEOUT -1
#define RET_ERR -2



#ifdef USE_FreeRTOS

#include"FreeRTOS.h"
#include"task.h"
#include"semphr.h"

//获取剩余栈大小
#define Task_PrintMinStackSize() printf("%s:%ldW\r\n",pcTaskGetName(NULL),(uxTaskGetStackHighWaterMark(NULL)))
#define Task_PrintMinStackSize2(x) printf("%s:%ldW\r\n",pcTaskGetName(x),(uxTaskGetStackHighWaterMark(x)))
#define Print_MinHeapSize()  printf("min heap size:%ldB\r\n",(uint32_t)xPortGetMinimumEverFreeHeapSize())
#define Task_PrintStart() printf("%s start\r\n",pcTaskGetName(NULL))
#endif



#define LOG_I(fmt,...)  printf("[%s:%d] "fmt,__FILE__, __LINE__,##__VA_ARGS__)
#define LOG_E(err,fmt,...)  printf("[%s:%d] "fmt" fail:%d\r\n",__FILE__, __LINE__,err,##__VA_ARGS__)

#ifdef USE_DEBUG

#define DEBUG_I(fmt,...)  printf("[%s:%d] "fmt,__FILE_NAME__, __LINE__,##__VA_ARGS__)
#define DEBUG_E(err,fmt,...)  printf("[%s:%d] "fmt" fail:%d\r\n",__FILE_NAME__, __LINE__,err,##__VA_ARGS__)
#define DEBUG_E2(err,fmt,...)  printf("[%s:%d] err=%d "fmt,__FILE_NAME__, __LINE__,err,##__VA_ARGS__)

#else 

#define LOG_I(fmt,...) ((void)0)
#define LOG_E(fmt,...) ((void)0)

#endif


/* USER_CONFIG 配置芯片信息*/
//--------------------系统信息  包含boot app信息--------------

#define CHIP_NAME "STM32F103C8T6" //芯片名
#define CHIP_RAM_SIZE (20) //芯片RAM KB
#define CHIP_FLASH_SIZE (64) //芯片FLASH

#define CHIP_FLASH_PAGE_SIZE (1024) //芯片的Flash一页1KB


// #define HAL_GetRamSize() ((uint32_t)(&_estack-&_sdata)/1024)  //获取RAM大小 KB
// #define HAL_GetFlashSize() ((uint32_t)LL_GetFlashSize()) //获取FLASH 大小 KB
#define Print_ChipInfo() printf("chip:%s,ram:%ldKB,flash:%ldKB\r\n",CHIP_NAME,CHIP_RAM_SIZE,CHIP_FLASH_SIZE)


//FLASH分为 (BOOT区+系统配置区)+APP区
#define FLASH_START_ADDR (0x8000000) //FLASH起始地址
#define BOOT_SIZE (0x3400)  //boot占用大小 13KB -->52 / 26


#define APP_START_ADDR  (FLASH_START_ADDR+BOOT_SIZE)  //APP起始地址

#ifdef USE_SAVE_APP
#define APP_SIZE (0x7000) //app的大小

#define SAVE_APP_START_ADDR (APP_START_ADDR+APP_SIZE) //备份APP起始地址

#define APP_SAVE_SIZE (CHIP_FLASH_SIZE*1024-BOOT_SIZE-APP_SIZE) //备份APP大小，用于存放APP的备份
// #if (APP_SAVE_SIZE<APP_SIZE)
// #error save app size not enough, APP_SAVE_SIZE< APP_SIZE
// #endif



/*--------这部分存放 系统配置的信息 (共占用 256字节)---------*/
/*
配置大概如下面的结构体，会把整个结构体数据存入FLASH的，可以把整个结构体读出来然后得到数据也是可以的
*/

#define SYS_CFG_SIZE (0x50) //系统配置区大小
#define SYS_CFG_START_ADDR (FLASH_START_ADDR+BOOT_SIZE-SYS_CFG_SIZE) //系统配置起始区


//系统配置结构体
#define FACTOR_FIRMWARE_SIZE (27992) //出厂固件的大小
#define FACTOR_FIRMWARE_CRC32 (0x123456) //出厂固件的CRC32校验码


typedef struct 
{
    uint32_t size; //固件大小
    uint32_t crc32; //CRC32校验码
}FirewareInfo_t;

typedef struct 
{
    FirewareInfo_t factoryFirmware; //出厂的固件。这个主要是为了 如果更新挂了，还可以使用备份的程序
    FirewareInfo_t updateFireware; //更新得到的那个固件

    uint8_t isUpdate; //本次是否需要更新
    uint8_t factoryFlag; //1:代表是出厂标志，0则不是
    uint8_t reverser1;
    uint8_t reverser2;
}SysConfigInfo_t;

 

//每次boot 会读取这些信息，如果一开始出厂固件数据大小读取为0，则会配置 FACTOR_FIRMWARE_SIZE 和 FACTOR_FIRMWARE_CRC32
extern SysConfigInfo_t sysConfInfo; 


int SysConfInfo_Get(); 
int SysConfInfo_Set();


#define CHIP_FLASH_MAX_ADDR (FLASH_START_ADDR+CHIP_FLASH_SIZE*1024) //芯片Flash最大地址 


#endif

/*--------------------系统配置结束------------------------*/





#define DELAY_MODE_SYSTICK 0 //使用系统滴答器用于延迟 

//如果用定时器延迟，则需要配置下面三个参数
#define DELAY_MODE_TIMER 1 //使用定时器用于延迟 
#define DELAY_TIMx TIM4
#define DELAY_RCC_APB1ENR_TIMxEN RCC_APB1ENR_TIM4EN  //定时器使能





typedef void (*JumpApp)(void); //用于跳转到App的函数指针



typedef enum
{
    RES_OK=0,
    RES_ERR=-1,
    RES_TIMEOUT=-2,
    RES_UNKNOWED=-3
}ResultStatus_t;

/*--------------环形缓冲区-------------*/
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

int RingBuffer_Init(RingBuffer_t* rb,uint16_t size);
int RingBuffer_Write(RingBuffer_t* rb,uint8_t* data,uint16_t size);
int RingBuffer_Read(RingBuffer_t* rb,uint8_t* data,uint16_t size);
int RingBuffer_Peak(RingBuffer_t* rb,uint8_t* data,uint16_t size);
void RingBuffer_DeInit(RingBuffer_t* rb);



void delay_init(uint16_t sysclk);
void delay_us(uint16_t time);
void delay_ms(uint16_t time);


void SYS_SetMSP(uint32_t appBaseAddr);
void SYS_Set_Vector_Table(uint32_t appBaseAddr,uint32_t offset);
uint8_t SYS_JumpApp(uint32_t appBaseAddr);
void Sys_SoftReset();




/*--------------CRC 校验-----------------*/
typedef struct {
    unsigned long crc; //初始值
} CRC32_CTX;

void CRC32_Init(CRC32_CTX *ctx);
void CRC32_Update(CRC32_CTX *ctx, const unsigned char *data, size_t len);
void CRC32_Final(CRC32_CTX *ctx, uint32_t *finalCrcVal);
uint32_t CRC32_Calculate(uint8_t* data,uint16_t size);

uint16_t CRC16_Calculate(uint8_t *ptr, uint32_t len);


#endif