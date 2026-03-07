#ifndef __APP_HANDLES_H
#define __APP_HANDLES_H

#include"main.h"
#include "lvgl.h"

#define AUDIO_RECORDING_DIR "S:/audio_rec"
typedef enum
{
    AUDIO_STATUS_NO_START = 0,
    AUDIO_STATUS_ERR,
    AUDIO_STATUS_RECORD_READY,
    AUDIO_STATUS_PLAY_READY,
    AUDIO_STATUS_RECORDING,
    AUDIO_STATUS_PLAYING,
    AUDIO_STATUS_RECORD_PAUSE, /*录音准备暂停*/
    AUDIO_STATUS_PLAY_PAUSE, /*录音准备暂停*/
    AUDIO_STATUS_OVER,
    AUDIO_STATUS_OVER_TO_READY /*结束后立刻开始播放，一般用于切换播放录音，即关闭当前播放的，然后开始播放新的*/
}Audio_Status;

/*录音句柄*/
typedef struct
{
    uint8_t isInit : 2;
    uint8_t isDeInit : 1;

    uint8_t isClickedPlay : 1; /*是否点击播放按钮*/
    uint8_t isClickedRec : 1; /*是否点击录音按钮*/
    uint8_t isDelete : 1; /*是否删除了录音*/
    char deleteName[16]; /*删除的录音名*/
    uint8_t isAccessFile : 1; /*是否在访问文件，目的是 访问文件和删除文件分开*/

    uint32_t sampleRate;
    uint16_t duration; /*录音总时长*/
    uint16_t playedDuration; /*已经播放的时长*/
    uint16_t recordedDuration; /*已录音的时长*/
    uint8_t volume; /*音量 0~100*/
    uint16_t num; /*录音的个数*/
    uint16_t index; 



    char filePath[48]; /*录音或播放录音文件的路径*/
    char playName[16]; /*播放录音的文件名*/
    char recordName[16]; /*录音的文件名*/
    uint16_t recordNum; /*录音的文件个数，用于给录音文件名做后缀，即 "录音0" "录音1"... */

    Audio_Status status; /*音频的状态*/

    //FIL* fp; /*文件指针*/
}AR_Handle;
extern AR_Handle arHandle;



#endif
