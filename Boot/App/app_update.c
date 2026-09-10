#include"app_update.h"


#ifndef USE_BOOT

AppUpdate_Info_t updateInfo={0};

int  AppUpdate_Start(DataFrame_t* df)
{
    //清空数据
    //memset(&updateInfo,0,sizeof(AppUpdate_Info_t));
    updateInfo.isUpdating=1;
    updateInfo.allSize=0;
    updateInfo.rxAllSize=0;
    updateInfo.readFlashBuf=NULL;


    //1.获取本次传输的固件大小
    updateInfo.allSize=(df->body.data[3]<<24) |(df->body.data[2]<<16) |(df->body.data[1]<<8) |(df->body.data[0]);

    printf("rx fm size:%ld\r\n",updateInfo.allSize);

    //2.升级开始前，一次性擦除整个备份区。
    //  之后每帧用 STM_FLASH_Write_NoErase 直接写入（不擦除），避免"每帧擦除-重写"破坏已写入的数据。
    uint32_t erasePages = updateInfo.allSize / CHIP_FLASH_PAGE_SIZE;
    if(updateInfo.allSize % CHIP_FLASH_PAGE_SIZE != 0) erasePages++;
    if(erasePages==0) erasePages=1;
    uint8_t eraseRet = STM_FLASH_Erase(SAVE_APP_START_ADDR, erasePages);
    if(eraseRet!=0)
    {
        printf("erase backup flash fail,ret=%d\r\n",eraseRet);
        DataFrame_Response(ERR_CODE_WRITE_FLASH_ERR);
        return RET_ERR;
    }

    DataFrame_Response(ERR_CODE_OK);
    return RET_OK;
}


int AppUpdate_Updating(DataFrame_t* df)
{
    uint8_t writeMaxTimes=3;
    uint8_t ret=0;
    while(1)
    {
        // 升级开始前已一次性擦除备份区，这里只写不擦，避免"每帧擦除-重写"破坏已写入的数据
        ret=STM_FLASH_Write_NoErase(SAVE_APP_START_ADDR+updateInfo.rxAllSize,df->body.data,df->head.len);
        if(ret==0)
        {
            break;
        }
        writeMaxTimes--;

        vTaskDelay(200);
        if(writeMaxTimes==0)
        {
            DataFrame_Response(ERR_CODE_WRITE_FLASH_ERR);
            printf("write data to flash fail\r\n");

            //xTaskResumeAll(); //开启任务切换

            return RET_ERR;
        }
    }


    //本次传输的固件大小
    updateInfo.rxAllSize+=(df->head.len);
    printf("\r\n%ld/%ldKB,%d%%\r\n",updateInfo.rxAllSize,updateInfo.allSize,updateInfo.rxAllSize*100/updateInfo.allSize);

    DataFrame_Response(ERR_CODE_OK);

    return RET_OK;
}


int AppUpdate_Finished(DataFrame_t* df)
{
    uint32_t calCrc32=0; //计算得到的CRC32值

    //1.先判定接收的数据大小和 一开始给的一不一样
    if(updateInfo.rxAllSize!=updateInfo.allSize)
    {
        DEBUG_E(-1,"rx size no match,%ld\r\n",updateInfo.rxAllSize);
        DataFrame_Response(ERR_CODE_UNKONW);
        xTaskResumeAll();

        return RET_ERR;
    }

    //1.开始校验
    updateInfo.readFlashBuf=(uint8_t*)pvPortMalloc(CHIP_FLASH_PAGE_SIZE);
    if(updateInfo.readFlashBuf==NULL)
    {
        DEBUG_E(-1,"pvPortMalloc");
        DataFrame_Response(ERR_CODE_UNKONW);
        xTaskResumeAll();
        return RET_ERR;
    }

    //校验初始化
    CRC32_CTX ctx={0};
    CRC32_Init(&ctx);

    //【诊断】打印关键信息：固件大小、保存起始地址、Flash 中固件前 16 字节
    // printf("[diag] allSize=%lu rxAllSize=%lu SAVE_APP=0x%08X PAGE_SIZE=%lu\r\n",
    //        (unsigned long)updateInfo.allSize,
    //        (unsigned long)updateInfo.rxAllSize,
    //        (unsigned int)SAVE_APP_START_ADDR,
    //        (unsigned long)CHIP_FLASH_PAGE_SIZE);
    // STM_FLASH_ReadData(SAVE_APP_START_ADDR, updateInfo.readFlashBuf, 16);
    // printf("[diag] fm head @0x%08X: ", (unsigned int)SAVE_APP_START_ADDR);
    // for(int i=0;i<16;i++) printf("%02X ", updateInfo.readFlashBuf[i]);
    // printf("\r\n");

    // 按页(1024字节)分块读取并累加 CRC32
    for(uint16_t i=0;i<updateInfo.allSize/CHIP_FLASH_PAGE_SIZE;i++)
    {
        memset(updateInfo.readFlashBuf,0,CHIP_FLASH_PAGE_SIZE);
        //1.1 读取数据
        STM_FLASH_ReadData(SAVE_APP_START_ADDR+i*CHIP_FLASH_PAGE_SIZE,updateInfo.readFlashBuf,CHIP_FLASH_PAGE_SIZE);
        
        //1.2 校验累加
        CRC32_Update(&ctx,updateInfo.readFlashBuf,CHIP_FLASH_PAGE_SIZE);
    }

    //判断是不是整页，不是就要再读部分
    if(updateInfo.allSize%CHIP_FLASH_PAGE_SIZE!=0) //2000/1024=976
    {
        //1.1 读取数据
        uint32_t lastAddr=SAVE_APP_START_ADDR+(updateInfo.allSize/CHIP_FLASH_PAGE_SIZE*CHIP_FLASH_PAGE_SIZE);
        uint16_t lastSize=updateInfo.allSize%CHIP_FLASH_PAGE_SIZE;
        STM_FLASH_ReadData(lastAddr,updateInfo.readFlashBuf,lastSize);
        //1.2 校验累加
        CRC32_Update(&ctx,updateInfo.readFlashBuf,lastSize);
    }
    

    //1.4 释放内存
    vPortFree(updateInfo.readFlashBuf);
    updateInfo.readFlashBuf=NULL;

    //1.5 最终校验
    CRC32_Final(&ctx,&calCrc32);

    //接收的CRC32校验
    uint32_t rxCrc32= (df->body.data[3]<<24) | (df->body.data[2]<<16) | (df->body.data[1]<<8) | (df->body.data[0]);
    if(calCrc32!=rxCrc32)
    {
        DEBUG_E2(-1,"fireware crc fail:%08x,%08x\r\n",rxCrc32,calCrc32);
        DataFrame_Response(ERR_CODE_FIRMWARE_CRC_ERR);
        return RET_ERR;
    }

    //开启任务切换
    //xTaskResumeAll();

    DataFrame_Response(ERR_CODE_OK);

    updateInfo.isUpdating=0; //更新结束
    return RET_ERR;
}

#else 


uint8_t readBuf[FLASH_PAGE_SIZE]={0};
/// @brief 开始在boot里更新固件，把得到的固件包搬运到app区
/// @return 0成功
int AppUpdate_UpdateFirmware()
{
    int ret=0;

    //新旧固件大小
    uint32_t factoryFrimSize=sysConfInfo.factoryFirmware.size;
    uint32_t updateFirmSize=sysConfInfo.updateFireware.size;

    //旧的固件的页数
    uint16_t factoryFirmPage=factoryFrimSize/FLASH_PAGE_SIZE;
    if(factoryFrimSize%FLASH_PAGE_SIZE!=0) factoryFirmPage++;

    //新的固件的页数
    uint16_t updateFirmPage=updateFirmSize/FLASH_PAGE_SIZE;
    if(updateFirmSize%FLASH_PAGE_SIZE!=0) updateFirmPage++;


    //1.先擦除整块APP区(按新固件页数擦除,防止新固件比旧固件大时APP区擦不干净)
    ret=STM_FLASH_Erase(APP_START_ADDR,updateFirmPage);
    if(ret!=0)
    {
        return ret;
    }

    //2.读取这个新的固件的总大小
    uint32_t offsetAddr=0; //偏移地址
    for(uint16_t i=0;i<updateFirmPage;i++)
    {
        //2.1搬运得到的APP数据
        STM_FLASH_ReadData(SAVE_APP_START_ADDR+offsetAddr,readBuf,FLASH_PAGE_SIZE);

        //2.2把得到的数据写入到APP区
        STM_FLASH_Write(APP_START_ADDR+offsetAddr,readBuf,FLASH_PAGE_SIZE);

        offsetAddr+=FLASH_PAGE_SIZE;
    }

    //3.检验 写入app区的新固件 
    CRC32_CTX ctx={0};
    uint32_t readCrc32=sysConfInfo.updateFireware.crc32;
    uint32_t calCrc32=0;
    CRC32_Init(&ctx);


    offsetAddr=0; //偏移地址归零
    for(uint16_t i=0;i<updateFirmSize/FLASH_PAGE_SIZE;i++)
    {
        STM_FLASH_ReadData(APP_START_ADDR+offsetAddr,readBuf,FLASH_PAGE_SIZE);   
        CRC32_Update(&ctx,readBuf,FLASH_PAGE_SIZE);
        offsetAddr+=FLASH_PAGE_SIZE;
    }

    //4.把剩余 还没读取的APP区固件数据读取出来，如果有的话
    uint32_t leftSize=updateFirmSize%FLASH_PAGE_SIZE;
    if(leftSize!=0)
    {
        STM_FLASH_ReadData(APP_START_ADDR+offsetAddr,readBuf,leftSize);   
        CRC32_Update(&ctx,readBuf,leftSize);
    }

    //5.校验
    CRC32_Final(&ctx,&calCrc32);

    if(calCrc32!=readCrc32)
    {
        LOG_E(-1,"firmmware crc32 fail,cal crc:%08x,read crc:%08x\r\n",calCrc32,readCrc32);
        return -1;
    }

    return RET_OK;
}


#endif