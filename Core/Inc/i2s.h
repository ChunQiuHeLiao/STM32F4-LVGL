#ifndef __I2S_H
#define __I2S_H
#include"main.h"


/**I2Sx
 * I2S4 GPIO Configuration
PB12     ------> I2S4_WS
PB13     ------> I2S4_CK
PA1    ------> I2S4_SD
*/
#define I2Sx_MODE 2
#define I2Sx SPI4
#define I2Sx_CLK_ENABLE()  __HAL_RCC_SPI4_CLK_ENABLE();

#define I2Sx_WS_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Sx_WS_GPIO_PORT GPIOB
#define I2Sx_WS_GPIO_PIN GPIO_PIN_12
#define I2Sx_WS_GPIO_AF GPIO_AF6_SPI4

#define I2Sx_CK_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Sx_CK_GPIO_PORT GPIOB
#define I2Sx_CK_GPIO_PIN GPIO_PIN_13
#define I2Sx_CK_GPIO_AF GPIO_AF6_SPI4

#define I2Sx_SD_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define I2Sx_SD_GPIO_PORT GPIOA
#define I2Sx_SD_GPIO_PIN GPIO_PIN_1
#define I2Sx_SD_GPIO_AF GPIO_AF5_SPI4

#define I2Sx_IRQn SPI4_IRQn
#define I2Sx_IRQHandler SPI4_IRQHandler

/*DMA部分*/
#define I2Sx_DMA_TX
//#define I2Sx_DMA_RX
#define I2Sx_DMA_CLK_ENABLE() __HAL_RCC_DMA2_CLK_ENABLE()
#define I2Sx_DMA_Stream DMA2_Stream4
#define I2Sx_DMA_Stream_IRQn DMA2_Stream4_IRQn
#define I2Sx_DMA_CHANNEL DMA_CHANNEL_4
#define I2Sx_DMA_IRQHandler  DMA2_Stream4_IRQHandler

/**I2Sx2
 * I2S3 GPIO Configuration
PA15    ------> I2S3_WS
PB3     ------> I2S3_CK
PB5     ------> I2S3_SD
*/
#define I2Sx2_MODE 2
#define I2Sx2 SPI3
#define I2Sx2_CLK_ENABLE()  __HAL_RCC_SPI3_CLK_ENABLE();

#define I2Sx2_WS_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define I2Sx2_WS_GPIO_PORT GPIOA
#define I2Sx2_WS_GPIO_PIN GPIO_PIN_15
#define I2Sx2_WS_GPIO_AF GPIO_AF6_SPI3

#define I2Sx2_CK_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Sx2_CK_GPIO_PORT GPIOB
#define I2Sx2_CK_GPIO_PIN GPIO_PIN_3
#define I2Sx2_CK_GPIO_AF GPIO_AF6_SPI3

#define I2Sx2_SD_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Sx2_SD_GPIO_PORT GPIOB
#define I2Sx2_SD_GPIO_PIN GPIO_PIN_5
#define I2Sx2_SD_GPIO_AF GPIO_AF6_SPI3

#define I2Sx2_IRQn SPI3_IRQn
#define I2Sx2_IRQHandler SPI3_IRQHandler


/*DMA部分*/
#define I2Sx2_DMA_TX
//#define I2Sx2_DMA_RX
#define I2Sx2_DMA_CLK_ENABLE() __HAL_RCC_DMA1_CLK_ENABLE()
#define I2Sx2_DMA_Stream DMA1_Stream5
#define I2Sx2_DMA_Stream_IRQn DMA1_Stream5_IRQn
#define I2Sx2_DMA_CHANNEL DMA_CHANNEL_0
#define I2Sx2_DMA_IRQHandler  DMA1_Stream5_IRQHandler


typedef enum 
{
  I2S1_ID=1,
  I2S2_ID,
  I2S3_ID
}I2S_ID;


void I2S_Init(uint32_t sampleRate);
void I2S2_Init(uint32_t sampleRate);

uint8_t I2S_Transmit(uint16_t *data, uint16_t size,I2S_ID i2sId);
uint8_t I2S_Receive(uint16_t *rxData, uint16_t size,I2S_ID i2sId);
void I2S_Stop();

uint8_t I2S_WaitTxCplt(I2S_ID i2sId);
uint8_t I2S_WaitRxCplt(I2S_ID i2sId);

uint8_t I2S_Transmit_IT(uint16_t *rxData, uint16_t size,I2S_ID i2sId);

uint8_t I2S_Transmit_DMA(uint16_t *txData, uint16_t size,I2S_ID i2sId);
uint8_t I2S_Receive_DMA(uint16_t *rxData, uint16_t size,I2S_ID i2sId);
#endif