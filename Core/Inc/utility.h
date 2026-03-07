#ifndef __UTILITY_H
#define __UTILITY_H
#include"main.h"
#include"string.h"

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


#define DELAY_MODE_SYSTICK 0 //使用系统滴答器用于延迟 

#define Printf(fmt,...) my_printf(fmt,##__VA_ARGS__)
#define Sprintf(buf,fmt,...) my_sprintf(buf,fmt,##__VA_ARGS__)


//如果用定时器延迟，则需要配置下面三个参数
#define DELAY_MODE_TIMER 1 //使用定时器用于延迟 
#define DELAY_TIMx TIM5
#define DELAY_RCC_APB1ENR_TIMxEN RCC_APB1ENR_TIM5EN  //定时器使能


typedef void (*JumpApp)(void); //用于跳转到App的函数指针

void my_printf(const char *fmt, ...);
uint8_t my_sprintf(char* buf, const char* fmt, ...);
char* my_strcat(const char* str1, const char* str2);

void delay_init(uint16_t sysclk);
void delay_us(uint16_t time);
void delay_ms(uint16_t time);


void SYS_SetMSP(uint32_t appBaseAddr);
void SYS_Set_Vector_Table(uint32_t appBaseAddr,uint32_t offset);
uint8_t SYS_JumpApp(uint32_t appBaseAddr);
#endif