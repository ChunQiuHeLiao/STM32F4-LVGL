#include"stm_flash.h"


static uint8_t* readDataBuf=NULL; //擦除一页时，如果地址在一页中，那就需要先读取这一页的数据，存起来，擦除，再写进去

/// @brief 擦除从指定地址开始的连续页数。擦除之前必须解锁
/// @param addr 擦除的起始地址，必须是0x400(1KB)的倍数，具体芯片具体页大小
/// @param pageNum 擦除的页数
/// @return 返回0擦除成功，其它失败
uint8_t STM_FLASH_Erase(uint32_t addr,uint16_t pageNum)
{
    HAL_StatusTypeDef ret=HAL_FLASH_Unlock();
    if(ret!=HAL_OK)
    {
        LOG_E(ret,"HAL_FLASH_Unlock");
        return ret;
    }  

    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2); // 72 MHz 时至少要 2 wait
    
    uint32_t PageError=0;
    FLASH_EraseInitTypeDef pEraseInit={0};
    pEraseInit.TypeErase=FLASH_TYPEERASE_PAGES; //页写(也就是扇区 2KB);
    pEraseInit.PageAddress=addr;
    pEraseInit.NbPages=pageNum;
    pEraseInit.Banks=FLASH_BANK_1;

    ret=HAL_FLASHEx_Erase(&pEraseInit,&PageError);
    if(ret!=HAL_OK)
    {
        LOG_E(ret,"HAL_FLASHEx_Erase");
        HAL_FLASH_Lock();
        return ret;
    }

    HAL_FLASH_Lock();

    return 0;
}


/// @brief 写入半字到内部FLASH。包含擦除
/// @param addr 起始地址，必须是2的倍数
/// @param data 半字数组
/// @param size 大小，是页 CHIP_FLASH_PAGE_SIZE 的倍数
/// @return 0：成功
uint8_t STM_FLASH_Write(uint32_t addr, uint8_t* data,uint16_t size)
{
   // if(addr<FLASH_START_ADDR || addr>CHIP_FLASH_MAX_ADDR) return HAL_ERROR;

    uint8_t isNeedReadData=0; //是否需要读取数据

    HAL_StatusTypeDef ret=0; 



    //得到擦除的页数
    uint8_t pageNum=0;
    if(size%CHIP_FLASH_PAGE_SIZE==0) pageNum=size/CHIP_FLASH_PAGE_SIZE;
    else  pageNum=size/CHIP_FLASH_PAGE_SIZE+1; 
    
    uint32_t eraseAddr=(addr-FLASH_START_ADDR)/CHIP_FLASH_PAGE_SIZE*CHIP_FLASH_PAGE_SIZE+FLASH_START_ADDR; //0x3000

    //先判断地址是不是页的倍数
    if( ((addr-FLASH_START_ADDR)%CHIP_FLASH_PAGE_SIZE)!=0) //不是一页
    {
        #ifdef USE_FreeRTOS
        readDataBuf=pvPortMalloc(CHIP_FLASH_PAGE_SIZE);
        #else
        readDataBuf=(uint8_t*)malloc(CHIP_FLASH_PAGE_SIZE);
        #endif

        if(readDataBuf==NULL)
        {
            DEBUG_E(-1,"malloc");
            //HAL_FLASH_Lock();
            return 1;
        }


        //开始读取数据
        STM_FLASH_ReadData(eraseAddr,readDataBuf,CHIP_FLASH_PAGE_SIZE); //addr=0x33B0  eraseAddr=0x3000  size=20

        // for(uint16_t i=0;i<FLASH_PAGE_SIZE;i++)
        // {
        //     printf("%02x ",readDataBuf[i]);
        // }
        // printf("\r\n");
        //2.把 [addr,addr+size] 区间的数据全部清零(0xff)
        memset(readDataBuf+ addr-eraseAddr,0xff,size);
        //printf("%08x,%d\r\n",addr-eraseAddr,size);

        // for(uint16_t i=0;i<FLASH_PAGE_SIZE;i++)
        // {
        //     printf("%02x ",readDataBuf[i]);
        // }
        // printf("\r\n");
        isNeedReadData=1;
    }


    //开始擦除
    ret=STM_FLASH_Erase(eraseAddr,pageNum);
    if(ret!=HAL_OK) return ret+50;
    
    //printf("erase over\n");
    uint16_t temp=0;

    
    //写入FLASH之前需要解锁
    ret=HAL_FLASH_Unlock();
    if(ret!=HAL_OK) return ret;
    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2); // 72 MHz 时至少要 2 wait
    
    //1.先写入擦除了的数据
    if(isNeedReadData)
    {
        //for(uint16_t i=0;i<addr-eraseAddr;i+=2)  ❌️
        for(uint16_t i=0;i<FLASH_PAGE_SIZE;i+=2) //这里是把要写入的部分数据擦除后再写入flash
        {
            temp=readDataBuf[i];
            temp |= (((uint16_t)readDataBuf[i+1])<<8);
            
            ret=HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,eraseAddr,temp);
            if(ret!=HAL_OK) 
            {
                printf("i=%d\n",i);
                return ret+100;
            }
            eraseAddr+=2;
        }
    }

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

    if(isNeedReadData==1)
    {
        #ifdef USE_FreeRTOS
        vPortFree(readDataBuf);
        #else
        free(readDataBuf); 
        #endif
    }

    //printf("write over\n");
    return 0;
}


/// @brief 写入半字到内部FLASH，不包含擦除（写入前必须已经擦除好目标页）。
///        用于升级场景：升级开始前一次性擦除备份区，之后逐帧直接写入，避免"每帧擦除-重写"破坏数据。
/// @param addr 起始地址，必须是2的倍数
/// @param data 字节数据
/// @param size 字节数
/// @return 0：成功
uint8_t STM_FLASH_Write_NoErase(uint32_t addr, uint8_t* data,uint16_t size)
{
    if(addr<FLASH_START_ADDR || addr>CHIP_FLASH_MAX_ADDR) return HAL_ERROR;

    HAL_StatusTypeDef ret=0; 
    ret=HAL_FLASH_Unlock();
    if(ret!=HAL_OK) return ret;

    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2); // 72 MHz 时至少要 2 wait


    // 逐半字写入，不擦除
    uint16_t temp=0;
    for(uint16_t i=0;i<size;i+=2)
    {
        temp=data[i];
        temp |= ((uint16_t)(data[i+1])<<8);
        ret=HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addr,temp);
        if(ret!=HAL_OK) 
        {
            printf("i=%d write fail\n",i);
            HAL_FLASH_Lock();
            return ret+100;
        }
        addr+=2;
    }

    HAL_FLASH_Lock();
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


void STM_FLASH_ReadData(uint32_t addr,uint8_t* data,uint16_t size)
{
    for(uint16_t i=0;i<size;i++)
    {
        data[i]=STM_FLASH_Read_Byte(addr);
        addr++;
    }
}


uint16_t STM_FLASH_Get_ChipFlash()
{
   return (*((volatile uint16_t*)(0x1FFFF7E0)));
}