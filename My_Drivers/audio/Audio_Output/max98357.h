#ifndef __MAX98357_H
#define __MAX98357_H
#include"main.h"



/*对应MAX98357的SD引脚，使能引脚，高电平芯片使能*/
#define MAX98357_EN_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define MAX98357_EN_GPIO_PORT GPIOA
#define MAX98357_EN_GPIO_PIN GPIO_PIN_8


void MAX98357_Init(uint32_t sampleRate);
uint8_t MAX98357_Transmit(uint16_t *rxData, uint16_t size);
uint8_t MAX98357_WaitTxCplt();

void MAX98357_SetState(MODULE_STATE moduleState);
void MAX98357_SetVolume(uint16_t volume);
#endif