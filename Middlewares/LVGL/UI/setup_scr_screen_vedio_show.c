/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"



lv_obj_t* down_widget = NULL;
lv_obj_t* up_widget = NULL;
lv_obj_t* play_vedio_label = NULL; /*播放视频的标签*/
lv_obj_t* show_vedio_label = NULL; /*显示视频名字的标签*/

uint8_t isPlay = 1; /*1：播放   0：暂停*/
uint8_t isPlayPrev = 0;
uint8_t isPlayNext = 0;

uint8_t isShowWidget = 0;


void Vedio_UI_Init(lv_ui* ui);
void Vedio_ClosePlayWidget();

void setup_scr_screen_vedio_show(lv_ui *ui)
{
    //Write codes screen_vedio_show
    ui->screen_vedio_show = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_vedio_show, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_vedio_show, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_vedio_show, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_vedio_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_vedio_show.
    //Push(&dispHandle, Vedio);
    //Vedio((void*)ui); /*直接运行*/
    dispHandle.capacity++; 
    Vedio_UI_Init(ui);

    //Update current screen layout.
    lv_obj_update_layout(ui->screen_vedio_show);

    lv_label_set_text(show_vedio_label,vedioHandle.name);
}





/*****自定义****/


/*退出按钮事件*/
static void scr_vedio_show_btn_exit_event_handler(lv_event_t* e)
{
    vedioHandle.isExitVedio=1; //退出视频播放
    Screen_SetShowDir(0); /*切为竖屏*/

    if(vedioHandle.openMethod==1)
    {
        ui_load_scr_no_animation(&guider_ui, &guider_ui.screen_vedio, guider_ui.screen_vedio_del, &guider_ui.screen_vedio_show_del, setup_scr_screen_vedio);
    }
    else if(vedioHandle.openMethod==2)
    {
        ui_load_scr_no_animation(&guider_ui, &guider_ui.screen_fe, guider_ui.screen_fe_del, 
            &guider_ui.screen_vedio_show_del, setup_scr_screen_fe);
    }
}


/*主要处理 播放，暂停  播放上下一个视频的事件*/
void Vedio_EventHandler(lv_event_t* e)
{
    const char* str = (const char*)lv_event_get_user_data(e);
    if (strcmp(str, "VedioPlayBtn") == 0) /*播放·视频*/
    {
        if (isPlay == 0) lv_label_set_text(play_vedio_label, LV_SYMBOL_PAUSE);
        else lv_label_set_text(play_vedio_label, LV_SYMBOL_PLAY);
        isPlay = (isPlay == 1) ? 0 : 1;

        Vedio_Set_Play_Flag(isPlay);
    }
    else if (strcmp(str, "VedioPlayUpBtn") == 0) /*播放上一个视频*/
    {
        if(vedioHandle.openMethod==2) return;
        if(vedioHandle.index==0) return;
        vedioHandle.index--;
        vedioHandle.isExitVedio=1;
        vedioHandle.isDispVedio=1;

        Vedio_ClosePlayWidget(); 
        lv_label_set_text(play_vedio_label, LV_SYMBOL_PAUSE);

        ML_GetFileName_By_Index(vedioHandle.index,VEDIO_DIR,vedioHandle.name,sizeof(vedioHandle.name));
        lv_label_set_text(show_vedio_label,vedioHandle.name);

        printf("up vedio\n");
    }
    else if (strcmp(str, "VedioPlayDownBtn") == 0)
    {
        if(vedioHandle.openMethod==2) return;
        if(vedioHandle.index==(vedioHandle.num-1)) return; /*视频到抵了*/
        vedioHandle.index++;
        vedioHandle.isExitVedio=1;
        vedioHandle.isDispVedio=1; /*开始播放新视频*/

        Vedio_ClosePlayWidget();  /*关闭视频上下部分显示的组件*/
        lv_label_set_text(play_vedio_label, LV_SYMBOL_PAUSE); /*将播放按钮设置为暂停，代表在播放*/

        ML_GetFileName_By_Index(vedioHandle.index,VEDIO_DIR,vedioHandle.name,sizeof(vedioHandle.name));
        lv_label_set_text(show_vedio_label,vedioHandle.name);

        printf("down vedio\n");
    }
    else if (strcmp(str, "ClickScreenActive") == 0) /*显示或隐藏 播放栏*/
    {
        if (isShowWidget == 0)
        {
            lv_obj_add_flag(up_widget, LV_OBJ_FLAG_HIDDEN);      /* 隐藏 */
            lv_obj_add_flag(down_widget, LV_OBJ_FLAG_HIDDEN);      /* 隐藏 */
        }
        else if (isShowWidget == 1)
        {
            lv_obj_clear_flag(up_widget, LV_OBJ_FLAG_HIDDEN);    /* 再显示 */
            lv_obj_clear_flag(down_widget, LV_OBJ_FLAG_HIDDEN);    /* 再显示 */
        }
        isShowWidget = (isShowWidget == 0) ? 1 : 0;
    }
    else if(strcmp(str, "slider") == 0)
    {
        printf("slider release\n");
        /*获取视频滑动条组件*/
        lv_obj_t* slider=(lv_obj_t*)lv_event_get_target(e);
        uint16_t slider_value=lv_slider_get_value(slider);
        
        /*修改当前帧率。当前滑动条值/总值*总帧数*/
        vedioHandle.frameIndex=(uint16_t)((float)slider_value/vedioHandle.sliderMaxValue*vedioHandle.allFrame);
        vedioHandle.isSliderReleased=1; /*以便在显示函数里修改文件指针*/
        printf("slider val:%d,new frame index:%d,all frame:%d\n",slider_value,vedioHandle.frameIndex,vedioHandle.allFrame);
    }
}


//关闭播放窗口，也就是全屏，清除屏幕上其它部件
void Vedio_ClosePlayWidget()
{
    isShowWidget = 1;
    lv_obj_add_flag(up_widget, LV_OBJ_FLAG_HIDDEN);      /* 隐藏 */
    lv_obj_add_flag(down_widget, LV_OBJ_FLAG_HIDDEN);      /* 隐藏 */
}

/*自定义的UI*/
void Vedio_UI_Init(lv_ui* ui)
{
    /*为活动屏幕添加事件*/
    lv_obj_t* scr_vedio_show = ui->screen_vedio_show; /*得到该屏幕对象*/
    lv_obj_set_style_bg_color(scr_vedio_show,lv_color_hex(0), 0);
    lv_obj_set_style_bg_opa(scr_vedio_show, 255, 0);

    lv_obj_add_event_cb(scr_vedio_show, Vedio_EventHandler, LV_EVENT_RELEASED, (void*)("ClickScreenActive"));
    const lv_font_t* btn_font = &lv_font_montserrat_16; //上下窗口共用
    /*1. 创建一个视频播放暂停窗口*/
    /*1.1 先创建下边的窗口。里面包含继续/暂停播放按钮，上下视频播放按钮*/
    uint8_t down_btn_width = 0.08 * scr_w;

    down_widget = lv_obj_create(scr_vedio_show);
    lv_obj_set_style_bg_color(down_widget, lv_color_hex(0), 0);
    lv_obj_align(down_widget, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_size(down_widget, scr_w, 0.1 * scr_h);
    lv_obj_set_style_bg_opa(down_widget, 0, 0);
    lv_obj_set_style_radius(down_widget, 0, 0); /*设置部件四边圆角为0*/
    lv_obj_set_scrollbar_mode(down_widget, LV_SCROLLBAR_MODE_OFF); /* 不显示滚动条 */
    lv_obj_clear_flag(down_widget, LV_OBJ_FLAG_SCROLLABLE);        /* 禁止滚动 */
    lv_obj_update_layout(down_widget);   /* 立刻重新计算布局 */


    /*1.2创建上一个视频按钮*/
    lv_obj_t* play_up_vedio_btn = lv_button_create(down_widget);
    //lv_obj_set_pos(play_up_vedio_btn, down_btn_start_pos_x, -(lv_obj_get_height(down_widget) / 2));
    lv_obj_set_size(play_up_vedio_btn, down_btn_width, lv_obj_get_height(down_widget));
    lv_obj_align(play_up_vedio_btn, LV_ALIGN_LEFT_MID, -0.05 * scr_w, 0);
    lv_obj_set_style_shadow_width(play_up_vedio_btn, 0, 0);
    lv_obj_set_style_bg_opa(play_up_vedio_btn, 0, 0); //按钮设置透明

    lv_obj_t* play_up_vedio_label = lv_label_create(play_up_vedio_btn);
    lv_obj_set_align(play_up_vedio_label, LV_ALIGN_CENTER);
    lv_label_set_text(play_up_vedio_label, LV_SYMBOL_PREV);
    lv_obj_set_style_text_font(play_up_vedio_label, &lv_font_montserrat_16, 0);
    lv_obj_add_event_cb(play_up_vedio_btn, Vedio_EventHandler, LV_EVENT_RELEASED, (void*)"VedioPlayUpBtn");


    /*1.3 在里面创建播放按钮*/
    lv_obj_t* play_vedio_btn = lv_button_create(down_widget);
    // lv_obj_set_pos(play_vedio_btn, x,-(lv_obj_get_height(down_widget)/2));
    lv_obj_align(play_vedio_btn, LV_ALIGN_LEFT_MID, 0.05 * scr_w, 0);
    lv_obj_set_size(play_vedio_btn, down_btn_width, lv_obj_get_height(down_widget));
    lv_obj_set_style_shadow_width(play_vedio_btn, 0, 0);
    lv_obj_set_style_bg_opa(play_vedio_btn, 0, 0); //按钮设置透明

    play_vedio_label = lv_label_create(play_vedio_btn);
    lv_obj_set_align(play_vedio_label, LV_ALIGN_CENTER);
    lv_label_set_text(play_vedio_label, LV_SYMBOL_PAUSE);
    lv_obj_set_style_text_font(play_vedio_label, &lv_font_montserrat_16, 0);
    lv_obj_add_event_cb(play_vedio_btn, Vedio_EventHandler, LV_EVENT_RELEASED, (void*)"VedioPlayBtn");


    /*1.4创建下一个视频按钮*/
    lv_obj_t* play_down_vedio_btn = lv_button_create(down_widget);
    //lv_obj_set_pos(play_down_vedio_btn, x, -(lv_obj_get_height(down_widget) / 2));
    lv_obj_align(play_down_vedio_btn, LV_ALIGN_LEFT_MID, 0.15 * scr_w, 0);
    lv_obj_set_size(play_down_vedio_btn, down_btn_width, lv_obj_get_height(down_widget));
    lv_obj_set_style_shadow_width(play_down_vedio_btn, 0, 0);
    lv_obj_set_style_bg_opa(play_down_vedio_btn, 0, 0); //按钮设置透明

    lv_obj_t* play_down_vedio_label = lv_label_create(play_down_vedio_btn);
    lv_obj_set_align(play_down_vedio_label, LV_ALIGN_CENTER);
    lv_label_set_text(play_down_vedio_label, LV_SYMBOL_NEXT);
    lv_obj_set_style_text_font(play_down_vedio_label, &lv_font_montserrat_16, 0);
    lv_obj_add_event_cb(play_down_vedio_btn, Vedio_EventHandler, LV_EVENT_RELEASED, (void*)"VedioPlayDownBtn");



    /*2.1先创建上边边的窗口。里面包含退出视频按钮，当前视频名字标签*/
    uint8_t up_btn_width = 0.08 * scr_w;

    up_widget = lv_obj_create(scr_vedio_show);
    lv_obj_set_style_bg_color(up_widget, lv_color_hex(0), 0);
    //lv_obj_set_pos(up_widget, 0, 0.4 * scr_h);
    lv_obj_align(up_widget, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_size(up_widget, scr_w, 0.1 * scr_h);
    lv_obj_set_style_bg_opa(up_widget, 0, 0);
    lv_obj_set_style_radius(up_widget, 0, 0); /*设置部件四边圆角为0*/
    lv_obj_set_scrollbar_mode(up_widget, LV_SCROLLBAR_MODE_OFF); /* 不显示滚动条 */
    lv_obj_clear_flag(up_widget, LV_OBJ_FLAG_SCROLLABLE);        /* 禁止滚动 */
    lv_obj_update_layout(up_widget);   /* 立刻重新计算布局 */



    /*2.2创建退出按钮*/
    lv_obj_t* exit_btn = lv_button_create(up_widget);
    lv_obj_set_size(exit_btn, up_btn_width, lv_obj_get_height(up_widget));
    lv_obj_align(exit_btn, LV_ALIGN_LEFT_MID, -0.05 * scr_w, 0);
    lv_obj_set_style_bg_color(exit_btn, lv_color_hex(0), 0);
    lv_obj_set_style_shadow_width(exit_btn, 0, 0);
    lv_obj_set_style_bg_opa(exit_btn, 0, 0); //按钮设置透明

    /*2.3创建退出字体符号*/
    lv_obj_t* label = lv_label_create(exit_btn);
    lv_obj_set_style_text_font(exit_btn, btn_font, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, LV_SYMBOL_LEFT);

    
    /*2.4添加事件函数*/
    lv_obj_add_event_cb(exit_btn, scr_vedio_show_btn_exit_event_handler, LV_EVENT_CLICKED, NULL);
    

    /*2.5创建显示 视频名字标签*/
    show_vedio_label = lv_label_create(up_widget);
    lv_obj_set_size(show_vedio_label, 0.8 * scr_w, lv_obj_get_height(up_widget));
    lv_obj_align(show_vedio_label, LV_ALIGN_CENTER, 0, 0.025 * scr_h);

    lv_obj_set_style_text_align(show_vedio_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(show_vedio_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_bg_opa(show_vedio_label, 0, 0);
    lv_label_set_text(show_vedio_label, "ERROR");

    lv_obj_set_style_bg_color(scr_vedio_show, lv_color_hex(0), 0);



    /*生成的*/
    //Write codes screen_album_show_slider
    ui->screen_vedio_show_slider = lv_slider_create(down_widget);
    //lv_obj_set_pos(ui->screen_vedio_show_slider, 0, 251);
    lv_obj_set_size(ui->screen_vedio_show_slider, 220, 4); 
    lv_obj_align(ui->screen_vedio_show_slider,LV_ALIGN_CENTER,40,0);
    lv_slider_set_range(ui->screen_vedio_show_slider, 0, 1000);
    lv_slider_set_mode(ui->screen_vedio_show_slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_vedio_show_slider, 0, LV_ANIM_OFF);

    //Write style for screen_vedio_show_slider, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_vedio_show_slider, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_vedio_show_slider, lv_color_hex(0x7e7e7e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_vedio_show_slider, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_vedio_show_slider, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_vedio_show_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_vedio_show_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_vedio_show_slider, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_vedio_show_slider, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_vedio_show_slider, lv_color_hex(0xff6daf), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_vedio_show_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_vedio_show_slider, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_vedio_show_slider, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_vedio_show_slider, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_vedio_show_slider, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_vedio_show_slider, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_vedio_show_slider, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    /*为视频滑条添加事件*/
    lv_obj_add_event_cb(ui->screen_vedio_show_slider,Vedio_EventHandler,LV_EVENT_RELEASED,"slider");

    Vedio_Handle vh = { 0 };
    vh.down_widget = down_widget;
    vh.up_widget = up_widget;
    vh.play_vedio_label = play_vedio_label;
    vh.show_vedio_label = show_vedio_label;

    Vedio_ClosePlayWidget();
    Vedio_Init(&vh);
}





