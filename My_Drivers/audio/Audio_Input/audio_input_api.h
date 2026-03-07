/*
该文件需要自己将底层驱动传入.c文件的函数中
*/

#ifndef __AUDIO_INPUT_API_H
#define __AUDIO_INPUT_API_H
#include"main.h"

void Audio_Input_Init(uint32_t sampleRate);
uint8_t Audio_Input_Receive(uint16_t *rxData, uint16_t size);
uint8_t Audio_Input_WaitRxCplt();

void Audio_Input_Enable();
#endif