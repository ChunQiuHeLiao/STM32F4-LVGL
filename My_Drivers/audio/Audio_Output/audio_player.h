/*
用法：
1.先初始化调用 Audio_Player_Init()
2.初始化播放的音频文件 Audio_Player_Play_Init
3.一定要把 Audio_Player_Handler 调用在循环里面，且其它模块运行时间不要太长，不然会影响音质

1.目前只支持双缓存区，所以两个缓存区的都要分配好
2.经过测试 单个缓存区字节数大小/(采样率*2*音频数据字节)*0.3 得到的时间为 播放音频间隔最大时间。也就是你调用 Audio_Player_Handler
后，不要超过该时间再调用Audio_Player_Handler，不然音质会有损失。如缓存区为13200，采样率48k，双声道，16位，则Tmax=13200/(48000*2*2)*0.3=20ms
*/

#ifndef __AUDIO_PLAYER_H
#define __AUDIO_PLAYER_H
#include"main.h"

typedef enum __Audio_Player_Status
{
    AUDIO_STATUS_PLAY=0, /*正在播放*/
    AUDIO_STATUS_PLAY_ERR, /*播放失败*/
    AUDIO_STATUS_PAUSE, /*暂停了*/
    AUDIO_STATUS_NO_START /*都没开始播*/
}Audio_Status;


void Audio_Player_Init(uint32_t sampleRate);
uint8_t Audio_Player_Play_Init(const char* filePath);
Audio_Status Audio_Player_Handler();

void Audio_Player_Play();
void Audio_Player_Pause();
void Audio_Player_SetVolume(uint16_t volume);

uint16_t Audio_Player_GetDuration();
uint16_t Audio_Player_GetPlayedDuration();
#endif