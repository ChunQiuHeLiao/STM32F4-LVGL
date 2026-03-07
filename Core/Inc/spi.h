#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


/*************************使用别的SPI需要更改的参数*******************/
/*******使用哪个SPI*******/
#define SPI_EN_SPI1
#define SPI_EN_SPI2

/********使用哪个DMA******/
#define SPI_EN_DMA_SPI1
// #define SPI_EN_DMA_SPI2






/********************SPI1******************/
#ifdef SPI_EN_SPI1

#define SPIx SPI1  //使用SPI几

/*********时钟使能定义********/
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
// #define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE() 


/***引脚和端口定义****/
//CS A4 SCK A5，MISO A6， MOSI A7，  
#define SPIx_GPIO_AF  GPIO_AF5_SPI1

#define SPIx_SCK_PIN                     GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT               GPIOA

// #define SPIx_MISO_PIN                    GPIO_PIN_6
// #define SPIx_MISO_GPIO_PORT              GPIOA

#define SPIx_MOSI_PIN                    GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT              GPIOA
//野火的这个开发板固定死了CS就是PC0，其它都不行，包括PA4

#define SPIx_CS_PIN                     GPIO_PIN_4        
#define SPIx_CS_GPIO_PORT               GPIOA


#endif



/****************************SPI2******************************/

#ifdef SPI_EN_SPI2

#define SPIx2 SPI2  //使用SPI几

/*********时钟使能定义********/
#define SPIx2_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()

#define SPIx2_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx2_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE() 
#define SPIx2_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE() 
#define SPIx2_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE() 


/***引脚和端口定义****/
//SCK A5， MOSI A7， MISO A6， CS C0
#define SPIx2_GPIO_AF  GPIO_AF5_SPI2

#define SPIx2_SCK_PIN                     GPIO_PIN_10
#define SPIx2_SCK_GPIO_PORT               GPIOB

#define SPIx2_MISO_PIN                    GPIO_PIN_2
#define SPIx2_MISO_GPIO_PORT              GPIOC

#define SPIx2_MOSI_PIN                    GPIO_PIN_3
#define SPIx2_MOSI_GPIO_PORT              GPIOC
//野火的这个开发板固定死了CS就是PC0，其它都不行，包括PA4

#define SPIx2_CS_PIN                     GPIO_PIN_2          
#define SPIx2_CS_GPIO_PORT               GPIOB 


#endif






/********************SPI更改参数结束******************/



#ifdef SPI_EN_SPI1
void SPI_Init(void);
#endif

#ifdef SPI_EN_SPI2
void SPI2_Init(void);
#endif


uint8_t SPI_SendData(uint8_t* data,uint16_t size,uint8_t spiId);
uint8_t SPI_SendData_DMA(uint8_t* data,uint16_t size,uint8_t spiId);

uint8_t SPI_RevData(uint8_t* data,uint16_t size,uint8_t spiId);


uint8_t SPI_TransmitRev(uint8_t* sendData,uint8_t* revData,uint16_t size);

uint8_t SPI_DMA_GetRxFlag(uint8_t spiId);
void SPI_DMA_ClearRxFlag(uint8_t spiId);

void SPI_ON_OR_OFF(uint8_t spiId,uint8_t state);


#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

