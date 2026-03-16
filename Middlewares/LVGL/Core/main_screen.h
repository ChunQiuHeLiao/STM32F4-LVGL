#ifndef __MAIN_SCREEN_H
#define __MAIN_SCREEN_H
#ifdef __cplusplus
extern "C" {
#endif


void MainScreen_Init();
void Flag_Handler();
void App_Clear_ScreenActive_EventCB();
uint8_t App_GetJumpFlag();


#ifdef __cplusplus
}
#endif
#endif
