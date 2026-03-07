#include "spi.h"
#include"lcd_conf.h"
#include"lv_port_disp_template.h"

SPI_HandleTypeDef spiHandle={0};
SPI_HandleTypeDef spi2Handle={0};

DMA_HandleTypeDef spi_dmaHandle={0};
DMA_HandleTypeDef spi2_dmaHandle={0};

uint8_t spi_dmaTxFlag=2; //0:正在传输,1传输完成,2空闲，无数据传输
uint8_t spi2_dmaTxFlag=0;

void SPI_DMA_Init();
void SPI2_DMA_Init();








#ifdef SPI_EN_SPI1
void SPI_Init(void)
{
  spiHandle.Instance               = SPIx;
  spiHandle.Init.Mode = SPI_MODE_MASTER;
  spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; //50MHz
  spiHandle.Init.Direction         = SPI_DIRECTION_2LINES; 
  spiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  spiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  spiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  spiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  spiHandle.Init.NSS               = SPI_NSS_SOFT;
  
  spiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  spiHandle.Init.CRCPolynomial     = 7;
  spiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;

  HAL_SPI_Init(&spiHandle); 

  #ifdef SPI_EN_DMA_SPI1
  SPI_DMA_Init();
  #endif
}
#endif

#ifdef SPI_EN_SPI2
void SPI2_Init(void)
{
  spi2Handle.Instance               = SPIx2;
  spi2Handle.Init.Mode = SPI_MODE_MASTER;
  spi2Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; //3.125MHz
  spi2Handle.Init.Direction         = SPI_DIRECTION_2LINES;
  spi2Handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  spi2Handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  spi2Handle.Init.DataSize          = SPI_DATASIZE_8BIT;
  spi2Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  spi2Handle.Init.NSS               = SPI_NSS_SOFT;
  
  spi2Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  spi2Handle.Init.CRCPolynomial     = 7;
  spi2Handle.Init.TIMode            = SPI_TIMODE_DISABLE;

  HAL_SPI_Init(&spi2Handle); 

  #ifdef SPI_EN_DMA_SPI2
  SPI2_DMA_Init();
  #endif
}
#endif


void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct={0};
  #ifdef SPI_EN_SPI1
  if(hspi->Instance==SPIx)
  {
    //使能时钟
    SPIx_SCK_GPIO_CLK_ENABLE();        
    SPIx_MOSI_GPIO_CLK_ENABLE();   
    SPIx_CS_GPIO_CLK_ENABLE();   
    SPIx_CLK_ENABLE();

    //配置SCK引脚
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate=SPIx_GPIO_AF;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
      
    //配置MOSI引脚
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN; 
    GPIO_InitStruct.Alternate=SPIx_GPIO_AF;
    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);   

    // //配置MISO引脚
    // GPIO_InitStruct.Pin = SPIx_MISO_PIN;  
    // GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;
    // HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

    //配置CS引脚
    GPIO_InitStruct.Pin = SPIx_CS_PIN ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init( SPIx_CS_GPIO_PORT, &GPIO_InitStruct); 
   
    HAL_GPIO_WritePin(SPIx_CS_GPIO_PORT,SPIx_CS_PIN,1); //一定要拉高
    HAL_GPIO_WritePin(SPIx_SCK_GPIO_PORT,SPIx_SCK_PIN,0); //一定要拉低
  }
  #endif

  #ifdef SPI_EN_SPI2
  else if(hspi->Instance==SPIx2)
  {
    //使能时钟
    SPIx2_SCK_GPIO_CLK_ENABLE();        
    SPIx2_MOSI_GPIO_CLK_ENABLE();   
    SPIx2_MISO_GPIO_CLK_ENABLE();   
    SPIx2_CS_GPIO_CLK_ENABLE();   
    SPIx2_CLK_ENABLE();

    //配置SCK引脚
    GPIO_InitStruct.Pin       = SPIx2_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate=SPIx2_GPIO_AF;
    HAL_GPIO_Init(SPIx2_SCK_GPIO_PORT, &GPIO_InitStruct);
      

    //配置MOSI引脚
    GPIO_InitStruct.Pin = SPIx2_MOSI_PIN; 
    GPIO_InitStruct.Alternate=SPIx2_GPIO_AF;
    HAL_GPIO_Init(SPIx2_MOSI_GPIO_PORT, &GPIO_InitStruct);   

    // //配置MISO引脚
    GPIO_InitStruct.Pin = SPIx2_MISO_PIN;  
    GPIO_InitStruct.Alternate=SPIx2_GPIO_AF;
    GPIO_InitStruct.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(SPIx2_MISO_GPIO_PORT, &GPIO_InitStruct);

    //配置CS引脚
    GPIO_InitStruct.Pin = SPIx2_CS_PIN ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init( SPIx2_CS_GPIO_PORT, &GPIO_InitStruct); 
   
    HAL_GPIO_WritePin(SPIx2_CS_GPIO_PORT,SPIx2_CS_PIN,1); //一定要拉高
    HAL_GPIO_WritePin(SPIx2_SCK_GPIO_PORT,SPIx2_SCK_PIN,0); //一定要拉低
  }
  #endif
}



//开启或关闭外设
void SPI_ON_OR_OFF(uint8_t spiId,uint8_t state)
{
  if(spiId==1)
  {
    if(state==1) __HAL_SPI_ENABLE(&spiHandle);
    else if(state==0) __HAL_SPI_DISABLE(&spiHandle);
  } 
  else if(spiId==2)
  {
    if(state==1) __HAL_SPI_ENABLE(&spi2Handle);
    else if(state==0) __HAL_SPI_DISABLE(&spi2Handle);
  } 
}



#ifdef SPI_EN_DMA_SPI1
void SPI_DMA_Init()
{
  __HAL_RCC_DMA2_CLK_ENABLE(); //千万不能忘
  spi_dmaHandle.Instance=DMA2_Stream2;
  spi_dmaHandle.Init.Channel=DMA_CHANNEL_2;
  spi_dmaHandle.Init.Direction=DMA_MEMORY_TO_PERIPH;
  spi_dmaHandle.Init.Mode=DMA_NORMAL;
  spi_dmaHandle.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
  spi_dmaHandle.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
  spi_dmaHandle.Init.PeriphInc=DMA_PINC_DISABLE;
  spi_dmaHandle.Init.MemInc=DMA_MINC_ENABLE;
  spi_dmaHandle.Init.Priority=DMA_PRIORITY_HIGH;
  // F4可选：FIFO模式（默认禁用，小数据可不用）
  spi_dmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE; 
  uint8_t ret=HAL_DMA_Init(&spi_dmaHandle);
  if(ret)
  {
    printf("sd dma init fail:%d\n",ret);
  }

  __HAL_LINKDMA(&spiHandle,hdmatx,spi_dmaHandle);

  
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn,0,0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
}
#endif


#ifdef SPI_EN_DMA_SPI2
void SPI2_DMA_Init()
{
  __HAL_RCC_DMA1_CLK_ENABLE(); //千万不能忘
  spi2_dmaHandle.Instance=DMA1_Channel5;
  spi2_dmaHandle.Init.Direction=DMA_PERIPH_TO_MEMORY;
  spi2_dmaHandle.Init.Mode=DMA_NORMAL;
  spi2_dmaHandle.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
  spi2_dmaHandle.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
  spi2_dmaHandle.Init.PeriphInc=DMA_PINC_DISABLE;
  spi2_dmaHandle.Init.MemInc=DMA_PINC_ENABLE;
  spi2_dmaHandle.Init.Priority=DMA_PRIORITY_HIGH;

  uint8_t ret=HAL_DMA_Init(&spi2_dmaHandle);
  if(ret)
  {
    printf("sd dma init fail:%d\n",ret);
  }

  __HAL_LINKDMA(&spi2Handle,hdmatx,spi2_dmaHandle);

  
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn,0,0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}
#endif



#if (defined(SPI_EN_DMA_SPI1)|| defined(SPI_EN_DMA_SPI2))

/// @brief 获取DMA从内存搬运屏幕显示的数据的完成情况。1，完成，0没有或未开始搬运。此函数调用后接收标志位会自己清零
/// @return 
uint8_t SPI_DMA_GetRxFlag(uint8_t spiId)
{
  switch (spiId)
  {
  case 1:
    if(spi_dmaTxFlag==0) return 0;
    spi_dmaTxFlag=2;
    return 1;

   case 2:
    if(spi2_dmaTxFlag==0) return 0;
    spi2_dmaTxFlag=2;
    return 1;
  default:
    break;
  }
}

void SPI_DMA_ClearRxFlag(uint8_t spiId)
{
  if(spiId==1) spi_dmaTxFlag=0;
  else if(spiId==2) spi2_dmaTxFlag=0;
}


uint8_t SPI_SendData_DMA(uint8_t* data,uint16_t size,uint8_t spiId)
{
  spi_dmaTxFlag=0; //正在传输
  uint8_t ret=0;
  if(spiId==1) ret=HAL_SPI_Transmit_DMA(&spiHandle,data,size);
  if(spiId==2) ret=HAL_SPI_Transmit_DMA(&spi2Handle,data,size);
  if(ret)
  {
    printf("spi%d send dma fail:%d\n",spiId,ret);
    return ret;
  }
  return 0;
}

#endif




/// @brief 通过SPI发送数据
/// @param data 数据
/// @param size 数据大小
/// @return 返回0代表成功，其它失败
uint8_t SPI_SendData(uint8_t* data,uint16_t size,uint8_t spiId)
{
  // uint8_t rxData;
  
  // /* 等待发送缓冲区为空，TXE事件 */
  // uint16_t timeout=2000;
  // while (__HAL_SPI_GET_FLAG( &spiHandle, SPI_FLAG_TXE ) == RESET)
  // {
  //   if((timeout--) == 0) return 1;//return SPI_TIMEOUT_UserCallback(0);
  // }

  // //写入一字节到数据寄存器，后面数据会自动写入FLASH
  // WRITE_REG(spiHandle.Instance->DR, byte);
  // timeout=2000;
  // /* 等待接收缓冲区非空，RXNE事件 */
  // while (__HAL_SPI_GET_FLAG( &spiHandle, SPI_FLAG_RXNE ) == RESET)
  // {
  //   if((timeout--) == 0) return 2;//return SPI_TIMEOUT_UserCallback(1);
  // }
  // return 0;
  //return READ_REG(spiHandle.Instance->DR); //获取到缓存区数据

  if(spiId==1) return HAL_SPI_Transmit(&spiHandle,data,size,1000);
  else if(spiId==2) return HAL_SPI_Transmit(&spi2Handle,data,size,1000);
}


/// @brief 通过SPI接收数据
/// @param data 数据
/// @param size 数据大小
/// @return 返回0代表成功，其它失败
uint8_t SPI_RevData(uint8_t* data,uint16_t size,uint8_t spiId)
{
  if(spiId==1) return HAL_SPI_Receive(&spiHandle,data,size,1000);
  else if(spiId==2) return HAL_SPI_Receive(&spi2Handle,data,size,1000);
}



uint8_t SPI_TransmitRev(uint8_t* sendData,uint8_t* revData,uint16_t size)
{
  return HAL_SPI_TransmitReceive(&spi2Handle,sendData,revData,size,1000);
}

#ifdef SPI_EN_DMA_SPI1
// DMA传输完成中断
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&spi_dmaHandle);
}

#endif

#ifdef SPI_EN_DMA_SPI2
// DMA传输完成中断
void DMA1_Channel5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&spi2_dmaHandle);
}

#endif

// SPI传输完成回调
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  #ifdef SPI_EN_SPI1
  if(hspi->Instance == SPIx)
  {
    // 传输完成，可以处理后续任务
    //printf("spi\n");
    spi_dmaTxFlag= 1;
    

    #ifdef LCD_USE_LVGL
    #ifdef LCD_USE_SPI_DMA 
    if(isSpiSend==1)
    {
      isSpiSend=0;
      lv_display_flush_ready(disp_driver);
      HAL_GPIO_WritePin(SPIx_CS_GPIO_PORT,SPIx_CS_PIN,1); //片选拉高，代表DMA传输完成
    }
    #endif
    #endif
  }
  #endif


  #ifdef SPI_EN_SPI2
  else if(hspi->Instance == SPIx2)
  {
    // 传输完成，可以处理后续任务
    //printf("n%d\n",num++);
    spi2_dmaTxFlag= 1;
  }
  #endif
}