#include"i2s.h"

I2S_HandleTypeDef i2sHandle={0};
I2S_HandleTypeDef i2s2Handle={0};

DMA_HandleTypeDef i2sDmaHandle={0};
DMA_HandleTypeDef i2s2DmaHandle={0};


typedef struct
{
  uint8_t txFlag:1;
  uint8_t rxFlag:1;
}I2S_Flag;

I2S_Flag i2sFlag={0};
I2S_Flag i2s2Flag={0};


//I2S_AUDIOFREQ_xxx 如：I2S_AUDIOFREQ_48K
void I2S_Init(uint32_t sampleRate)
{
  i2sHandle.Instance = I2Sx;
  i2sHandle.Init.Mode = I2S_MODE_MASTER_TX;
  i2sHandle.Init.Standard = I2S_STANDARD_PHILIPS;
  i2sHandle.Init.DataFormat = I2S_DATAFORMAT_16B;
  i2sHandle.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  i2sHandle.Init.AudioFreq =sampleRate;;
  i2sHandle.Init.ClockSource = I2S_CLOCK_PLL;
  i2sHandle.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  i2sHandle.Init.CPOL = I2S_CPOL_LOW;
  if (HAL_I2S_Init(&i2sHandle) != HAL_OK)
  {
    printf("i2s init fail\n");
    return;
    //Error_Handler();
  }

  #if I2Sx_MODE==1
  HAL_NVIC_SetPriority(I2Sx_IRQn,0,3);
  HAL_NVIC_EnableIRQ(I2Sx_IRQn);
  #elif I2Sx_MODE==2

  I2Sx_DMA_CLK_ENABLE();
  i2sDmaHandle.Instance=I2Sx_DMA_Stream;
  i2sDmaHandle.Init.Channel=I2Sx_DMA_CHANNEL;
  i2sDmaHandle.Init.Direction=DMA_MEMORY_TO_PERIPH;
  i2sDmaHandle.Init.Mode=DMA_NORMAL;
  i2sDmaHandle.Init.MemInc=DMA_MINC_ENABLE;
  i2sDmaHandle.Init.PeriphInc=DMA_PINC_DISABLE;
  i2sDmaHandle.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;
  i2sDmaHandle.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;
  i2sDmaHandle.Init.Priority=DMA_PRIORITY_HIGH;
  if(HAL_DMA_Init(&i2sDmaHandle))
  {
    printf("i2s dma init fail\n");
    return;
  }

  #ifdef I2Sx_DMA_TX
  __HAL_LINKDMA(&i2sHandle,hdmatx,i2sDmaHandle);
  #endif
  #ifdef I2Sx_DMA_RX
  __HAL_LINKDMA(&i2sHandle,hdmarx,i2sDmaHandle);
  #endif
  HAL_NVIC_SetPriority(I2Sx_DMA_Stream_IRQn,0,0);
  HAL_NVIC_EnableIRQ(I2Sx_DMA_Stream_IRQn);
  #endif
}

//I2S_AUDIOFREQ_xxx 如：I2S_AUDIOFREQ_48K
void I2S2_Init(uint32_t sampleRate)
{
  i2s2Handle.Instance = I2Sx2;
  i2s2Handle.Init.Mode = I2S_MODE_MASTER_RX;
  i2s2Handle.Init.Standard = I2S_STANDARD_PHILIPS;
  i2s2Handle.Init.DataFormat = I2S_DATAFORMAT_24B;
  i2s2Handle.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  i2s2Handle.Init.AudioFreq =sampleRate;;
  i2s2Handle.Init.ClockSource = I2S_CLOCK_PLL;
  i2s2Handle.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  i2s2Handle.Init.CPOL = I2S_CPOL_LOW;
  if (HAL_I2S_Init(&i2s2Handle) != HAL_OK)
  {
    printf("i2s init fail\n");
    return;
    //Error_Handler();
  }

  #if I2Sx2_MODE==1
  HAL_NVIC_SetPriority(I2Sx2_IRQn,0,3);
  HAL_NVIC_EnableIRQ(I2Sx2_IRQn);
  #elif I2Sx2_MODE==2

  I2Sx2_DMA_CLK_ENABLE();
  i2s2DmaHandle.Instance=I2Sx2_DMA_Stream;
  i2s2DmaHandle.Init.Channel=I2Sx2_DMA_CHANNEL;
  i2s2DmaHandle.Init.Direction=DMA_PERIPH_TO_MEMORY;
  i2s2DmaHandle.Init.Mode=DMA_NORMAL;
  i2s2DmaHandle.Init.MemInc=DMA_MINC_ENABLE;
  i2s2DmaHandle.Init.PeriphInc=DMA_PINC_DISABLE;
  i2s2DmaHandle.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;
  i2s2DmaHandle.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;
  i2s2DmaHandle.Init.Priority=DMA_PRIORITY_HIGH;
  if(HAL_DMA_Init(&i2s2DmaHandle))
  {
    printf("i2s dma init fail\n");
    return;
  }

  #ifdef I2Sx2_DMA_TX
  __HAL_LINKDMA(&i2s2Handle,hdmatx,i2s2DmaHandle);
  #endif
  #ifdef I2Sx2_DMA_RX
  __HAL_LINKDMA(&i2s2Handle,hdmarx,i2s2DmaHandle);
  #endif
  HAL_NVIC_SetPriority(I2Sx2_DMA_Stream_IRQn,2,0);
  HAL_NVIC_EnableIRQ(I2Sx2_DMA_Stream_IRQn);
  #endif
}

void HAL_I2S_MspInit(I2S_HandleTypeDef* hi2s)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 16;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    printf("i2s pclk fail\n");
  }

  if(hi2s->Instance==I2Sx)
  {
    I2Sx_CLK_ENABLE();
    I2Sx_CK_CLK_ENABLE();     
    I2Sx_WS_CLK_ENABLE();
    I2Sx_SD_CLK_ENABLE();


    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = I2Sx_WS_GPIO_PIN;
    GPIO_InitStruct.Alternate=I2Sx_WS_GPIO_AF;
    HAL_GPIO_Init(I2Sx_WS_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = I2Sx_CK_GPIO_PIN;
    GPIO_InitStruct.Alternate=I2Sx_CK_GPIO_AF;
    HAL_GPIO_Init(I2Sx_CK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = I2Sx_SD_GPIO_PIN;
    GPIO_InitStruct.Alternate=I2Sx_SD_GPIO_AF;
    HAL_GPIO_Init(I2Sx_SD_GPIO_PORT, &GPIO_InitStruct);
  }
  if(hi2s->Instance==I2Sx2)
  {
    I2Sx2_CLK_ENABLE();
    I2Sx2_CK_CLK_ENABLE();     
    I2Sx2_WS_CLK_ENABLE();
    I2Sx2_SD_CLK_ENABLE();


    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = I2Sx2_WS_GPIO_PIN;
    GPIO_InitStruct.Alternate=I2Sx2_WS_GPIO_AF;
    HAL_GPIO_Init(I2Sx2_WS_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = I2Sx2_CK_GPIO_PIN;
    GPIO_InitStruct.Alternate=I2Sx2_CK_GPIO_AF;
    HAL_GPIO_Init(I2Sx2_CK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = I2Sx2_SD_GPIO_PIN;
    GPIO_InitStruct.Alternate=I2Sx2_SD_GPIO_AF;
    HAL_GPIO_Init(I2Sx2_SD_GPIO_PORT, &GPIO_InitStruct);
  }
}

uint8_t I2S_Transmit(uint16_t *data, uint16_t size,I2S_ID i2sId)
{
  if(i2sId==I2S1_ID)
  {
    return HAL_I2S_Transmit(&i2sHandle,data,size,1000);
  }
  else if(i2sId==I2S2_ID)
  {
    return HAL_I2S_Transmit(&i2s2Handle,data,size,1000);
  }

}

uint8_t I2S_Receive(uint16_t *rxData, uint16_t size,I2S_ID i2sId)
{
  if(i2sId==I2S1_ID)
  {
    return HAL_I2S_Receive(&i2sHandle,rxData,size,1000);
  }
  else if(i2sId==I2S2_ID)
  {
    return HAL_I2S_Receive(&i2s2Handle,rxData,size,1000);
  }
}

void I2S_Stop()
{
  __HAL_I2S_DISABLE(&i2sHandle);
  delay_ms(1);
  __HAL_I2S_ENABLE(&i2sHandle);
}



/**********************----------------中断篇-----------------********************** */
uint8_t I2S_Transmit_IT(uint16_t *rxData, uint16_t size,I2S_ID i2sId)
{
  if(i2sId==I2S1_ID)
  {
    return HAL_I2S_Transmit_IT(&i2sHandle,rxData,size);
  }
}


void I2Sx_IRQHandler()
{
  HAL_I2S_IRQHandler(&i2sHandle);
}

void I2Sx2_IRQHandler()
{
  HAL_I2S_IRQHandler(&i2s2Handle);
}



/*--------------------------DMA 篇---------------------------*/
uint8_t I2S_Transmit_DMA(uint16_t *txData, uint16_t size,I2S_ID i2sId)
{
  if(i2sId==I2S1_ID)
  {
    return HAL_I2S_Transmit_DMA(&i2sHandle,txData,size);
  }
  else if(i2sId==I2S2_ID)
  {
    return HAL_I2S_Transmit_DMA(&i2s2Handle,txData,size);
  }
}


uint8_t I2S_Receive_DMA(uint16_t *rxData, uint16_t size,I2S_ID i2sId)
{
  if(i2sId==I2S1_ID)
  {
    return HAL_I2S_Receive_DMA(&i2sHandle,rxData,size);
  }
  else if(i2sId==I2S2_ID)
  {
    return HAL_I2S_Receive_DMA(&i2s2Handle,rxData,size);
  }
}

void I2Sx_DMA_IRQHandler()
{
  HAL_DMA_IRQHandler(&i2sDmaHandle);
}

void I2Sx2_DMA_IRQHandler()
{
  HAL_DMA_IRQHandler(&i2s2DmaHandle);
}




/************ ****************/
/// @brief以中断/DMA方式 等待I2S传输完成
/// @return 0：传输完成，其它未完成
uint8_t I2S_WaitTxCplt(I2S_ID i2sId)
{
  if(i2sId==I2S1_ID)
  {
    if(i2sFlag.txFlag==1) 
    {
      i2sFlag.txFlag=0;
      return 0;
    }
  }
  else if(i2sId==I2S2_ID)
  {
    if(i2s2Flag.txFlag==1) 
    {
      i2s2Flag.txFlag=0;
      return 0;
    }
  }

  return 1;
}


/// @brief以中断/DMA方式 等待I2S读取数据完成
/// @return 0：读取数据完成，其它未完成
uint8_t I2S_WaitRxCplt(I2S_ID i2sId)
{
  if(i2sId==I2S1_ID)
  {
    if(i2sFlag.rxFlag==1) 
    {
      i2sFlag.rxFlag=0;
      return 0;
    }
  }
  else if(i2sId==I2S2_ID)
  {
    if(i2s2Flag.rxFlag==1) 
    {
      i2s2Flag.rxFlag=0;
      return 0;
    }
  }
  
  return 1;
}



void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if(hi2s->Instance==I2Sx)
  {
    i2sFlag.txFlag=1;
  }
  else if(hi2s->Instance==I2Sx2)
  {
    i2s2Flag.txFlag=1;
  }
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if(hi2s->Instance==I2Sx)
  {
    i2sFlag.rxFlag=1;
  }
  else if(hi2s->Instance==I2Sx2)
  {
    i2s2Flag.rxFlag=1;
    //printf("v\n");
  }
}