#ifndef __MAIN_LIB_H
#define __MAIN_LIB_H
#include"wifi_app.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "Core/main_screen.h"

#include "Core/album.h"
#include "Core/calendar.h"
#include "Core/file_manager.h"
#include "Core/setting.h"
#include "Core/music.h"
#include "Core/weather.h"



#define LV_USE_GUIDER_SIMULATOR 0 //是否在电脑上仿真 0则是代表要烧录的


#if LV_USE_GUIDER_SIMULATOR==0
#include"main.h"
#include"lcd.h"

#include"lv_port_disp_template.h"
#include"lv_api_map_v8.h"
#include"lv_draw_private.h"


//图形化工具
#include"gui_guider.h"

#define DRIVER_DIR "S:" //盘符 SD卡的话就"F"
#define VEDIO_DIR "S:/vedio" /*视频路径*/
#define IMG_DIR "S:/img" /*视频路径*/
#define MUSIC_DIR "S:/music"
#define AUDIO_RECORDING_DIR "S:/audio_rec"
#else
#include"src/draw/lv_draw_private.h"

#include"UI/src/gui_guider.h"

#define DRIVER_DIR "D:" //盘符 SD卡的话就"S
#define VEDIO_DIR "D:/vedio" /*视频路径*/
#define IMG_DIR "D:/img" /*视频路径*/
#endif

// #define  uint8_t unsigned char
// typedef unsigned short uint16_t;








#define SCR_WIDTH 240
#define SCR_HEIGHT 320

extern uint16_t scr_w;
extern uint16_t scr_h;

#define TITLE_HEIGHT (0.14*scr_h)


// #define lv_font_montserratMedium_12 lv_font_montserrat_12
// #define lv_font_montserratMedium_16 lv_font_montserrat_16
// #define lv_font_SourceHanSerifSC_Regular_18 lv_font_montserrat_16


LV_FONT_DECLARE(lv_font_source_han_siyuan_songti_36)
LV_FONT_DECLARE(ui_font_show_14)

#define lv_font_montserratMedium_12 lv_font_montserrat_12
#define lv_font_montserratMedium_16 ui_font_show_14
#define lv_font_SourceHanSerifSC_Regular_18 ui_font_show_14
#define lv_font_montserrat_16 ui_font_show_14
#define lv_font_montserrat_36 lv_font_source_han_siyuan_songti_36
#define lv_font_montserrat_14 ui_font_show_14






/*下面两个主要为颜色反转做准备*/
#define SREEN_COLOER_WHITE lv_color_hex(0xffffff) /*界面默认颜色为白色*/
#define SREEN_COLOR_BLACK lv_color_hex(0) /*界面默认颜色为黑色*/


typedef struct __AppFlag
{  /* int isEnter_setting;
    int isEnter_music;*/
    uint8_t isEnter_vedio;
    /*int isEnter_pic;
    int isEnter_weather;
    int isEnter_calender;
    int isEnter_camera;
    int isEnter_fileManager;
    
    int isExitApp;*/
    uint8_t isSwitchShowDir;
    uint8_t isAppJump; //判断App是否要跳转
    uint8_t isPageChange; /*判断页面是否更改*/

    uint8_t isDispImg:1;


    //int isExit_setting;
    //int isExit_music;
    //int isExit_vedio;
    //int isExit_pic;
    //int isExit_weather;
    //int isExit_calender;
    //int isExit_camera;
    //int isExit_fileManager;
}AppFlag;



typedef void(*DispFunc)(void); //函数指针
typedef struct
{
    DispFunc dispFunc[8];  /*显示函数，用队列存起来进入的页面*/
    int capacity; /*记录当前存的函数个数*/
}DispHandle;



extern DispHandle dispHandle;
extern AppFlag appFlag;

extern int screenColor;
extern char switchTaskName[64];




void MainScreen();
void Setting();

void Calendar();
void Album();
void Vedio();


void FileManager();



lv_obj_t* App_CreateExitBtn(lv_obj_t* parent);
void App_SetPageTitle(lv_obj_t* label);


void AppExit_EventHandler(lv_event_t* e);


void StackInit(DispHandle* dispHandle);
void Push(DispHandle* dispHandle, DispFunc func);
void Pop(DispHandle* dispHandle);
bool Util_IsEndWith(const char* str1, const char* str2);

#if LV_USE_GUIDER_SIMULATOR==0
void Screen_SetShowDir(uint8_t dir);
#endif


/******自定义函数************/

uint8_t ML_GetFileName_By_Index(uint16_t index, const char* filePath,char* fileName, uint16_t size);
uint8_t ML_GetFilePath_By_Index(uint16_t index, const char* filePath);


char* MyStrCpy(char** str, uint8_t size, char* value);

void ML_RefreshUI();
void print_lvgl_mem_usage(void);

uint8_t lv_fs_delete(const char* filePath);
#ifdef __cplusplus
}
#endif
#endif

