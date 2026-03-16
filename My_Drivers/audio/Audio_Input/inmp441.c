#include"inmp441.h"
#include"i2s.h"

#define I2S_ID I2S2_ID

void INMP441_Init(uint32_t sampleRate)
{
    I2S2_Init(sampleRate);
}

uint8_t INMP441_Receive(uint16_t *rxData, uint16_t size)
{
    return I2S_Receive_DMA(rxData,size,I2S_ID);
}

uint8_t INMP441_WaitRxCplt()
{
   return I2S_WaitRxCplt(I2S_ID);
}