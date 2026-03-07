#include"audio_output_api.h"
#include"inmp441.h"


//I2S_AUDIOFREQ_xxx 如：I2S_AUDIOFREQ_48K
void Audio_Input_Init(uint32_t sampleRate)
{
    INMP441_Init(sampleRate);
}


uint8_t Audio_Input_Receive(uint16_t *rxData, uint16_t size)
{
    return INMP441_Receive(rxData,size);
}


/// @brief 等待模块接收完成
/// @return 0：传输完成，其它未完成
uint8_t Audio_Input_WaitRxCplt()
{
    return INMP441_WaitRxCplt();
}



/*使能音频输入芯片，要录音一定要调用，不使用一定要失能，为了省电*/
void Audio_Input_Enable()
{

}


/*失能音频输入*/
void Audio_Input_Disable()
{

}


// /*以硬件方式设置音量。取值0~100*/
// void Audio_Input_SetVolume(uint8_t volume)
// {

// }