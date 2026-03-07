#ifndef __MAX98357_H
#define __MAX98357_H
#include"main.h"

void MAX98357_Init(uint32_t sampleRate);
uint8_t MAX98357_Transmit(uint16_t *rxData, uint16_t size);
uint8_t MAX98357_WaitTxCplt();
#endif