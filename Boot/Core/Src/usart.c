#include "usart.h"

UART_HandleTypeDef uartHandle;
UART_HandleTypeDef uart2Handle;
UART_HandleTypeDef uart3Handle;
UART_HandleTypeDef uart4Handle;
UART_HandleTypeDef uart5Handle;


uint8_t uart1_receiveBuf[UART1_RX_DATA_SIZE]={0};     //接收的字符缓冲区
uint8_t uart2_receiveBuf[UART2_RX_DATA_SIZE]={0};    //接收的字符缓冲区
uint8_t uart3_receiveBuf[UART3_RX_DATA_SIZE]={0};    //接收的字符缓冲区
uint8_t uart4_receiveBuf[UART4_RX_DATA_SIZE]={0};    //接收的字符缓冲区
uint8_t uart5_receiveBuf[UART5_RX_DATA_SIZE]={0};    //接收的字符缓冲区


//每次接收的数据的大小
uint16_t uart1_rxSize=0;
uint16_t uart2_rxSize=0;
uint16_t uart3_rxSize=0;
uint16_t uart4_rxSize=0;
uint16_t uart5_rxSize=0;


uint8_t uart1_revFlag=0; //第一个串口接收标志，1代表接收到了，0代表还没
uint8_t uart2_revFlag=0; //第二个串口接收标志，1代表接收到了，0代表还没
uint8_t uart3_revFlag=0; //第二个串口接收标志，1代表接收到了，0代表还没
uint8_t uart4_revFlag=0; //第二个串口接收标志，1代表接收到了，0代表还没
uint8_t uart5_revFlag=0; //第二个串口接收标志，1代表接收到了，0代表还没




#ifdef EN_PRINTF

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
  // 阻塞方式打印 -> 串口1
  UART_Transmit_String((uint8_t *)&ch, 1,1);
  return ch;
}

#endif


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USARTx)
  {
    USARTx_CLK_ENABLE();
    USARTx_TX_CLK_ENABLE();
    USARTx_RX_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = USARTx_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USARTx_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USARTx_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(USARTx_RX_PORT, &GPIO_InitStruct);

    //启动串口中断
    HAL_NVIC_SetPriority(USARTx_IRQn,6,0);
    HAL_NVIC_EnableIRQ(USARTx_IRQn);
  }
  
  
  #ifdef  EN_USART2
  if(huart->Instance==USARTx2)
  {
    USARTx2_CLK_ENABLE();
    USARTx2_TX_CLK_ENABLE();
    USARTx2_RX_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = USARTx2_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USARTx2_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USARTx2_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(USARTx2_RX_PORT, &GPIO_InitStruct);

    //启动串口中断
    HAL_NVIC_SetPriority(USARTx2_IRQn,3,3);
    HAL_NVIC_EnableIRQ(USARTx2_IRQn);
  }

  #endif


  #ifdef  EN_USART3
  if(huart->Instance==USARTx3)
  {
    //__HAL_RCC_AFIO_CLK_ENABLE(); //引脚复用使能
    //USARTx3_REMAP_CLK_ENABLE();

    USARTx3_CLK_ENABLE();
    USARTx3_TX_CLK_ENABLE();
    USARTx3_RX_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = USARTx3_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USARTx3_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USARTx3_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(USARTx3_RX_PORT, &GPIO_InitStruct);

    //启动串口中断
    HAL_NVIC_SetPriority(USARTx3_IRQn,3,3);
    HAL_NVIC_EnableIRQ(USARTx3_IRQn);
  }

  #endif


  #ifdef EN_UART4
  if(huart->Instance==UARTx4)
  {
    //__HAL_RCC_AFIO_CLK_ENABLE(); //引脚复用使能
    //UARTx4_REMAP_CLK_ENABLE();
    UARTx4_TX_CLK_ENABLE();
    UARTx4_RX_CLK_ENABLE();
    UARTx4_CLK_ENABLE();

    GPIO_InitStruct.Pin = UARTx4_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(UARTx4_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UARTx4_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(UARTx4_RX_PORT, &GPIO_InitStruct);

    //启动串口中断
    HAL_NVIC_SetPriority(UARTx4_IRQn,3,3);
    HAL_NVIC_EnableIRQ(UARTx4_IRQn);
  }

  #endif


   #ifdef EN_UART5
  if(huart->Instance==UARTx5)
  {
    //__HAL_RCC_AFIO_CLK_ENABLE(); //引脚复用使能
    //UARTx5_REMAP_CLK_ENABLE();
    UARTx5_TX_CLK_ENABLE();
    UARTx5_RX_CLK_ENABLE();
    UARTx5_CLK_ENABLE();

    GPIO_InitStruct.Pin = UARTx5_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(UARTx5_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UARTx5_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(UARTx5_RX_PORT, &GPIO_InitStruct);

    //启动串口中断
    HAL_NVIC_SetPriority(UARTx5_IRQn,3,3);
    HAL_NVIC_EnableIRQ(UARTx5_IRQn);
  }

  #endif
}




void UART_Init(uint32_t baudRate)
{
  uartHandle.Instance = USARTx;
  uartHandle.Init.BaudRate = baudRate;
  uartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  uartHandle.Init.StopBits = UART_STOPBITS_1;
  uartHandle.Init.Parity = UART_PARITY_NONE;
  uartHandle.Init.Mode = UART_MODE_TX_RX; //串口收发
  uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&uartHandle) != HAL_OK)
  {
    Error_Handler();
  }

  //启动中断接收串口数据，每次接收完后需要再次开启
  //UART_Receive_String_IT(uart1_receiveBuf,1);
  UART_ReceiveToIdle_IT(uart1_receiveBuf,sizeof(uart1_receiveBuf),1);
}



uint16_t UART_GetRxDataSize(uint8_t uartId)
{
  if(uartId==1) return uart1_rxSize;
  else if(uartId==2) return uart2_rxSize;
  else if(uartId==3) return uart3_rxSize;
  else if(uartId==4) return uart4_rxSize;
  else if(uartId==5) return uart5_rxSize;
}


//获取接收完成标志
//usartRank:代表是第几个串口。1,2,3...
uint8_t UART_GetRxFlag(uint8_t usartRank)
{
  if(usartRank==1) return uart1_revFlag;
  else if(usartRank==2) return uart2_revFlag;
  else if(usartRank==3) return uart3_revFlag;
  else if(usartRank==4) return uart4_revFlag;
  else if(usartRank==5) return uart5_revFlag;
}




//清除接收完成标志
//usartRank:代表是第几个串口。1,2,3...
void UART_ClearRxFlag(uint8_t usartRank)
{
  if(usartRank==1) 
  {
    uart1_revFlag=0;
  }
  else if(usartRank==2) 
  {
    uart2_revFlag=0;
  }
  else if(usartRank==3) 
  {
    uart3_revFlag=0;
  }
  else if(usartRank==4)
  {
    uart4_revFlag=0;
  } 
  else if(usartRank==5)
  {
    uart5_revFlag=0;
  } 
}

/// @brief 接收数据
/// @param rxData 
/// @param size 数据大小，最大目前30，（可改 ）
/// @param usartId 1,2
/// @return 返回0代表有数据接收，1代表没有
uint8_t UART_GetRxData(uint8_t* rxData,uint16_t size,uint8_t usartId)
{
  if(usartId==1)
  {
    if(UART_GetRxFlag(1)==0) return 1; //没有数据接收
    UART_ClearRxFlag(usartId);
    for(uint16_t i=0;i<size;i++) 
    {
      rxData[i]=uart1_receiveBuf[i];
      uart1_receiveBuf[i]=0x00;
    }
  }
  else if(usartId==2)
  {
    if(UART_GetRxFlag(2)==0) return 1; //没有数据接收
    UART_ClearRxFlag(usartId);
    for(uint16_t i=0;i<size;i++) 
    {
      rxData[i]=uart2_receiveBuf[i];
      uart2_receiveBuf[i]=0x00;
    }
  }
  else if(usartId==3)
  {
    if(UART_GetRxFlag(3)==0) return 1; //没有数据接收
    UART_ClearRxFlag(usartId);
    for(uint16_t i=0;i<size;i++) 
    {
      rxData[i]=uart3_receiveBuf[i];
      uart3_receiveBuf[i]=0x00;
    }
  }
  else if(usartId==4)
  {
    if(UART_GetRxFlag(4)==0) return 1; //没有数据接收
    UART_ClearRxFlag(usartId);
    for(uint16_t i=0;i<size;i++) 
    {
      rxData[i]=uart4_receiveBuf[i];
      uart4_receiveBuf[i]=0x00;
    }
  }
  else if(usartId==5)
  {
    if(UART_GetRxFlag(5)==0) return 1; //没有数据接收
    UART_ClearRxFlag(usartId);
    for(uint16_t i=0;i<size;i++) 
    {
      rxData[i]=uart5_receiveBuf[i];
      uart5_receiveBuf[i]=0x00;
    }
  }

  return 0;
}




/// @brief 接收数据
/// @param rxData 
/// @param size 数据大小，最大目前30，（可改 ）
/// @param usartId 1,2
/// @return 返回0代表有数据接收，1代表没有
// uint8_t UART_GetRxData(uint8_t usartId)
// {
//   if(usartId==1)
//   {
//     if(UART_GetRxFlag(1)==0) return 1; //没有数据接收
//   }
//   else if(usartId==2)
//   {
//     if(UART_GetRxFlag(2)==0) return 1; //没有数据接收
//   }
//   else if(usartId==3)
//   {
//     if(UART_GetRxFlag(3)==0) return 1; //没有数据接收
//   }
//   else if(usartId==4)
//   {
//     if(UART_GetRxFlag(4)==0) return 1; //没有数据接收
//   }
//   else if(usartId==5)
//   {
//     if(UART_GetRxFlag(5)==0) return 1; //没有数据接收
//   }

//   return 0;
// }






/// @brief 发送指定字节的字符串
/// @param uartId 串口编号 1~5,一定要注意，没开启的串口千万不要用，不然会卡死
/// @return 0代表成功，1代表忙，2代表无该串口编号或失败
uint8_t UART_Transmit_String(uint8_t* str,uint16_t size,uint8_t uartId)
{
  if(uartId==1) return HAL_UART_Transmit(&uartHandle,str,size,1000);
  if(uartId==2) return HAL_UART_Transmit(&uart2Handle,str,size,1000);
  if(uartId==3) return HAL_UART_Transmit(&uart3Handle,str,size,1000);
  if(uartId==4) return HAL_UART_Transmit(&uart4Handle,str,size,1000);
  if(uartId==5) return HAL_UART_Transmit(&uart5Handle,str,size,1000);

  return 2;
}

//发送全部字节的字符串。注意可以在后面加上 '\0'
/// @return 0代表成功，1代表忙，2代表无该串口编号或失败
uint8_t UART_Transmit_AllString(uint8_t* str,uint8_t uartId)
{
  uint16_t size=0;
  while (str[size]!='\0')
  {
    size++;
  }
  
  return UART_Transmit_String(str,size,uartId);
}








/**************************************中断发送接收数据部分****************************/



//以中断模式发送全部字节的字符串。注意可以在后面加上 '\0'
/// @return 0代表成功，1代表忙，2代表无该串口编号或失败
uint8_t UART_Transmit_String_IT(uint8_t* str,uint8_t size,uint8_t uartId)
{
  if(uartId==1) return HAL_UART_Transmit_IT(&uartHandle,str,size);
  else if(uartId==2) return HAL_UART_Transmit_IT(&uart2Handle,str,size);
  else if(uartId==3) return HAL_UART_Transmit_IT(&uart3Handle,str,size);
  else if(uartId==4) return HAL_UART_Transmit_IT(&uart4Handle,str,size);
  else if(uartId==5) return HAL_UART_Transmit_IT(&uart5Handle,str,size);

  return 2;
}


/// @return 0代表成功，1代表忙，2代表无该串口编号或失败
uint8_t UART_Receive_String_IT(uint8_t* rxBuf,uint16_t size,uint8_t uartId)
{
  if(uartId==1)  return HAL_UART_Receive_IT(&uartHandle,rxBuf,size);
  else if(uartId==2) return HAL_UART_Receive_IT(&uart2Handle,rxBuf,size);
  else if(uartId==3) return HAL_UART_Receive_IT(&uart3Handle,rxBuf,size);
  else if(uartId==4) return HAL_UART_Receive_IT(&uart4Handle,rxBuf,size);
  else if(uartId==5) return HAL_UART_Receive_IT(&uart5Handle,rxBuf,size);

  return 2;
}



//等待空闲时接收完数据。也就是会自动判断是否对方已经发送完数据。如果发完就会进入回调函数
/// @return 0代表成功，1代表忙，2代表无该串口编号或失败。
uint8_t UART_ReceiveToIdle_IT(uint8_t* rxBuf,uint16_t size,uint8_t uartId)
{
  if(uartId==1) return HAL_UARTEx_ReceiveToIdle_IT(&uartHandle,rxBuf,size);
  if(uartId==2) return HAL_UARTEx_ReceiveToIdle_IT(&uart2Handle,rxBuf,size);
  if(uartId==3) return HAL_UARTEx_ReceiveToIdle_IT(&uart3Handle,rxBuf,size);
  if(uartId==4) return HAL_UARTEx_ReceiveToIdle_IT(&uart4Handle,rxBuf,size);
  if(uartId==5) return HAL_UARTEx_ReceiveToIdle_IT(&uart5Handle,rxBuf,size);

  return 2;
}


void USART1_IRQHandler()
{
  //回自动清除中断标志
  HAL_UART_IRQHandler(&uartHandle);
 
}










/***************************USART2*******************************/

#ifdef  EN_USART2


void UART2_Init(uint32_t baudRate)
{
  uart2Handle.Instance = USARTx2;
  uart2Handle.Init.BaudRate = baudRate;
  uart2Handle.Init.WordLength = UART_WORDLENGTH_8B;
  uart2Handle.Init.StopBits = UART_STOPBITS_1;
  uart2Handle.Init.Parity = UART_PARITY_NONE;
  uart2Handle.Init.Mode = UART_MODE_TX_RX; //串口收发
  uart2Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart2Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&uart2Handle) != HAL_OK)
  {
    Error_Handler();
  }

  //启动中断接收串口数据，每次接收完后需要再次开启
  //UART_Receive_String_IT(uart2_receiveBuf,1); 
  UART_ReceiveToIdle_IT(uart2_receiveBuf,sizeof(uart2_receiveBuf),2);
}


void USART2_IRQHandler()
{
  //回自动清除中断标志
  HAL_UART_IRQHandler(&uart2Handle);
 
}

#endif







/*******************USART3************************** */


#ifdef  EN_USART3

void UART3_Init(uint32_t baudRate)
{
  uart3Handle.Instance = USARTx3;
  uart3Handle.Init.BaudRate = baudRate;
  uart3Handle.Init.WordLength = UART_WORDLENGTH_8B;
  uart3Handle.Init.StopBits = UART_STOPBITS_1;
  uart3Handle.Init.Parity = UART_PARITY_NONE;
  uart3Handle.Init.Mode = UART_MODE_TX_RX; //串口收发
  uart3Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart3Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&uart3Handle) != HAL_OK)
  {
    Error_Handler();
  }
  //启动中断接收串口数据，每次接收完后需要再次开启
  UART_ReceiveToIdle_IT(uart3_receiveBuf,sizeof(uart3_receiveBuf),3);
}


void USART3_IRQHandler()
{
  //回自动清除中断标志
  HAL_UART_IRQHandler(&uart3Handle);
 
}

#endif








/*******************UART4************************** */


#ifdef  EN_UART4

void UART4_Init(uint32_t baudRate)
{
  uart4Handle.Instance = UARTx4;
  uart4Handle.Init.BaudRate = baudRate;
  uart4Handle.Init.WordLength = UART_WORDLENGTH_8B;
  uart4Handle.Init.StopBits = UART_STOPBITS_1;
  uart4Handle.Init.Parity = UART_PARITY_NONE;
  uart4Handle.Init.Mode = UART_MODE_TX_RX; //串口收发
  uart4Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart4Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&uart4Handle) != HAL_OK)
  {
    Error_Handler();
  }

  //启动中断接收串口数据，每次接收完后需要再次开启
  UART_ReceiveToIdle_IT(uart4_receiveBuf,sizeof(uart4_receiveBuf),4);
}


void UART4_IRQHandler()
{
  //回自动清除中断标志
  HAL_UART_IRQHandler(&uart4Handle);
 
}

#endif





/*******************UART5************************** */

#ifdef  EN_UART5

void UART5_Init(uint32_t baudRate)
{
  uart5Handle.Instance = UARTx5;
  uart5Handle.Init.BaudRate = baudRate;
  uart5Handle.Init.WordLength = UART_WORDLENGTH_8B;
  uart5Handle.Init.StopBits = UART_STOPBITS_1;
  uart5Handle.Init.Parity = UART_PARITY_NONE;
  uart5Handle.Init.Mode = UART_MODE_TX_RX; //串口收发
  uart5Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart5Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&uart5Handle) != HAL_OK)
  {
    Error_Handler();
  }

  //启动中断接收串口数据，每次接收完后需要再次开启
  UART_ReceiveToIdle_IT(uart5_receiveBuf,sizeof(uart5_receiveBuf),5);
}


void USART5_IRQHandler()
{
  //回自动清除中断标志
  HAL_UART_IRQHandler(&uart5Handle);
 
}

#endif













/************************回调函数区域********************* */


//发送数据完成回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USARTx)
  {
    printf("uart1 send suc\n");
  }
  // else if(huart->Instance==USARTx2)
  // {
  //   printf("uart2 send suc\n");
  // }
  // else if(huart->Instance==USARTx3)
  // {
  //   printf("uart3 send suc\n");
  // }
  // else if(huart->Instance==UARTx4)
  // {
  //   printf("uart4 send suc\n");
  // }
  // else if(huart->Instance==UARTx5)
  // {
  //   printf("uart5 send suc\n");
  // }
}


// //接收数据完成回调函数
// void HAL_UART_RxCpltCallback (UART_HandleTypeDef* huart)
// {
//   if(huart->Instance==USARTx)
//   {
//     //将接收到的数据发送出去
//     printf("uart1 rev suc\n\r");
//     UART_Receive_String_IT(uart1_receiveBuf,1,1); 
//   }
//   else if(huart->Instance==USARTx2)
//   {
//     //将接收到的数据发送出去
//     printf("uart2 rev suc\n\r");
//     UART_Receive_String_IT(uart1_receiveBuf,1,2); 
//   }  
//   else if(huart->Instance==USARTx3)
//   {
//     //将接收到的数据发送出去
//     printf("uart3 rev suc\n\r");
//     UART_Receive_String_IT(uart1_receiveBuf,1,3); 
//   }  
//   else if(huart->Instance==UARTx4)
//   {
//     //将接收到的数据发送出去
//     printf("uart4 rev suc\n\r");
//     UART_Receive_String_IT(uart1_receiveBuf,1,4); 
//   }  
//   else if(huart->Instance==UARTx5)
//   {
//     //将接收到的数据发送出去
//     printf("uart5 rev suc\n\r");
//     UART_Receive_String_IT(uart1_receiveBuf,1,5); 
//   }  
// }



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if(huart->Instance==USARTx)
  {
    uart1_revFlag=1;
    uart1_rxSize=Size;
    UART_ReceiveToIdle_IT(uart1_receiveBuf,sizeof(uart1_receiveBuf),1);
  }


  #ifdef  EN_USART2
  if(huart->Instance==USARTx2)
  {
    uart2_revFlag=1;
    uart2_rxSize=Size;
    UART_ReceiveToIdle_IT(uart2_receiveBuf,sizeof(uart2_receiveBuf),2);
  }
  #endif


  #ifdef EN_USART3
  if(huart->Instance==USARTx3)
  {
    uart3_revFlag=1;
    uart3_rxSize=Size;
    UART_ReceiveToIdle_IT(uart3_receiveBuf,sizeof(uart3_receiveBuf),3);
  }
  #endif

  #ifdef EN_UART4
  if(huart->Instance==UARTx4)
  {
    uart4_revFlag=1;
    uart4_rxSize=Size;
    UART_ReceiveToIdle_IT(uart4_receiveBuf,sizeof(uart4_receiveBuf),4);
  }
  #endif

  #ifdef EN_UART5
  if(huart->Instance==UARTx5)
  {
    uart5_revFlag=1;
    uart5_rxSize=Size;
    UART_ReceiveToIdle_IT(uart5_receiveBuf,sizeof(uart5_receiveBuf),5);
  }
  #endif
}
