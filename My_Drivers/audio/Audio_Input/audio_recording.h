#ifndef __AUDIO_RECORDING_H
#define __AUDIO_RECORDING_H
#include"main.h"


typedef enum __Audio_Recording_Status
{
    AR_STATUS_RECORDING=0, /*正在录制*/
    AR_STATUS_ERR, /*录制失败*/
    AR_STATUS_START,
    AR_STATUS_PAUSE, /*录制暂停*/
    AR_STATUS_STOP,
    AR_STATUS_NO_START /*录制未开始,也叫录制结束*/
}AR_Status;


void Audio_Recording_Init(uint32_t sampleRate);
uint8_t Audio_Recording_Start(const char* filePath);
AR_Status Audio_Recording_Handler();

void Audio_Recording_PAUSE();
void Audio_Recording_Continue();
void Audio_Recording_Stop();

uint16_t Audio_Recording_GetDuration();
#endif