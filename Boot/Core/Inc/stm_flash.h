#ifndef __STM_FLASH
#define __STM_FLASH
#include"main.h"


// uint8_t STM_FLASH_Erase(uint32_t addr,uint16_t pageNum);
uint8_t STM_FLASH_Erase(uint32_t addr,uint16_t pageNum);
uint8_t STM_FLASH_Write(uint32_t addr, uint8_t* data,uint16_t size);
uint8_t STM_FLASH_Write_NoErase(uint32_t addr, uint8_t* data,uint16_t size);

uint8_t STM_FLASH_Read_Byte(uint32_t addr);
uint16_t STM_FLASH_Read_HalfWord(uint32_t addr);
uint32_t STM_FLASH_Read_Word(uint32_t addr);
void STM_FLASH_ReadData(uint32_t addr,uint8_t* data,uint16_t size);



/********获取内部flash存放MCU信息 函数********/
uint16_t STM_FLASH_Get_ChipFlash();
#endif