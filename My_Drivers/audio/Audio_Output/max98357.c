#include"max98357.h"
#include"i2s.h"

#define I2S_ID I2S1_ID


void MAX98357_Init(uint32_t sampleRate)
{
    GPIO_InitTypeDef gpio={0};
    gpio.Mode=GPIO_MODE_OUTPUT_PP;
    gpio.Pull=GPIO_PULLUP;
    gpio.Speed=GPIO_SPEED_FREQ_HIGH;
    gpio.Pin=MAX98357_EN_GPIO_PIN;
    
    HAL_GPIO_Init(MAX98357_EN_GPIO_PORT,&gpio);
    MAX98357_SetState(MODULE_ON); /*启用模块*/

    I2S_Init(sampleRate);
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

/*设置音频模块的状态，开还是关*/
void MAX98357_SetState(MODULE_STATE moduleState)
{
    if(moduleState==MODULE_ON) HAL_GPIO_WritePin(MAX98357_EN_GPIO_PORT,MAX98357_EN_GPIO_PIN,1);
    else if(moduleState==MODULE_OFF) HAL_GPIO_WritePin(MAX98357_EN_GPIO_PORT,MAX98357_EN_GPIO_PIN,0);
}