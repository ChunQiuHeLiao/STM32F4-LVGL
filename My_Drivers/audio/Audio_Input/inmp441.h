#ifndef __INMP441_H
#define __INMP441_H
#include"main.h"

void INMP441_Init(uint32_t sampleRate);
uint8_t INMP441_Receive(uint16_t *rxData, uint16_t size);
uint8_t INMP441_WaitRxCplt();
#endif