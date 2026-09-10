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


/*软件音量(0~100), 100 为原始音量*/
static uint16_t max98357Volume=100;

uint8_t MAX98357_Transmit(uint16_t *txData, uint16_t size)
{
    /*软件方式设置音量: 直接在发送缓冲上做增益并限幅*/
    if(max98357Volume!=100 && txData!=NULL)
    {
        int16_t* tmp_16=(int16_t*)txData;
        for(uint16_t j=0;j<size;j++)
        {
            int32_t tmp=(int32_t)tmp_16[j];
            tmp=(tmp/(100.0f/max98357Volume)); /*音频放大*/

            /*限幅*/
            if(tmp>32767)      tmp=32767;
            else if(tmp<-32768) tmp=-32768;

            tmp_16[j]=(int16_t)tmp;
        }
    }

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


void MAX98357_SetVolume(uint16_t volume)
{
    if(volume>100) volume=100;
    max98357Volume=volume;
}