#include"max98357.h"
#include"i2s.h"

#define I2S_ID I2S2_ID


void MAX98357_Init(uint32_t sampleRate)
{
    I2S2_Init(sampleRate);
}


uint8_t MAX98357_Transmit(uint16_t *txData, uint16_t size)
{
    return I2S_Transmit_DMA(txData,size,I2S_ID);
}

/// @brief 等待模块传输完成
/// @return 0：传输完成，其它未完成
uint8_t MAX98357_WaitTxCplt()
{
    return I2S_WaitTxCplt(I2S_ID);
}
