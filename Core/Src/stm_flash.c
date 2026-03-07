#include"stm_flash.h"

/* 各个扇区的基地址 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000)
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000)
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000)
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000)
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000)
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000)
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000)
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000)

#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000)
// #define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000)
// #define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000)
// #define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000)

// #define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000)
// #define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000)
// #define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000)
// #define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000)
// #define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000)
// #define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000)
// #define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000)
// #define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000)
// #define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000)
// #define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000)
// #define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000)
// #define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000)


 
/// @brief 获取扇区，对应FLASH_EraseInitTypeDef::Sector
/// @param addr 擦除的地址
/// @return 返回扇区。如果返回0xFFFFFFFF则代表地址不在范围内
static uint32_t Get_Sector(uint32_t addr)
{
    if(addr<ADDR_FLASH_SECTOR_0) return 0xFFFFFFFF;
    else if(addr<ADDR_FLASH_SECTOR_1) return FLASH_SECTOR_0;
    else if(addr<ADDR_FLASH_SECTOR_2) return FLASH_SECTOR_1;
    else if(addr<ADDR_FLASH_SECTOR_3) return FLASH_SECTOR_2;
    else if(addr<ADDR_FLASH_SECTOR_4) return FLASH_SECTOR_3;
    else if(addr<ADDR_FLASH_SECTOR_5) return FLASH_SECTOR_4;
    else if(addr<ADDR_FLASH_SECTOR_6) return FLASH_SECTOR_5;
    else if(addr<ADDR_FLASH_SECTOR_7) return FLASH_SECTOR_6;
    else if(addr<ADDR_FLASH_SECTOR_8) return FLASH_SECTOR_7;
    else return 0xFFFFFFFF;
}

/// @brief 擦除从指定地址开始的连续页数。擦除之前必须解锁
/// @param addr 擦除的起始地址，必须是0x800(2KB)的倍数
/// @param pageNum 擦除的页数
/// @return 返回0擦除成功，其它失败
uint8_t STM_FLASH_Erase(uint32_t addr,uint16_t sectorNum)
{
    uint32_t PageError=0;
    uint32_t sector=Get_Sector(addr);

    FLASH_EraseInitTypeDef pEraseInit={0};
    pEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS; //页写(也就是扇区 2KB); MASS_ERASE 整片擦除，什么都擦
    pEraseInit.Sector=sector; /*擦除的起始扇区*/
    pEraseInit.NbSectors=sectorNum; /*擦除的扇区数量*/
    pEraseInit.Banks=FLASH_BANK_1; /*擦除的扇区在第几块区域*/
    pEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3; /* 电源电压-->  2.7V~3.6V*/

    return HAL_FLASHEx_Erase(&pEraseInit,&PageError);
}


/// @brief 写入半字到内部FLASH。包含擦除
/// @param addr 起始地址，必须是2的倍数
/// @param data 半字数组
/// @param size 大小，建议2024的倍数
/// @return 0：成功
uint8_t STM_FLASH_Write(uint32_t addr, uint8_t* data,uint16_t size)
{
    HAL_StatusTypeDef ret=0; 
    ret=HAL_FLASH_Unlock();
    if(ret!=HAL_OK) return ret;
    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2); // 72 MHz 时至少要 2 wait

    //得到擦除的页数
    uint8_t pageNum=0;
    if(size%2048==0) pageNum=size/2048;
    else  pageNum=size/2048+1;
    
    uint32_t eraseAddr=addr;
    eraseAddr=(eraseAddr/STM_FLASH_PAGE_SIZE)*STM_FLASH_PAGE_SIZE;

    //printf("%#010x\n",eraseAddr); //log

    //开始擦除
    ret=STM_FLASH_Erase(eraseAddr,pageNum);
    if(ret!=HAL_OK) return ret+50;
    
    //printf("erase over\n");
    uint16_t temp=0;

    //开始写入u16数据
    for(uint16_t i=0;i<size;i+=2)
    {
        temp=data[i];
        temp |= ((uint16_t)data[i+1])<<8;
        
        ret=HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addr,temp);
        if(ret!=HAL_OK) 
        {
            printf("i=%d\n",i);
            return ret+100;
        }
        addr+=2;
    }

    HAL_FLASH_Lock();
    //printf("write over\n");
    return 0;
}


uint8_t STM_FLASH_Read_Byte(uint32_t addr)
{
    return *((volatile uint8_t*)addr);
}

uint16_t STM_FLASH_Read_HalfWord(uint32_t addr)
{
    return *((volatile uint16_t*)addr);
}

uint32_t STM_FLASH_Read_Word(uint32_t addr)
{
    return *((volatile uint32_t*)addr);
}

// void STM_FLASH_Read(uint32_t addr,uint8_t* readData,uint16_t size)
// {
//     for(uint16_t i=0;i<size;i++)
//     {
//         STM_FLASH_Read_HalfWord()
//     }
// }




uint8_t STM_FLASH_Get_UpdateFlag()
{
    uint8_t flag=STM_FLASH_Read_Byte(STM_FLASH_INFO_ADDR+1) & 0x01;
    flag=(flag==1)?0:1;
    return flag;
}


/// @brief 
/// @param flag 
/// @return 0成功
uint8_t STM_FLASH_Set_UpdateFlag(uint8_t flag)
{
    flag=(flag==1)?0:1;
    uint16_t temp=STM_FLASH_Read_HalfWord(STM_FLASH_INFO_ADDR);
    uint8_t t1[2]={(uint8_t)temp,0};
    if(flag==1) t1[1]=(temp>>8)|flag;
    else t1[1]=(temp>>8)&0xfe;
    
    return STM_FLASH_Write(STM_FLASH_INFO_ADDR,t1,2);
}


uint16_t STM_FLASH_Get_ChipFlash()
{
   return (*((volatile uint16_t*)(0x1FFFF7E0)));
}