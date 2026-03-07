#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


#define EN_PRINTF

#ifdef EN_PRINTF
#include"stdio.h"
#endif

/******************* 串口使用 ***********/


#define EN_USART2
// #define EN_USART3 
// #define EN_UART4
// #define EN_UART5




/******************修改串口接收的最多数据*******************/
#define UART1_RX_DATA_SIZE 64
#define UART2_RX_DATA_SIZE 2048
#define UART3_RX_DATA_SIZE 32 //推荐1600
#define UART4_RX_DATA_SIZE 0
#define UART5_RX_DATA_SIZE 0


/***********GPIO口时钟和USART外设时钟使能*************/


#define USARTx USART1
#define USARTx_CLK_ENABLE()  __HAL_RCC_USART1_CLK_ENABLE()
#define USARTx_TX_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE() //TX引脚
#define USARTx_RX_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE() //RX引脚

#define USARTx_TX_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE() //TX引脚
#define USARTx_RX_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE() //RX引脚


#define USARTx_GPIO_AF GPIO_AF7_USART1
#define USARTx_TX_PORT GPIOA
#define USARTx_RX_PORT GPIOA
#define USARTx_TX_PIN GPIO_PIN_9
#define USARTx_RX_PIN GPIO_PIN_10
#define USARTx_IRQn USART1_IRQn




#ifdef  EN_USART2

#define USARTx2 USART2 
#define USARTx2_CLK_ENABLE()  __HAL_RCC_USART2_CLK_ENABLE()
#define USARTx2_TX_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE() //TX引脚
#define USARTx2_RX_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE() //RX引脚

#define USARTx2_TX_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE() //TX引脚
#define USARTx2_RX_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE() //RX引脚

#define USARTx2_GPIO_AF GPIO_AF7_USART2
#define USARTx2_TX_PORT GPIOA
#define USARTx2_RX_PORT GPIOA
#define USARTx2_TX_PIN GPIO_PIN_2
#define USARTx2_RX_PIN GPIO_PIN_3

#define USARTx2_IRQn USART2_IRQn


#endif



/****************USART3*******************/

#ifdef EN_USART3


#define USARTx3 USART3                 
#define USARTx3_CLK_ENABLE()   __HAL_RCC_USART3_CLK_ENABLE()
// #define USARTx3_REMAP_CLK_ENABLE()   __HAL_AFIO_REMAP_USART3_ENABLE() //完全重映射引脚
#define USARTx3_TX_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE() //TX引脚
#define USARTx3_RX_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE() //RX引脚

#define USARTx3_TX_PORT GPIOB
#define USARTx3_RX_PORT GPIOB
#define USARTx3_TX_PIN GPIO_PIN_10
#define USARTx3_RX_PIN GPIO_PIN_11

#define USARTx3_IRQn USART3_IRQn

#endif




/*************************************UART4**********************/
#ifdef EN_UART4


#define UARTx4 UART4                 
#define UARTx4_CLK_ENABLE()   __HAL_RCC_UART4_CLK_ENABLE()
// #define UARTx4_REMAP_CLK_ENABLE()   __HAL_AFIO_REMAP_USART3_ENABLE() //完全重映射引脚
#define UARTx4_TX_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE() //TX引脚
#define UARTx4_RX_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE() //RX引脚

#define UARTx4_TX_PORT GPIOC
#define UARTx4_RX_PORT GPIOC
#define UARTx4_TX_PIN GPIO_PIN_10
#define UARTx4_RX_PIN GPIO_PIN_11

#define UARTx4_IRQn UART4_IRQn

#endif
/*************************************UART5**********************/

#ifdef  EN_UART5

#define UARTx5 UART5                 
#define UARTx5_CLK_ENABLE()   __HAL_RCC_UART5_CLK_ENABLE()
// #define UARTx5_REMAP_CLK_ENABLE()   __HAL_AFIO_REMAP_USART3_ENABLE() //完全重映射引脚
#define UARTx5_TX_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE() //TX引脚
#define UARTx5_RX_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE() //RX引脚

#define UARTx5_TX_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE() //TX引脚
#define UARTx5_RX_CLK_DISABLE() __HAL_RCC_GPIOD_CLK_DISABLE() //RX引脚


#define UARTx5_TX_PORT GPIOC
#define UARTx5_RX_PORT GPIOD

#define UARTx5_TX_PIN GPIO_PIN_12
#define UARTx5_RX_PIN GPIO_PIN_2

#define UARTx5_IRQn UART5_IRQn

#endif








/*************USART参数修改结束***********/
uint16_t UART_GetRxDataSize(uint8_t uartId);
uint8_t UART_GetRxData(char* rxData,uint16_t size,uint8_t usartId);
void UART_ClearRxFlag(uint8_t usartRank);
uint8_t UART_GetRxDataPointer(char** rxData,uint8_t usartId);

uint8_t UART_Transmit_AllString(uint8_t* str,uint8_t uartId);
uint8_t UART_Transmit_String(uint8_t* str,uint16_t size,uint8_t uartId);
uint8_t UART_Transmit_String_IT( uint8_t* str,uint8_t size,uint8_t uartId);

uint8_t UART_Receive_String_IT(uint8_t* rxBuf,uint16_t size,uint8_t uartId);
uint8_t UART_ReceiveToIdle_IT(uint8_t* rxBuf,uint16_t size,uint8_t uartId);



void UART_Init(uint32_t baudRate);

#ifdef  EN_USART2
void UART2_Init(uint32_t baudRate);
#endif


#ifdef  EN_USART3
void UART3_Init(uint32_t baudRate);
#endif


#ifdef EN_UART4
void UART4_Init(uint32_t baudRate);
#endif


#ifdef EN_UART5
void UART5_Init(uint32_t baudRate);
#endif


#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

