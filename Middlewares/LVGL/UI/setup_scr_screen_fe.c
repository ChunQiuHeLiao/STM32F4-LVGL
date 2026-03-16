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



static void FileManager_EventHandler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        /*获取产生事件的组件*/
        lv_obj_t* file_explorer = (lv_obj_t*)lv_event_get_current_target(e);

        printf("clicked file:%s\n", lv_file_explorer_get_selected_file_name(file_explorer));
    }
}


void setup_scr_screen_fe(lv_ui *ui)
{
    //Write codes screen_fe
    printf("enter fe\n");
    ui->screen_fe = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_fe, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_fe, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_fe, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_fe, 0, LV_PART_MAIN|LV_STATE_DEFAULT);


    //The custom code of screen_fe.
    
    lv_obj_t* fe = lv_file_explorer_create(ui->screen_fe);
    lv_file_explorer_open_dir(fe, DRIVER_DIR);

    /*添加退出按钮*/
    App_CreateExitBtn(ui->screen_fe);
    /*点击了文件时*/
    lv_obj_add_event_cb(fe, FileManager_EventHandler, LV_EVENT_VALUE_CHANGED, NULL);

    //Update current screen layout.
    lv_obj_update_layout(ui->screen_fe);

}
