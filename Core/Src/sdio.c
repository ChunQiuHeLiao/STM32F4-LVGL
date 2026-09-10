#include "sdio.h"

SD_HandleTypeDef sdHandle={0};
SD_CardInfoHandle sdCardInfoHandle={0};


#ifdef SDIOx_EN_DMA
DMA_HandleTypeDef sd_dmaHandle={0};
uint8_t sd_dma_rxFlag=0; //判断DMA是否把SD卡的数据搬运完成到内存。完成1，没有0
#endif


uint8_t SD_Init()
{
  sdHandle.Instance=SDIOx;
  sdHandle.Init.ClockEdge=SDIO_CLOCK_EDGE_RISING;
  sdHandle.Init.ClockBypass=SDIO_CLOCK_BYPASS_DISABLE;
  sdHandle.Init.ClockPowerSave=SDIO_CLOCK_POWER_SAVE_DISABLE;
  sdHandle.Init.BusWide=SDIO_BUS_WIDE_1B; //一开始先使用一根数据线，后面改
  sdHandle.Init.HardwareFlowControl=SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  sdHandle.Init.ClockDiv=SDIO_TRANSFER_CLK_DIV; //初始化分频系数，分频后SDIO不能超过400KHz,时钟一定要是非旁路
  
  HAL_StatusTypeDef status=HAL_SD_Init(&sdHandle);
  if(status!=HAL_OK)
  {
    printf("sdio init fail %d\n",status);
    return status;
  }

  status=HAL_SD_ConfigWideBusOperation(&sdHandle, SDIO_BUS_WIDE_4B);
  if(status!= HAL_OK)
  {
    printf("sdio HAL_SD_ConfigWideBusOperation fail %d\n",status);
    return status+100;
  }

  SD_GetCardMainInfo(&sdCardInfoHandle);

  #ifdef SDIOx_EN_DMA

  // 使能中断
  HAL_NVIC_SetPriority(SDIO_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SDIO_IRQn);
  SD_DMA_Init();

  #endif

  return 0;
}

uint8_t SD_DeInit()
{
  HAL_SD_DeInit(&sdHandle);
}

void HAL_SD_MspInit(SD_HandleTypeDef* sdHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(sdHandle->Instance==SDIOx)
  {
    SDIOx_CLK_ENABLE();

    SDx_CK_GPIO_CLK_ENABLE();
    SDx_CMD_GPIO_CLK_ENABLE();
    SDx_D0_GPIO_CLK_ENABLE();
    SDx_D1_GPIO_CLK_ENABLE();
    SDx_D2_GPIO_CLK_ENABLE();
    SDx_D3_GPIO_CLK_ENABLE();


    GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin=SDx_CK_GPIO_PIN;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate=SDx_GPIO_AF;
    HAL_GPIO_Init(SDx_CK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin=SDx_CMD_GPIO_PIN;
    GPIO_InitStruct.Alternate=SDx_GPIO_AF;
    HAL_GPIO_Init(SDx_CMD_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin=SDx_D0_GPIO_PIN;
    GPIO_InitStruct.Alternate=SDx_GPIO_AF;
    HAL_GPIO_Init(SDx_D0_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin=SDx_D1_GPIO_PIN;
    GPIO_InitStruct.Alternate=SDx_GPIO_AF;
    HAL_GPIO_Init(SDx_D1_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin=SDx_D2_GPIO_PIN;
    GPIO_InitStruct.Alternate=SDx_GPIO_AF;
    HAL_GPIO_Init(SDx_D2_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin=SDx_D3_GPIO_PIN;
    GPIO_InitStruct.Alternate=SDx_GPIO_AF;
    HAL_GPIO_Init(SDx_D3_GPIO_PORT, &GPIO_InitStruct);


    HAL_GPIO_WritePin(SDx_CK_GPIO_PORT,SDx_CK_GPIO_PIN,1);
    HAL_GPIO_WritePin(SDx_CMD_GPIO_PORT,SDx_CMD_GPIO_PIN,1);
    HAL_GPIO_WritePin(SDx_D0_GPIO_PORT,SDx_D0_GPIO_PIN,1);
    HAL_GPIO_WritePin(SDx_D1_GPIO_PORT,SDx_D1_GPIO_PIN,1);
    HAL_GPIO_WritePin(SDx_D2_GPIO_PORT,SDx_D2_GPIO_PIN,1);
    HAL_GPIO_WritePin(SDx_D3_GPIO_PORT,SDx_D3_GPIO_PIN,1);

    //  GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
    // GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStruct.Pin=SDx_CK_GPIO_PIN;
    // GPIO_InitStruct.Pull=GPIO_NOPULL;
    // HAL_GPIO_Init(SDx_CK_GPIO_PORT, &GPIO_InitStruct);

    // GPIO_InitStruct.Pin=SDx_CMD_GPIO_PIN;
    // HAL_GPIO_Init(SDx_CMD_GPIO_PORT, &GPIO_InitStruct);

    // GPIO_InitStruct.Pin=SDx_D0_GPIO_PIN;
    // HAL_GPIO_Init(SDx_D0_GPIO_PORT, &GPIO_InitStruct);

    // GPIO_InitStruct.Pin=SDx_D1_GPIO_PIN;
    // HAL_GPIO_Init(SDx_D1_GPIO_PORT, &GPIO_InitStruct);

    // GPIO_InitStruct.Pin=SDx_D2_GPIO_PIN;
    // HAL_GPIO_Init(SDx_D2_GPIO_PORT, &GPIO_InitStruct);

    // GPIO_InitStruct.Pin=SDx_D3_GPIO_PIN;
    // HAL_GPIO_Init(SDx_D3_GPIO_PORT, &GPIO_InitStruct);


    // while(1)
    // {
    //   HAL_GPIO_WritePin(SDx_CK_GPIO_PORT,SDx_CK_GPIO_PIN,1);
    //   HAL_GPIO_WritePin(SDx_CMD_GPIO_PORT,SDx_CMD_GPIO_PIN,1);
    //   HAL_GPIO_WritePin(SDx_D0_GPIO_PORT,SDx_D0_GPIO_PIN,1);
    //   HAL_GPIO_WritePin(SDx_D1_GPIO_PORT,SDx_D1_GPIO_PIN,1);
    //   HAL_GPIO_WritePin(SDx_D2_GPIO_PORT,SDx_D2_GPIO_PIN,1);
    //   HAL_GPIO_WritePin(SDx_D3_GPIO_PORT,SDx_D3_GPIO_PIN,1);

    //   printf("--1--\n");
    //   delay_ms(1000);
    
    //   HAL_GPIO_WritePin(SDx_CK_GPIO_PORT,SDx_CK_GPIO_PIN,0);
    //   HAL_GPIO_WritePin(SDx_CMD_GPIO_PORT,SDx_CMD_GPIO_PIN,0);
    //   HAL_GPIO_WritePin(SDx_D0_GPIO_PORT,SDx_D0_GPIO_PIN,0);
    //   HAL_GPIO_WritePin(SDx_D1_GPIO_PORT,SDx_D1_GPIO_PIN,0);
    //   HAL_GPIO_WritePin(SDx_D2_GPIO_PORT,SDx_D2_GPIO_PIN,0);
    //   HAL_GPIO_WritePin(SDx_D3_GPIO_PORT,SDx_D3_GPIO_PIN,0);

    //         printf("--2--\n");
    //   delay_ms(1000);
    // }
  }
}

void HAL_SD_MspDeInit(SD_HandleTypeDef* sdHandle)
{

   GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(sdHandle->Instance==SDIOx)
  {
    SDIOx_CLK_DISABLE();

    SDx_CK_GPIO_CLK_DISABLE();
    SDx_CMD_GPIO_CLK_DISABLE();
    SDx_D0_GPIO_CLK_DISABLE();
    SDx_D1_GPIO_CLK_DISABLE();
    SDx_D2_GPIO_CLK_DISABLE();
    SDx_D3_GPIO_CLK_DISABLE();


    GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin=SDx_CK_GPIO_PIN;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    HAL_GPIO_DeInit(SDx_CK_GPIO_PORT,SDx_CK_GPIO_PIN);

    GPIO_InitStruct.Pin=SDx_CMD_GPIO_PIN;
    HAL_GPIO_DeInit(SDx_CMD_GPIO_PORT, SDx_CMD_GPIO_PIN);

    GPIO_InitStruct.Pin=SDx_D0_GPIO_PIN;
    HAL_GPIO_DeInit(SDx_D0_GPIO_PORT, SDx_D0_GPIO_PIN);

    GPIO_InitStruct.Pin=SDx_D1_GPIO_PIN;
    HAL_GPIO_DeInit(SDx_D1_GPIO_PORT, SDx_D1_GPIO_PIN);

    GPIO_InitStruct.Pin=SDx_D2_GPIO_PIN;
    HAL_GPIO_DeInit(SDx_D2_GPIO_PORT, SDx_D2_GPIO_PIN);

    GPIO_InitStruct.Pin=SDx_D3_GPIO_PIN;
    HAL_GPIO_DeInit(SDx_D3_GPIO_PORT, SDx_D3_GPIO_PIN);
  }
}



#ifdef SDIOx_EN_DMA


void SD_DMA_Init()
{
  __HAL_RCC_DMA2_CLK_ENABLE(); //千万不能忘
  sd_dmaHandle.Instance=DMA2_Channel4;
  sd_dmaHandle.Init.Direction=DMA_PERIPH_TO_MEMORY;
  sd_dmaHandle.Init.Mode=DMA_NORMAL;
  sd_dmaHandle.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
  sd_dmaHandle.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
  sd_dmaHandle.Init.PeriphInc=DMA_PINC_DISABLE;
  sd_dmaHandle.Init.MemInc=DMA_PINC_ENABLE;
  sd_dmaHandle.Init.Priority=DMA_PRIORITY_HIGH;

  uint8_t ret=HAL_DMA_Init(&sd_dmaHandle);
  if(ret)
  {
    printf("sd dma init fail:%d\n",ret);
  }

  __HAL_LINKDMA(&sdHandle,hdmarx,sd_dmaHandle);

  HAL_NVIC_SetPriority(DMA2_Channel4_IRQn,0,0);
  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
}


/// @brief 获取DMA从SD卡搬运数据到内存完成情况。1，完成，0没有或未开始搬运。此函数调用后接收标志位会自己清零
/// @return 
uint8_t SD_DMA_GetRxFlag()
{
  if(sd_dma_rxFlag==0) return 0;
  sd_dma_rxFlag=0;
  return 1;
}


uint8_t SD_ReadBlocks_DMA(uint8_t *pData, uint32_t blockAddr, uint32_t blockNum)
{
  //先判断下SD卡是不是处于就绪态
  uint16_t timeout=1000; //最多等1000ms
  while (SD_GetFreeState()!=0)
  {
    delay_ms(1);
    timeout--;
    if(timeout==0) return SD_GetFreeState()+100;
  }

  uint8_t ret=0;
  ret=HAL_SD_ReadBlocks_DMA(&sdHandle,pData,blockAddr,blockNum);
  if(ret)
  {
    printf("sd dma read fail:%d\n",ret);
    return ret;
  }
  return 0;
}


#endif


/// @brief 阻塞形式从SD卡中读取数据(块=512Byte)
/// @param pData 读取的数据
/// @param blockAddr 读取的块地址,一块512字节，0开始
/// @param blockNum 读取块的个数
/// @return 返回读取状态。0代表写入成功并空闲，1代表写入失败，2代表忙，100代表写入完成但等待空闲超时
uint8_t SD_ReadBlocks(uint8_t *pData, uint32_t blockAddr, uint32_t blockNum)
{
  //先判断下SD卡是不是处于就绪态
  uint16_t timeout=1000; //最多等1000ms
  while (SD_GetFreeState()!=0)
  {
    delay_ms(1);
    timeout--;
    if(timeout==0) return SD_GetFreeState()+100;
  }
  
  uint8_t ret=0;
  // __disable_irq(); //关闭总中断，防止中断打断数据写入
  ret=HAL_SD_ReadBlocks(&sdHandle,pData,blockAddr,blockNum,1000);
  // __enable_irq(); //开启总中断

  if(ret!=HAL_OK) return ret;

  return 0;
}



/// @brief 阻塞形式将数据写入SD卡(块=512Byte)
/// @param pData 要写入的数据
/// @param blockAddr 写入的块地址，一块512字节，0开始
/// @param blockNum 写入块的个数
/// @return 返回写入状态,0代表写入成功并空闲，1代表写入失败，2代表忙，100代表写入完成但等待空闲超时
uint8_t SD_WrietBlocks(uint8_t *pData, uint32_t blockAddr, uint32_t blockNum)
{
  //先判断下SD卡是不是处于就绪态
  uint16_t timeout=1000; //最多等1000ms
  while (SD_GetFreeState()!=0)
  {
    delay_ms(1);
    timeout--;
    if(timeout==0) return SD_GetFreeState()+100;
  }


  uint8_t ret=0;
  //__disable_irq(); //关闭总中断，防止中断打断数据写入
  ret=HAL_SD_WriteBlocks(&sdHandle,pData,blockAddr,blockNum,1000);
  //__enable_irq(); //开启总中断

  if(ret!=HAL_OK) return ret;

  return 0;
}


/// @brief 获取SDIO空闲状态 
/// @return 0代表空闲，代表其它看HAL_SD_CardStateTypeDef，都是不空闲
uint8_t SD_GetFreeState()
{
  
  // uint16_t timeout=20000; //40ms
  // while (1)
  // {
  //   if(HAL_GPIO_ReadPin(SDx_D0_GPIO_PORT,SDx_D0_GPIO_PIN)==1&&HAL_GPIO_ReadPin(SDx_CMD_GPIO_PORT,SDx_CMD_GPIO_PIN)==1&&
  // HAL_GPIO_ReadPin(SDx_D1_GPIO_PORT,SDx_D1_GPIO_PIN)==1&&HAL_GPIO_ReadPin(SDx_D2_GPIO_PORT,SDx_D2_GPIO_PIN)==1&&HAL_GPIO_ReadPin(SDx_D2_GPIO_PORT,SDx_D2_GPIO_PIN)==1)
  //   {
  //     return 0;
  //   }
  //   delay_us(2);
  //   timeout--;
  //   if(timeout==0) return 1;
  //   printf("d\n");
  // }
  HAL_SD_CardStateTypeDef ret=0;
  ret=HAL_SD_GetCardState(&sdHandle);
  if(ret!=HAL_SD_CARD_TRANSFER) return ret;
  return 0;
}




/**************获取SD卡信息函数*************/


//获取SD卡的容量、速度等级，扇区大小等这些产品信息
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef* carInfo)
{
  return HAL_SD_GetCardInfo(&sdHandle,carInfo);
}

//获取产品的生产的日期、序列号、生产厂家这些东西
uint8_t SD_GetCardCID(HAL_SD_CardCIDTypeDef *pCID)
{
  return HAL_SD_GetCardCID(&sdHandle,pCID);
}


//获取一些用户对卡比较关心的信息
void SD_GetCardMainInfo(SD_CardInfoHandle* cardInfoHandle)
{
  HAL_SD_CardInfoTypeDef cardInfo={0};
  HAL_SD_CardCIDTypeDef cardCid={0};
  SD_GetCardInfo(&cardInfo);
  SD_GetCardCID(&cardCid);

  if(cardInfo.CardType==CARD_SDSC)
  {
    if(cardInfo.CardVersion==CARD_V1_X) sprintf(cardInfoHandle->cardType,"SDSC V1");
    else if(cardInfo.CardVersion==CARD_V2_X) sprintf(cardInfoHandle->cardType,"SDSC V2");
  }
  else if(cardInfo.CardType==CARD_SDHC_SDXC) sprintf(cardInfoHandle->cardType,"SDHC");
  else sprintf((char*)cardInfoHandle->cardType,"unkowned");

  cardInfoHandle->capacityKB=cardInfo.BlockNbr/1000.0f*cardInfo.BlockSize;
  cardInfoHandle->capacityMB=cardInfoHandle->capacityKB/1000.0f;
  cardInfoHandle->capacityGB=cardInfoHandle->capacityMB/1000.0f;
  cardInfoHandle->blockNum=cardInfo.BlockNbr;
  cardInfoHandle->blockSize=cardInfo.BlockSize;
}

SD_CardInfoHandle* SD_GetSDInfoHandle()
{
  return &sdCardInfoHandle;
}

void SD_ShowCardInfo()
{
  HAL_SD_CardInfoTypeDef cardInfo={0};
  HAL_SD_CardCIDTypeDef cardCid={0};
  SD_GetCardInfo(&cardInfo);
  SD_GetCardCID(&cardCid);

  printf("/**********CardInfo**********/\n");
  if(cardInfo.CardType==CARD_SDSC)
  {
    if(cardInfo.CardVersion==CARD_V1_X) printf("CardType:SDSC V1\n");
    else if(cardInfo.CardVersion==CARD_V2_X) printf("CardType:SDSC V2\n");
  }
  else if(cardInfo.CardType==CARD_SDHC_SDXC) printf("CardType:SDHC\n");
  else printf("CardType:unkowned\n");

  printf("Class:%d\n",cardInfo.Class);
  printf("BlockNbr:%d\n",cardInfo.BlockNbr);
  printf("BlockSize:%d\n",cardInfo.BlockSize);
  printf("LogBlockNbr:%d\n",cardInfo.LogBlockNbr);
  printf("LogBlockSize:%d\n",cardInfo.LogBlockSize);
  printf("Capacity:%.2fGB\n",sdCardInfoHandle.capacityGB);
  printf("c2=%.2fGB\n",cardInfo.BlockNbr/1000*cardInfo.BlockSize/1000.0f/1000.0f);
}



/********************擦除扇区函数*********************/


/// @brief 擦除扇区指定区域
/// @param blockStartAddr 扇区起始地址
/// @param blockEndAddr 结束地址，包括在内，即[start_addr,end_addr]
/// @return 0:成功，1~4。100+：擦除完毕后 等待SDIO空闲超时   
uint8_t SD_EraseBlocks(uint32_t blockStartAddr, uint32_t blockEndAddr)
{
  uint8_t ret=0;
  ret=HAL_SD_Erase(&sdHandle,blockStartAddr,blockEndAddr);
  if(ret!=HAL_OK) return ret;

  uint16_t time=10000; //等2000ms
  while(SD_GetFreeState()!=0)
  {
    delay_ms(1);
    time--;
    if(time==0) 
    {
      printf("Erase ErrorCode=0x%08lX\n", sdHandle.ErrorCode);
      HAL_SD_DeInit(&sdHandle);
      HAL_SD_Init(&sdHandle);
      delay_ms(10);

      return SD_GetFreeState()+100;
    }
  }
  return 0;
}


//第二种方式擦除,擦除指定个数扇区
uint8_t SD_EraseBlocks_Num(uint32_t blockStartAddr, uint32_t blockNum)
{
  return SD_EraseBlocks(blockStartAddr,blockStartAddr+blockNum-1);
}


//擦除所有扇区，即格式化
uint8_t SD_EraseAllBlocks()
{
  uint8_t ret=0;
  ret=SD_EraseBlocks(0,1);
  if(ret!=HAL_OK) return ret;

  uint16_t time=10000; //等10000ms
  while(SD_GetFreeState()!=0)
  {
    delay_ms(10);
    time-=10;
    if(time==0) return SD_GetFreeState()+100;
  }
  return 0;
}







#ifdef SDIOx_EN_DMA

// DMA传输依赖中断来通知完成
void DMA2_Channel4_IRQHandler(void)  // SDIO DMA中断
{
  HAL_DMA_IRQHandler(&sd_dmaHandle);  // 如果关闭总中断，这个无法执行
}

#endif



#ifdef SDIOx_EN_DMA 

// SDIO 全局中断
void SDIO_IRQHandler(void)
{
  HAL_SD_IRQHandler(&sdHandle);
}


void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)  // 传输完成回调
{
  if(hsd->Instance==SDIOx)
  {
    printf("sd dma enter\n");
    sd_dma_rxFlag=1; 
  }
}

#endif