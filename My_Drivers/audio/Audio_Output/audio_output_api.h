/*
该文件需要自己将底层驱动传入.c文件的函数中
*/

#ifndef __AUDIO_OUTPUT_API_H
#define __AUDIO_OUTPUT_API_H
#include"main.h"

void Audio_Output_Init(uint32_t sampleRate);
uint8_t Audio_Output_Transmit(uint16_t *rxData, uint16_t size);
void Audio_Output_Enable();

uint8_t Audio_Output_WaitTxCplt();
#endif