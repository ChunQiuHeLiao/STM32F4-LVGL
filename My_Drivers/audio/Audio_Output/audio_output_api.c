#include"audio_output_api.h"
#include"max98357.h"


//I2S_AUDIOFREQ_xxx 如：I2S_AUDIOFREQ_48K
void Audio_Output_Init(uint32_t sampleRate)
{
    MAX98357_Init(sampleRate);
}


uint8_t Audio_Output_Transmit(uint16_t *rxData, uint16_t size)
{
    return MAX98357_Transmit(rxData,size);
}


/// @brief 等待模块传输完成
/// @return 0：传输完成，其它未完成
uint8_t Audio_Output_WaitTxCplt()
{
    return MAX98357_WaitTxCplt();
}



/*使能音频输出芯片，要播放一定要调用，不使用音频输出后一定要失能，为了省电*/
void Audio_Output_Enable()
{

}


/*失能音频输出*/
void Audio_Output_Disable()
{

}


/*以硬件方式设置音量。取值0~100*/
void Audio_Output_SetVolume(uint8_t volume)
{

}