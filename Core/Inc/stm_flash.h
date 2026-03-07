#ifndef __STM_FLASH
#define __STM_FLASH
#include"main.h"


/***************下面对的所以程序都是对大容量的32*****************/

//max 0x0808 0000 -1
#define STM_FLASH_APP_OFSET_ADDR 0xC000 //APP程序起始地址相对于单片机起始地址的偏移量
#define STM_FLASH_APP_ADDR (FLASH_BASE+STM_FLASH_APP_OFSET_ADDR) //BootLoader 48KB 0x0800 8000

#define STM_FLASH_PAGE_SIZE (1024*2)  //一般修改这
#define STM_FLASH_MAX_PAGE 255 //0~255 改这
#define STM_FLASH_APP_INIT_PAGE (0xC000/STM_FLASH_PAGE_SIZE)



/***********存放 单片机信息 的起始地址 共128字节
    共2字节
    1.开始两个字节用于存储版本号和更新标志。
2字节：4位大版本号+4位小版本号+7位预留+1位更新标志


*/


#define STM_FLASH_INFO_ADDR (0x08000000+(0x800)*239+1920) //第239页，的1920字节开始，本页可用128字节




// uint8_t STM_FLASH_Erase(uint32_t addr,uint16_t pageNum);
uint8_t STM_FLASH_Write(uint32_t addr, uint8_t* data,uint16_t size);

uint8_t STM_FLASH_Read_Byte(uint32_t addr);
uint16_t STM_FLASH_Read_HalfWord(uint32_t addr);
uint32_t STM_FLASH_Read_Word(uint32_t addr);


/********获取内部flash存放MCU信息 函数********/

uint8_t STM_FLASH_Get_UpdateFlag();
uint8_t STM_FLASH_Set_UpdateFlag(uint8_t flag);

uint16_t STM_FLASH_Get_ChipFlash();
#endif