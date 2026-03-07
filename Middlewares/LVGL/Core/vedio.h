#pragma once

#ifndef __VEDIO_H
#define __VEDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include"main_lib.h"
#include"lcd.h"

typedef struct __Vedio_Handle
{
    uint8_t isInit:1; /*初始化*/

    lv_obj_t* down_widget;
    lv_obj_t* up_widget;
    lv_obj_t* play_vedio_label; /*播放视频的标签*/
    lv_obj_t* show_vedio_label; /*播放视频的标签*/

    uint8_t isPlay :1; /*播放 暂停*/
    uint8_t isPlayPrev:1;
    uint8_t isPlayNext:1;

    uint8_t isDispVedio:1; /*是否开始播放视频*/
    uint8_t isDispVedioFromFe:1;  /*从文件浏览器里播放视频*/
    uint8_t isExitVedio:1; /*是否退出视频*/
    uint8_t isDispOver:1; /*播放完毕*/

    uint16_t index; /*当前视频的索引值*/
    uint16_t num; /*总视频个数*/
    char name[32]; //当前视频的名字
    char path[32]; /*视频路径。完整的*/
    float size; //MB。保留3位小数 

    uint16_t allFrame; /*视频帧数*/
    uint16_t frameIndex;

    uint16_t sliderMaxValue;
    uint8_t isSliderReleased;

    uint8_t openMethod; /*1:相册打开，2：文件浏览器打开*/
}Vedio_Handle;

extern Vedio_Handle vedioHandle;

uint8_t Vedio_GetPlayFlag();
uint8_t Vedio_Get_PlayPre_Flag();
uint8_t Vedio_Get_PlayNext_Flag();

void Vedio_Set_Play_Flag(uint8_t flag);
void Vedio_Set_PlayPre_Flag(uint8_t flag);
void Vedio_Set_PlayNext_Flag(uint8_t flag);


void Vedio_SetPlayVedioName(const char* name);
void Vedio_Clear_ScrAct_Event();

uint8_t Vedio_Disp(const char* filePath,FrameFunc frameFunc);

void Vedio_EventHandler(lv_event_t* e);

void Vedio_Init(Vedio_Handle* vh);

void Vedio_SetShowDir(uint8_t dir);
void Vedio_Handler();
#ifdef __cplusplus
}
#endif
#endif
