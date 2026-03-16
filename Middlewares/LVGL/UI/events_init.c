/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


static void screen_main_imgbtn_camera_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_camera, guider_ui.screen_camera_del, &guider_ui.screen_main_del, setup_scr_screen_camera, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_main_imgbtn_music_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_music, guider_ui.screen_music_del, &guider_ui.screen_main_del, setup_scr_screen_music, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_main_imgbtn_vedio_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_vedio, guider_ui.screen_vedio_del, &guider_ui.screen_main_del, setup_scr_screen_vedio, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_main_imgbtn_album_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_album, guider_ui.screen_album_del, &guider_ui.screen_main_del, setup_scr_screen_album, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_main_imgbtn_calendar_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_calendar, guider_ui.screen_calendar_del, &guider_ui.screen_main_del, setup_scr_screen_calendar, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_main_imgbtn_weather_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_weather, guider_ui.screen_weather_del, &guider_ui.screen_main_del, setup_scr_screen_weather, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_main_imgbtn_file_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_fe, guider_ui.screen_fe_del, &guider_ui.screen_main_del, setup_scr_screen_fe, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_main_imgbtn_setting_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_setting, guider_ui.screen_setting_del, &guider_ui.screen_main_del, setup_scr_screen_setting, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_main (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_main_imgbtn_camera, screen_main_imgbtn_camera_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_imgbtn_music, screen_main_imgbtn_music_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_imgbtn_vedio, screen_main_imgbtn_vedio_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_imgbtn_album, screen_main_imgbtn_album_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_imgbtn_calendar, screen_main_imgbtn_calendar_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_imgbtn_weather, screen_main_imgbtn_weather_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_imgbtn_file, screen_main_imgbtn_file_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_imgbtn_setting, screen_main_imgbtn_setting_event_handler, LV_EVENT_ALL, ui);
}

static void screen_setting_list_1_item0_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_set_waln, guider_ui.screen_set_waln_del, &guider_ui.screen_setting_del, setup_scr_screen_set_waln, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_list_1_item2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_remove_flag(guider_ui.screen_setting_cont_sd_card, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_list_1_item3_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_setting_sys_update, guider_ui.screen_setting_sys_update_del, &guider_ui.screen_setting_del, setup_scr_screen_setting_sys_update, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_list_1_item4_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_remove_flag(guider_ui.screen_setting_cont_sys_info, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_btn_exit_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        //ui_load_scr_no_animation()
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_setting_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_btn_2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_setting_cont_sys_info, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_btn_exit_sd_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_setting_cont_sd_card, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_setting (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_setting_list_1_item0, screen_setting_list_1_item0_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_list_1_item2, screen_setting_list_1_item2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_list_1_item3, screen_setting_list_1_item3_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_list_1_item4, screen_setting_list_1_item4_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_btn_exit, screen_setting_btn_exit_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_btn_2, screen_setting_btn_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_btn_exit_sd, screen_setting_btn_exit_sd_event_handler, LV_EVENT_ALL, ui);
}

static void screen_setting_sys_update_btn_3_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_remove_flag(guider_ui.screen_setting_sys_update_spinner_update, LV_OBJ_FLAG_HIDDEN);

        /*(逻辑代码) 这里写点击检查更新后的逻辑，包含一下几点
        *1.显示加载框并和服务器建立连接
        *2.建立成功，获取安装包信息。失败则说明失败原因
        *3.开始更新，把安装包数据放入SD卡，如果更新过程中出现问题，给个提示。更新完毕，是否现在就开始安装安装包
        */

        break;
    }
    default:
        break;
    }
}

static void screen_setting_sys_update_btn_2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_remove_flag(guider_ui.screen_setting_sys_update_cont_2, LV_OBJ_FLAG_HIDDEN);
        /*逻辑代码） 更新历史的信息，如什么时候更新的 更新了什么*/
        break;
    }
    default:
        break;
    }
}

static void screen_setting_sys_update_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_setting, guider_ui.screen_setting_del, &guider_ui.screen_setting_sys_update_del, setup_scr_screen_setting, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_sys_update_cont_2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_remove_flag(guider_ui.screen_setting_sys_update_cont_2, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_sys_update_btn_4_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_setting_sys_update_cont_2, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_setting_sys_update (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_setting_sys_update_btn_3, screen_setting_sys_update_btn_3_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_sys_update_btn_2, screen_setting_sys_update_btn_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_sys_update_btn_1, screen_setting_sys_update_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_sys_update_cont_2, screen_setting_sys_update_cont_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_sys_update_btn_4, screen_setting_sys_update_btn_4_event_handler, LV_EVENT_ALL, ui);
}

static void screen_calendar_btn_exit_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_calendar_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_calendar (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_calendar_btn_exit, screen_calendar_btn_exit_event_handler, LV_EVENT_ALL, ui);
}

static void screen_set_waln_btn_flush_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        /*逻辑代码 wifi 刷新按钮，也就是再次发送扫描wifi指令，然后返回数据*/
        break;
    }
    default:
        break;
    }
}

static void screen_set_waln_btn_exit_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_setting, guider_ui.screen_setting_del, &guider_ui.screen_set_waln_del, setup_scr_screen_setting, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        /*记录一下wifi开关状态*/
        break;
    }
    default:
        break;
    }
}

static void screen_set_waln_sw_waln_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;

        switch (status) {
        case (true):
        {
            lv_obj_remove_flag(guider_ui.screen_set_waln_cont_waln_area, LV_OBJ_FLAG_HIDDEN);

            /*逻辑代码 主要用于将搜素WIFI*/
           
            break;
        }
        case (false):
        {
            lv_obj_add_flag(guider_ui.screen_set_waln_cont_waln_area, LV_OBJ_FLAG_HIDDEN);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_set_waln_btn_connect_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        /*逻辑代码 WIFI 输入密码后连接WIFI的代码*/
        lv_obj_remove_flag(guider_ui.screen_set_waln_cont_conn_info, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void screen_set_waln_btn_cancel_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_t* ta_pwd = guider_ui.screen_set_waln_ta_pwd;
        lv_textarea_set_text(ta_pwd, ""); /*清空wifi密码框文本内容*/

        lv_obj_t* btn_pwd_mode = guider_ui.screen_set_waln_btn_pwd_show;
        lv_obj_t* text = lv_obj_get_child(btn_pwd_mode, 0); /*获取密码显示模式的那个文本 眼睛*/
        lv_label_set_text(text, LV_SYMBOL_EYE_OPEN);
        lv_obj_add_flag(guider_ui.screen_set_waln_cont_con_pwd, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void screen_set_waln_btn_pwd_show_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        static bool clickedIndex =true;
        lv_obj_t* input_pwd=guider_ui.screen_set_waln_ta_pwd;

        lv_textarea_set_password_mode(input_pwd,clickedIndex); /*设置密码显示模式*/
        /*获取确定密码模式按钮*/
        lv_obj_t* target=lv_event_get_target(e);
        lv_obj_t* text=lv_obj_get_child(target,0);

        //按下第一次
        if(clickedIndex==true)  lv_label_set_text(text, LV_SYMBOL_EYE_CLOSE);
        else lv_label_set_text(text, LV_SYMBOL_EYE_OPEN);

        clickedIndex=(clickedIndex==true)?false:true;



        break;
    }
    default:
        break;
    }
}

static void screen_set_waln_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_set_waln_cont_conn_info, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_set_waln (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_set_waln_btn_flush, screen_set_waln_btn_flush_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_waln_btn_exit, screen_set_waln_btn_exit_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_waln_sw_waln, screen_set_waln_sw_waln_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_waln_btn_connect, screen_set_waln_btn_connect_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_waln_btn_cancel, screen_set_waln_btn_cancel_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_waln_btn_pwd_show, screen_set_waln_btn_pwd_show_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_waln_btn_1, screen_set_waln_btn_1_event_handler, LV_EVENT_ALL, ui);
}

static void screen_vedio_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_vedio_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_vedio (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_vedio_btn_1, screen_vedio_btn_1_event_handler, LV_EVENT_ALL, ui);
}

static void screen_album_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_album_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_album (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_album_btn_1, screen_album_btn_1_event_handler, LV_EVENT_ALL, ui);
}

static void screen_album_show_btn_exit_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_album, guider_ui.screen_album_del, &guider_ui.screen_album_show_del, setup_scr_screen_album, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_album_show_btn_show_img_info_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        /*逻辑代码 点击后显示当前图片信息，再点击就没了*/
        break;
    }
    default:
        break;
    }
}

void events_init_screen_album_show (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_album_show_btn_exit, screen_album_show_btn_exit_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_album_show_btn_show_img_info, screen_album_show_btn_show_img_info_event_handler, LV_EVENT_ALL, ui);
}

static void screen_music_btn_play_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        /*逻辑代码 音乐 开始播放 暂停*/
        break;
    }
    default:
        break;
    }
}

static void screen_music_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        /*逻辑代码 音乐 下一首*/
        break;
    }
    default:
        break;
    }
}

static void screen_music_btn_prev_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        /*逻辑代码 音乐 上一首*/
        break;
    }
    default:
        break;
    }
}

static void screen_music_btn_exit_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_music_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_music (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_music_btn_play, screen_music_btn_play_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_btn_1, screen_music_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_btn_prev, screen_music_btn_prev_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_btn_exit, screen_music_btn_exit_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
