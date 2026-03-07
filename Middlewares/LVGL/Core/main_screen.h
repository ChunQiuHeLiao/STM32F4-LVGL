#ifndef __MAIN_SCREEN_H
#define __MAIN_SCREEN_H
#ifdef __cplusplus
extern "C" {
#endif

#include"calendar.h"
#include"file_manager.h"
#include"setting.h"
#include"album.h"
#include"vedio.h"

#include"weather_mid.h"
#include"ai_mid.h"
#include"album_mid.h"
#include"vedio_mid.h"



void MainScreen_Init();
void Flag_Handler();
void App_Clear_ScreenActive_EventCB();
uint8_t App_GetJumpFlag();


#ifdef __cplusplus
}
#endif
#endif
