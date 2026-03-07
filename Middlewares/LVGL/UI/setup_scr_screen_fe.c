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


void FileManager();
static void FileManager_EventHandler(lv_event_t* e);
static void File_Explorer_EventHandler(lv_event_t* e);

void setup_scr_screen_fe(lv_ui *ui)
{
    //Write codes screen_fe
    ui->screen_fe = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_fe, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_fe, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_fe, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_fe, 0, LV_PART_MAIN|LV_STATE_DEFAULT);


    //The custom code of screen_fe.
    FileManager();
    // Push(&dispHandle,FileManager);
    // appFlag.isAppJump = 1;
    //Update current screen layout.
    lv_obj_update_layout(ui->screen_fe);

    //lv_file_explorer_se
}


/*path:必须是 D: 不要有/ */
void FileManager()
{
    lv_obj_t* screen=guider_ui.screen_fe;
    lv_obj_t* fe = lv_file_explorer_create(screen);

    /*判定是不是通过文件浏览器来打开视频或音频*/
    if(vedioHandle.openMethod==2) 
    {
        vedioHandle.openMethod=1;
        lv_file_explorer_open_dir(fe, vedioHandle.path);
    }
    else if(albumHandle.openMethod==2) 
    {
        albumHandle.openMethod=1;
        lv_file_explorer_open_dir(fe, albumHandle.path);
    }
    else lv_file_explorer_open_dir(fe, DRIVER_DIR);


    /*创建退出按钮*/
    lv_obj_t* btn = lv_button_create(screen);
    
    uint8_t btn_w = 0.15 * scr_w;
    uint8_t btn_h = 0.12 * scr_h;
    //lv_obj_set_pos(btn, 0, 2);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 4);
    lv_obj_set_size(btn, btn_w, btn_h);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_radius(btn, 0, 0);

    /*创建字体标签*/
    lv_obj_t* label = lv_label_create(btn);
    lv_obj_set_style_text_font(btn, LV_FONT_DEFAULT, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0), LV_STATE_DEFAULT);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, LV_SYMBOL_LEFT);

    /*创建退出事件*/
    lv_obj_add_event_cb(btn, File_Explorer_EventHandler, LV_EVENT_CLICKED, NULL);

    /*点击了文件时*/
    lv_obj_add_event_cb(fe, FileManager_EventHandler, LV_EVENT_VALUE_CHANGED, NULL);
}


/*点了退出按钮*/
static void File_Explorer_EventHandler(lv_event_t* e)
{
    /*跳转到主页面*/
    ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_fe_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
}


/*点击了一个文件*/
static void FileManager_EventHandler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        /*获取产生事件的组件*/
        lv_obj_t* file_explorer = (lv_obj_t*)lv_event_get_current_target(e);
        char* filePath=lv_file_explorer_get_current_path(file_explorer);
        char* fileName=lv_file_explorer_get_selected_file_name(file_explorer);

        filePath[strlen(filePath)-1]=0; /*最后一个 / 去掉*/

        printf("clicked path:%s,file:%s\n",filePath,fileName);

        if(Util_IsEndWith(fileName,".bjpg")==true)
        {
            printf("bjpg\n");
            albumHandle.isDispImgFromFe=1;
            strcpy(albumHandle.path,filePath);
            strcpy(albumHandle.name,fileName);

            ui_load_scr_no_animation(&guider_ui, &guider_ui.screen_album_show, guider_ui.screen_album_show_del, &guider_ui.screen_album_del, setup_scr_screen_album_show);
        }
        else if(Util_IsEndWith(fileName,".bmp4")==true)
        {
            printf("bmp4\n");
            Screen_SetShowDir(1); /*切换横屏*/
            vedioHandle.isDispVedioFromFe=1;
            strcpy(vedioHandle.path,filePath);
            strcpy(vedioHandle.name,fileName);

            ui_load_scr_no_animation(&guider_ui, &guider_ui.screen_vedio_show, guider_ui.screen_vedio_show_del, &guider_ui.screen_vedio_del, setup_scr_screen_vedio_show);
        }
    }

}