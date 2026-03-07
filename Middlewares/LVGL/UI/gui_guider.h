/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


#include"main_lib.h"
#if LV_USE_GUIDER_SIMULATOR==0
#include"main_screen.h"
#else
#include"App/main_screen.h"
#endif
    
typedef struct
{
  
	lv_obj_t *screen_main;
	bool screen_main_del;
	lv_obj_t *screen_main_cont_app;
	lv_obj_t *screen_main_imgbtn_camera;
	lv_obj_t *screen_main_imgbtn_camera_label;
	lv_obj_t *screen_main_imgbtn_music;
	lv_obj_t *screen_main_imgbtn_music_label;
	lv_obj_t *screen_main_imgbtn_vedio;
	lv_obj_t *screen_main_imgbtn_vedio_label;
	lv_obj_t *screen_main_imgbtn_album;
	lv_obj_t *screen_main_imgbtn_album_label;
	lv_obj_t *screen_main_imgbtn_calendar;
	lv_obj_t *screen_main_imgbtn_calendar_label;
	lv_obj_t *screen_main_imgbtn_weather;
	lv_obj_t *screen_main_imgbtn_weather_label;
	lv_obj_t *screen_main_imgbtn_file;
	lv_obj_t *screen_main_imgbtn_file_label;
	lv_obj_t *screen_main_imgbtn_setting;
	lv_obj_t *screen_main_imgbtn_setting_label;
	lv_obj_t *screen_main_label_1;
	lv_obj_t *screen_main_label_2;
	lv_obj_t *screen_main_label_3;
	lv_obj_t *screen_main_label_4;
	lv_obj_t *screen_main_label_8;
	lv_obj_t *screen_main_label_7;
	lv_obj_t *screen_main_label_6;
	lv_obj_t *screen_main_label_5;
	lv_obj_t *screen_main_cont_icon;
	lv_obj_t *screen_setting;
	bool screen_setting_del;
	lv_obj_t *screen_setting_cont_setting;
	lv_obj_t *screen_setting_list_1;
	lv_obj_t *screen_setting_list_1_item0;
	lv_obj_t *screen_setting_list_1_item1;
	lv_obj_t *screen_setting_list_1_item2;
	lv_obj_t *screen_setting_list_1_item3;
	lv_obj_t *screen_setting_list_1_item4;
	lv_obj_t *screen_setting_btn_exit;
	lv_obj_t *screen_setting_btn_exit_label;
	lv_obj_t *screen_setting_label_1;
	lv_obj_t *screen_setting_cont_sys_info;
	lv_obj_t *screen_setting_btn_2;
	lv_obj_t *screen_setting_btn_2_label;
	lv_obj_t *screen_setting_label_3;
	lv_obj_t *screen_setting_spangroup_2;
	lv_span_t *screen_setting_spangroup_2_span;
	lv_obj_t *screen_setting_cont_sd_card;
	lv_obj_t *screen_setting_btn_exit_sd;
	lv_obj_t *screen_setting_btn_exit_sd_label;
	lv_obj_t *screen_setting_title_sd;
	lv_obj_t *screen_setting_area_sd;
	lv_span_t *screen_setting_area_sd_span;
	lv_obj_t *screen_setting_sys_update;
	bool screen_setting_sys_update_del;
lv_obj_t *screen_setting_sys_update_cont_main;
	lv_obj_t *screen_setting_sys_update_btn_3;
	lv_obj_t *screen_setting_sys_update_btn_3_label;
	lv_obj_t *screen_setting_sys_update_btn_2;
	lv_obj_t *screen_setting_sys_update_btn_2_label;
	lv_obj_t *screen_setting_sys_update_btn_1;
	lv_obj_t *screen_setting_sys_update_btn_1_label;
	lv_obj_t *screen_setting_sys_update_label_1;
	lv_obj_t *screen_setting_sys_update_cont_2;
	lv_obj_t *screen_setting_sys_update_btn_4;
	lv_obj_t *screen_setting_sys_update_btn_4_label;
	lv_obj_t *screen_setting_sys_update_label_2;
	lv_obj_t *screen_setting_sys_update_spangroup_1;
	lv_span_t *screen_setting_sys_update_spangroup_1_span;
	lv_obj_t *screen_setting_sys_update_mask;
	lv_obj_t *screen_setting_sys_update_spinner_update;
	lv_obj_t *screen_setting_sys_update_cont_info_box;
	lv_obj_t *screen_setting_sys_update_btn_info_yes;
	lv_obj_t *screen_setting_sys_update_btn_info_yes_label;
	lv_obj_t *screen_setting_sys_update_label_info;
	lv_obj_t *screen_setting_sys_update_btn_info_no;
	lv_obj_t *screen_setting_sys_update_btn_info_no_label;
	lv_obj_t *screen_setting_sys_update_cont_updating;
	lv_obj_t *screen_setting_sys_update_bar_updating;
	lv_obj_t *screen_setting_sys_update_label_updating;
	lv_obj_t *screen_calendar;
	bool screen_calendar_del;
	lv_obj_t *screen_calendar_calendar;
	lv_obj_t *screen_calendar_btn_exit;
	lv_obj_t *screen_calendar_btn_exit_label;
	lv_obj_t *screen_set_waln;
	bool screen_set_waln_del;
lv_obj_t *screen_set_waln_cont_waln_area;
	lv_obj_t *screen_set_waln_btn_flush;
	lv_obj_t *screen_set_waln_btn_flush_label;
	lv_obj_t *screen_set_waln_label_1;
	lv_obj_t *screen_set_waln_cont_up;
	lv_obj_t *screen_set_waln_label;
	lv_obj_t *screen_set_waln_label_title;
	lv_obj_t *screen_set_waln_btn_exit;
	lv_obj_t *screen_set_waln_btn_exit_label;
	lv_obj_t *screen_set_waln_sw_waln;
	lv_obj_t *screen_set_waln_cont_con_pwd;
	lv_obj_t *screen_set_waln_cont_title;
	lv_obj_t *screen_set_waln_btn_connect;
	lv_obj_t *screen_set_waln_btn_connect_label;
	lv_obj_t *screen_set_waln_btn_cancel;
	lv_obj_t *screen_set_waln_btn_cancel_label;
	lv_obj_t *screen_set_waln_label_titl;
	lv_obj_t *screen_set_waln_cont_inp_pwd;
	lv_obj_t *screen_set_waln_btn_pwd_show;
	lv_obj_t *screen_set_waln_btn_pwd_show_label;
	lv_obj_t *screen_set_waln_label_pwd;
	lv_obj_t *screen_set_waln_ta_pwd;
	lv_obj_t *screen_set_waln_mask;
	lv_obj_t *screen_set_waln_spinner_conn_wifi;
	lv_obj_t *screen_set_waln_cont_conn_info;
	lv_obj_t *screen_set_waln_textarea_con_info;
	lv_span_t *screen_set_waln_textarea_con_info_span;
	lv_obj_t *screen_set_waln_btn_1;
	lv_obj_t *screen_set_waln_btn_1_label;
	lv_obj_t *screen_vedio;
	bool screen_vedio_del;
	lv_obj_t *screen_vedio_cont_title;
	lv_obj_t *screen_vedio_btn_1;
	lv_obj_t *screen_vedio_btn_1_label;
	lv_obj_t *screen_vedio_label_1;
	lv_obj_t *screen_vedio_list_vedio;
	lv_obj_t* screen_vedio_show_slider;
	lv_obj_t *screen_album;
	bool screen_album_del;
	lv_obj_t *screen_album_btn_1;
	lv_obj_t *screen_album_btn_1_label;
	lv_obj_t *screen_album_label_1;
	lv_obj_t *screen_album_list_album;
	lv_obj_t *screen_album_show;
	bool screen_album_show_del;
	lv_obj_t *screen_album_show_cont_img_show;
	lv_obj_t *screen_album_show_cont_title;
	lv_obj_t *screen_album_show_btn_exit;
	lv_obj_t *screen_album_show_btn_exit_label;
	lv_obj_t *screen_album_show_label_img_name;
	lv_obj_t *screen_album_show_btn_show_img_info;
	lv_obj_t *screen_album_show_btn_show_img_info_label;
	lv_obj_t *screen_album_show_cont_img_info;
	lv_obj_t *screen_album_show_label_img_info;
	lv_obj_t *screen_music;
	bool screen_music_del;
	lv_obj_t *screen_music_cont_main;
	lv_obj_t *screen_music_list_music;
	lv_obj_t *screen_music_list_music_item0;
	lv_obj_t *screen_music_cont_play_area;
	lv_obj_t *screen_music_btn_play;
	lv_obj_t *screen_music_btn_play_label;
	lv_obj_t *screen_music_label_music_name;
	lv_obj_t *screen_music_btn_1;
	lv_obj_t *screen_music_btn_1_label;
	lv_obj_t *screen_music_btn_prev;
	lv_obj_t *screen_music_btn_prev_label;
	lv_obj_t *screen_music_btn_show_list;
	lv_obj_t *screen_music_btn_show_list_label;
	lv_obj_t *screen_music_cont_title;
	lv_obj_t *screen_music_btn_exit;
	lv_obj_t *screen_music_btn_exit_label;
	lv_obj_t *screen_music_label_tiltle;
	lv_obj_t *screen_fe;
	bool screen_fe_del;
	lv_obj_t *screen_weather;
	bool screen_weather_del;
	lv_obj_t *screen_camera;
	bool screen_camera_del;

    lv_obj_t* screen_vedio_show;
    bool screen_vedio_show_del;

	lv_obj_t *g_kb_top_layer;
}lv_ui;






typedef void (*ui_setup_scr_t)(lv_ui * ui);

/*自定义代码*/
void ui_load_scr_no_animation(lv_ui* ui, lv_obj_t** new_scr, bool new_scr_del, bool* old_scr_del, ui_setup_scr_t setup_scr);



void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen_main(lv_ui *ui);
void setup_scr_screen_setting(lv_ui *ui);
void setup_scr_screen_setting_sys_update(lv_ui *ui);
void setup_scr_screen_calendar(lv_ui *ui);
void setup_scr_screen_set_waln(lv_ui *ui);
void setup_scr_screen_vedio(lv_ui *ui);
void setup_scr_screen_vedio_show(lv_ui* ui);
void setup_scr_screen_album(lv_ui *ui);
void setup_scr_screen_album_show(lv_ui *ui);
void setup_scr_screen_music(lv_ui *ui);
void setup_scr_screen_fe(lv_ui *ui);
void setup_scr_screen_weather(lv_ui *ui);
void setup_scr_screen_camera(lv_ui *ui);

//LV_FONT_DECLARE(lv_font_montserratMedium_12)
//LV_FONT_DECLARE(lv_font_montserratMedium_16)
//LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_18)


#ifdef __cplusplus
}
#endif
#endif
