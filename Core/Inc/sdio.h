#ifndef __SDIO_H__
#define __SDIO_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"




typedef struct _SD_CardInfo
{
  float capacityGB; //卡容量(GB单位)
  float capacityMB; //卡容量(MB单位)
  float capacityKB; //卡容量(KB单位)
  float usedCapacityMB; //已使用的卡容量
  float unusedCapacityMB; //未使用的
  uint8_t cardType[16]; //卡的类型，SDSC,SDHC还是别的
  uint32_t blockSize;
  uint32_t blockNum;
}SD_CardInfoHandle;


/**SDIO GPIO Configuration
PC8     ------> SDIO_D0
PC9     ------> SDIO_D1
PC10     ------> SDIO_D2
PC11     ------> SDIO_D3
PC12     ------> SDIO_CK
PD2     ------> SDIO_CMD
*/

// #define SDIOx_EN_DMA

#define SDIOx SDIO


/***************引脚时钟和IO口配置*********************/
#define SDx_GPIO_AF GPIO_AF12_SDIO

#define SDIOx_CLK_ENABLE()  __HAL_RCC_SDIO_CLK_ENABLE();
#define SDIOx_CLK_DISABLE()  __HAL_RCC_SDIO_CLK_DISABLE();

#define SDx_CK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE();
#define SDx_CK_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE();
#define SDx_CK_GPIO_PORT GPIOB
#define SDx_CK_GPIO_PIN GPIO_PIN_15

#define SDx_CMD_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE();
#define SDx_CMD_GPIO_CLK_DISABLE() __HAL_RCC_GPIOD_CLK_DISABLE();
#define SDx_CMD_GPIO_PORT GPIOD
#define SDx_CMD_GPIO_PIN GPIO_PIN_2

#define SDx_D0_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE();
#define SDx_D0_GPIO_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE();
#define SDx_D0_GPIO_PORT GPIOC
#define SDx_D0_GPIO_PIN GPIO_PIN_8

#define SDx_D1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE();
#define SDx_D1_GPIO_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE();
#define SDx_D1_GPIO_PORT GPIOC
#define SDx_D1_GPIO_PIN GPIO_PIN_9

#define SDx_D2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE();
#define SDx_D2_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE();
#define SDx_D2_GPIO_PORT GPIOA
#define SDx_D2_GPIO_PIN GPIO_PIN_9

#define SDx_D3_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE();
#define SDx_D3_GPIO_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE();
#define SDx_D3_GPIO_PORT GPIOC
#define SDx_D3_GPIO_PIN GPIO_PIN_11




uint8_t SD_Init();
uint8_t SD_DeInit();
uint8_t SD_ReadBlocks(uint8_t *readData, uint32_t blockAddr, uint32_t blockNum);
uint8_t SD_WrietBlocks(uint8_t *pData, uint32_t blockAddr, uint32_t blockNum);


#ifdef SDIOx_EN_DMA
void SD_DMA_Init();
uint8_t SD_ReadBlocks_DMA(uint8_t *pData, uint32_t blockAddr, uint32_t blockNum);
uint8_t SD_DMA_GetRxFlag();
#endif

uint8_t SD_GetFreeState();


uint8_t SD_EraseBlocks(uint32_t blockStartAddr, uint32_t blockEndAddr);
uint8_t SD_EraseBlocks_Num(uint32_t blockStartAddr, uint32_t blockNum);
uint8_t SD_EraseAllBlocks();


SD_CardInfoHandle* SD_GetSDInfoHandle();
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef* carInfo);
uint8_t SD_GetCardCID(HAL_SD_CardCIDTypeDef *pCID);
void SD_GetCardMainInfo(SD_CardInfoHandle* cardInfoHandle);
void SD_ShowCardInfo();
#ifdef __cplusplus
}
#endif

#endif /* __SDIO_H__ */

