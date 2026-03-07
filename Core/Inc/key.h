#ifndef __KEY_H
#define __KEY_H
#include"main.h"

#define KEY_ON  1
#define KEY_OFF 0

//初始化
void KEY_Init();

uint8_t KEY_Scan(GPIO_TypeDef* GPIOx,uint8_t GPIO_Pin);

uint8_t KEY_GetFlag(uint8_t keyVal);
void KEY_ClearFlag(uint_least8_t keyVal);
#endif