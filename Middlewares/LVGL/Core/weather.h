#pragma once
#ifndef __WEATHER_H
#define __WEATHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include"Core/main_lib.h"


typedef enum
{
    WEATHER_WIND_DIR_EAST,
    WEATHER_WIND_DIR_SOUTH,
    WEATHER_WIND_DIR_WEST,
    WEATHER_WIND_DIR_NORTH
}WEATHER_WIND_DIR;

typedef struct
{
    char* text; /*本日总体天气。晴，多云 这些*/
    char* lowTemp; /*本日最低温度*/
    char* highTemp; /*本日最高温度*/

    char* rainfall; /*本日降雨量(mm)*/
    char* precip; /*降雨概率*/
    char* windDir; /*风的方向。东南西北*/
    //uint16_t wind_dir_degree; /*风向角度，0~360*/
    char* windSpeed; /*风速 km/h*/
    char* windScale; /*风级*/
    char* humidity; /*相对湿度 0~100*/
}Weather_Info_Day;


/*天气实时信息*/
typedef struct
{
    char* text;
    char* temp;
}Weather_Info_RealTime;

typedef struct
{
    Weather_Info_Day daysInfo[3]; /*今明后三天的整日的天气信息*/
    Weather_Info_RealTime realTimeInfo; /*今日的天气实时信息*/

    uint8_t isReflush : 1; /*是否刷新天气信息*/
}Weather_Handle;


extern Weather_Handle weatherHandle;




uint8_t Weather_UpdateInfo_RealTime(const char* city);
uint8_t Weather_UpdateInfo(const char* city);
void Weather_PrintInfo();

Weather_Handle* Weather_GetHandlePointer();


#ifdef __cplusplus
}
#endif
#endif
