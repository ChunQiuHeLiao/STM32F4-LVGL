#pragma once

#ifndef __ALBUM_H
#define __ALBUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include"main_lib.h"
typedef struct
{
    uint8_t isInit : 1; /*初始化标志*/
    uint16_t num; //图片个数
    uint16_t index; //图片的当前索引值，从1开始

    uint8_t isDispImg : 1;
    uint8_t isDispImgFromFe : 1; /*从文件浏览器开始播放*/

    uint8_t isSwipeLeft; /*向左滑动图片*/
    uint8_t isSwipeRight; /*向右滑动图片*/
    uint8_t isShowInfo : 1; /*当前是否处于显示图片信息状态*/

    uint8_t name[32]; //当前图片的名字
    char path[32]; /*图片路径。完整的*/
    float size; //KB。保留3位小数 
    uint16_t w;
    uint16_t h;

    uint8_t isExit : 1;/*退出相册*/
    uint8_t openMethod; /*1:相册打开，2文件浏览器打开*/
}Album_Handle;


extern Album_Handle albumHandle;


void Album_SetImgNum(uint16_t num);
void Album_SetImgIndex(uint16_t cur_index);

uint16_t Album_GetImgNum();
uint16_t Album_GetImgIndex();


#ifdef __cplusplus
}
#endif
#endif
