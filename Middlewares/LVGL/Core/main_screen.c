#include"Core/main_lib.h"

#include"ex/weather_mid.h"
#include "ex/music_mid.h"
#include "ex/album_mid.h"
#include "ex/vedio_mid.h"
#include"ex/audio_rec_mid.h"

#if LV_USE_GUIDER_SIMULATOR==0
#include"gui_guider.h"
#else
#include"UI/src/gui_guider.h"
#endif
static void MainScreen_EventHandler(lv_event_t* e);




lv_color_t uiDefaultColor = { 0 };

void MainScreen_Init()
{
    /*uiDefaultColor = SREEN_COLOER_WHITE;
    StackInit(&dispHandle);
    dispHandle.capacity++;*/
    //Push(&dispHandle, MainScreen);
    ////MainScreen();
    //dispHandle.dispFunc[dispHandle.capacity-1]();

   // Weather_Mid_Init();
    //Music_Mid_Init();
    
}



void MainScreen()
{

    uint8_t icon_w = 40;
    uint8_t icon_h = 40;
    uint16_t icon_x = 0;
    uint16_t icon_y = 0;
    uint8_t iconSpacing_col = 20; //列间距
    uint8_t iconSpacing_row = (scr_w-4*icon_w)/5; //行间距

    lv_obj_t* imgbtn = lv_imagebutton_create(lv_screen_active());
    lv_obj_t* imgbtn2 = lv_imagebutton_create(lv_screen_active());
    lv_obj_t* imgbtn3 = lv_imagebutton_create(lv_screen_active());
    lv_obj_t* imgbtn4 = lv_imagebutton_create(lv_screen_active());
    lv_obj_t* imgbtn5 = lv_imagebutton_create(lv_screen_active());
    lv_obj_t* imgbtn6 = lv_imagebutton_create(lv_screen_active());
    lv_obj_t* imgbtn7 = lv_imagebutton_create(lv_screen_active());
    lv_obj_t* imgbtn8 = lv_imagebutton_create(lv_screen_active());


    lv_imagebutton_set_src(imgbtn,  LV_IMAGEBUTTON_STATE_RELEASED, NULL, "S:img/setting.jpg", NULL);
    lv_obj_set_size(imgbtn, icon_w, icon_h);
    lv_imagebutton_set_src(imgbtn2, LV_IMAGEBUTTON_STATE_RELEASED, NULL, "S:img/music.jpg", NULL);
    lv_obj_set_size(imgbtn2, icon_w, icon_h);
    lv_imagebutton_set_src(imgbtn3, LV_IMAGEBUTTON_STATE_RELEASED, NULL, "S:img/vedio.jpg", NULL);
    lv_obj_set_size(imgbtn3, icon_w, icon_h);
    lv_imagebutton_set_src(imgbtn4, LV_IMAGEBUTTON_STATE_RELEASED, NULL, "S:img/pic.jpg", NULL);
    lv_obj_set_size(imgbtn4, icon_w, icon_h);
    lv_imagebutton_set_src(imgbtn5, LV_IMAGEBUTTON_STATE_RELEASED, NULL, "S:img/weather.jpg", NULL);
    lv_obj_set_size(imgbtn5, icon_w, icon_h);
    lv_imagebutton_set_src(imgbtn6, LV_IMAGEBUTTON_STATE_RELEASED, NULL, "S:img/calender.jpg", NULL);
    lv_obj_set_size(imgbtn6, icon_w, icon_h);
    lv_imagebutton_set_src(imgbtn7, LV_IMAGEBUTTON_STATE_RELEASED, NULL, "S:img/camera.jpg", NULL);
    lv_obj_set_size(imgbtn7, icon_w, icon_h);
    lv_imagebutton_set_src(imgbtn8, LV_IMAGEBUTTON_STATE_RELEASED, NULL, "S:img/file_manager.jpg", NULL);
    lv_obj_set_size(imgbtn8, icon_w, icon_h);


    icon_x = iconSpacing_row;
    icon_y = iconSpacing_col;
    lv_obj_set_pos(imgbtn, icon_x, icon_y);
    icon_x += iconSpacing_row + icon_w;
    lv_obj_set_pos(imgbtn2, icon_x, icon_y);
    icon_x += iconSpacing_row + icon_w;
    lv_obj_set_pos(imgbtn3, icon_x, icon_y);
    icon_x += iconSpacing_row + icon_w;
    lv_obj_set_pos(imgbtn4, icon_x, icon_y);


    icon_x = iconSpacing_row;
    icon_y += iconSpacing_col + icon_h;
    lv_obj_set_pos(imgbtn5, icon_x, icon_y);
    icon_x += iconSpacing_row + icon_w;
    lv_obj_set_pos(imgbtn6, icon_x, icon_y);
    icon_x += iconSpacing_row + icon_w;
    lv_obj_set_pos(imgbtn7, icon_x, icon_y);
    icon_x += iconSpacing_row + icon_w;
    lv_obj_set_pos(imgbtn8, icon_x, icon_y);


    //图片按钮事件回调函数
    lv_obj_add_event_cb(imgbtn,  MainScreen_EventHandler, LV_EVENT_RELEASED, (void*)"setting");
    lv_obj_add_event_cb(imgbtn2, MainScreen_EventHandler, LV_EVENT_RELEASED, (void*)"music");
    lv_obj_add_event_cb(imgbtn3, MainScreen_EventHandler, LV_EVENT_RELEASED, (void*)"vedio");
    lv_obj_add_event_cb(imgbtn4, MainScreen_EventHandler, LV_EVENT_RELEASED, (void*)"album");
    lv_obj_add_event_cb(imgbtn5, MainScreen_EventHandler, LV_EVENT_RELEASED, (void*)"weather");
    lv_obj_add_event_cb(imgbtn6, MainScreen_EventHandler, LV_EVENT_RELEASED, (void*)"calendar");
    lv_obj_add_event_cb(imgbtn7, MainScreen_EventHandler, LV_EVENT_RELEASED, (void*)"camera");
    lv_obj_add_event_cb(imgbtn8, MainScreen_EventHandler, LV_EVENT_RELEASED, (void*)"file_manager");
}




//App启动事件函数
static void MainScreen_EventHandler(lv_event_t* e)
{
    //lv_obj_t* tmp=lv_event_get_target(e);
    //const char* para = (const char*)lv_event_get_user_data(e);
    //if (strcmp(para, "setting") == 0)
    //{
    //    printf("setting\n");
    //    Push(&dispHandle, Setting);
    //    appFlag.isAppJump = 1;
    //}
    //else if (strcmp(para,"music")==0)
    //{
    //    printf("music\n");

    //}
    //else if (strcmp(para,"vedio")==0)
    //{
    //    printf("vedio\n");
    //    Push(&dispHandle, Vedio);
    //    appFlag.isAppJump = 1;
    //}
    //else if (strcmp(para,"album")==0)
    //{
    //    printf("album\n");
    //    Push(&dispHandle, Album);
    //    appFlag.isAppJump = 1;
    //}
    //else if (strcmp(para,"weather")==0)
    //{
    //    printf("weather\n");
    //}
    //else if (strcmp(para,"calendar")==0)
    //{
    //    printf("calendar\n");
    //    Push(&dispHandle, Calendar);
    //    appFlag.isAppJump = 1;
    //}
    //else if (strcmp(para,"camera")==0)
    //{
    //    printf("camera\n");
    //    //Push(&dispHandle, Camera);
    //    //appFlag.isAppJump = 1;
    //}
    //else if (strcmp(para,"file_manager")==0)
    //{
    //    printf("file_manager\n");
    //    Push(&dispHandle, FileManager);
    //    appFlag.isAppJump = 1;
    //    printf("over\n");
    //}
}




/*专门用于退出事件函数*/
void AppExit_EventHandler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        appFlag.isAppJump = 1; /*表示要跳转页面*/
        strcpy(switchTaskName, "");
        Pop(&dispHandle);

        if(appFlag.isEnter_vedio==1) 
        {
            lv_obj_set_style_bg_color(lv_screen_active(),lv_color_hex(0xffffff),0);
            //Screen_SetShowDir(0); //退出视频播放器，切换为竖屏
        }
    }
}




void Flag_Handler()
{

    Weather_Mid_Handler();
    Music_Mid_Handler();
    AI_MID_Handler();
    Album_MID_Handler();
    Vedio_Mid_Handler();
    Audio_Rec_Mid_Handler();
    //if (appFlag.isAppJump == 1 || appFlag.isPageChange == 1)
    //{
    //    appFlag.isAppJump = 0;
    //    if(appFlag.isPageChange==1) appFlag.isPageChange = 0;
    //    else lv_obj_clean(lv_screen_active());

    //    App_Clear_ScreenActive_EventCB();

    //    if (dispHandle.capacity == 1)
    //    {
    //        setup_scr_screen_main(&guider_ui);
    //        lv_screen_load(guider_ui.screen_main);
    //    }
    //    else dispHandle.dispFunc[dispHandle.capacity - 1](); /*显示*/
    //}
    //
    //return;
}

uint8_t App_GetJumpFlag()
{
    return appFlag.isAppJump;
}


/*创建退出按钮*/
/*
*parent:为NULL时，父类为活动屏幕
*/
lv_obj_t* App_CreateExitBtn(lv_obj_t* parent)
{
    /*创建退出按钮*/
    lv_obj_t* btn = NULL;
    if(parent == NULL) btn = lv_button_create(lv_screen_active());
    else btn= lv_button_create(parent);
    uint8_t btn_w = 0.15 * scr_w;
    uint8_t btn_h = 0.12 * scr_h;
    //lv_obj_set_pos(btn, 0, 2);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 4);
    lv_obj_set_size(btn, btn_w, btn_h);
    lv_obj_set_style_bg_color(btn, uiDefaultColor, 0);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_radius(btn, 0, 0);

    /*创建字体标签*/
    lv_obj_t* label = lv_label_create(btn);
    lv_obj_set_style_text_font(btn, LV_FONT_DEFAULT, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0), LV_STATE_DEFAULT);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, LV_SYMBOL_LEFT);

    /*创建退出事件*/
    lv_obj_add_event_cb(btn, AppExit_EventHandler, LV_EVENT_CLICKED, NULL);

    return btn;
}


/*设置页面标签的各种属性。该页面标签需要以列表为形式的才行*/
void App_SetPageTitle(lv_obj_t* label)
{
    /*将第一个组件的文本居中*/
    lv_obj_set_pos(label, 0, 0);
    lv_obj_set_size(label, scr_w, 0.14 * scr_h);/*设置文本标签高等*/
    lv_obj_set_style_bg_color(label, uiDefaultColor, 0); /*设置标签背景颜色*/
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0); /*设置字体大小*/
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); /*这里不能用lv_obj_set_align 因为这个是对组件的，而不是文本的*/

}



//清除活动屏幕的事件回调函数
void App_Clear_ScreenActive_EventCB()
{
    /*Vedio_Clear_ScrAct_Event();
    Album_Clear_ScrAct_Event();*/
}


