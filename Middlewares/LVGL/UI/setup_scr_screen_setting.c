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


void setup_scr_screen_setting(lv_ui *ui)
{
    //Write codes screen_setting
    ui->screen_setting = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_setting, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_setting, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_cont_setting
    ui->screen_setting_cont_setting = lv_obj_create(ui->screen_setting);
    lv_obj_set_pos(ui->screen_setting_cont_setting, 0, 0);
    lv_obj_set_size(ui->screen_setting_cont_setting, 240, 320);
    lv_obj_set_scroll_dir(ui->screen_setting_cont_setting, LV_DIR_TOP|LV_DIR_BOTTOM);
    lv_obj_set_scrollbar_mode(ui->screen_setting_cont_setting, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_setting_cont_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_cont_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_cont_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_cont_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_cont_setting, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_cont_setting, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_cont_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_cont_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_cont_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_cont_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_cont_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_list_1
    ui->screen_setting_list_1 = lv_list_create(ui->screen_setting_cont_setting);
    lv_obj_set_pos(ui->screen_setting_list_1, 1, 0);
    lv_obj_set_size(ui->screen_setting_list_1, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_setting_list_1, LV_SCROLLBAR_MODE_OFF);
    ui->screen_setting_list_1_item0 = lv_list_add_button(ui->screen_setting_list_1, LV_SYMBOL_WIFI, "WALN");
    ui->screen_setting_list_1_item1 = lv_list_add_button(ui->screen_setting_list_1, LV_SYMBOL_BLUETOOTH, "BlueTooth");
    ui->screen_setting_list_1_item2 = lv_list_add_button(ui->screen_setting_list_1, LV_SYMBOL_SD_CARD, "SD Card");
    ui->screen_setting_list_1_item3 = lv_list_add_button(ui->screen_setting_list_1, LV_SYMBOL_REFRESH, "System Update");
    ui->screen_setting_list_1_item4 = lv_list_add_button(ui->screen_setting_list_1, LV_SYMBOL_EJECT, "System Infomation");

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_list_1_main_main_default
    static lv_style_t style_screen_setting_list_1_main_main_default;
    ui_init_style(&style_screen_setting_list_1_main_main_default);

    lv_style_set_pad_top(&style_screen_setting_list_1_main_main_default, 40);
    lv_style_set_pad_left(&style_screen_setting_list_1_main_main_default, 5);
    lv_style_set_pad_right(&style_screen_setting_list_1_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_setting_list_1_main_main_default, 5);
    lv_style_set_bg_opa(&style_screen_setting_list_1_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_setting_list_1_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_setting_list_1_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_setting_list_1_main_main_default, 0);
    lv_style_set_radius(&style_screen_setting_list_1_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_setting_list_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_setting_list_1, &style_screen_setting_list_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_list_1_main_scrollbar_default
    static lv_style_t style_screen_setting_list_1_main_scrollbar_default;
    ui_init_style(&style_screen_setting_list_1_main_scrollbar_default);

    lv_style_set_radius(&style_screen_setting_list_1_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_setting_list_1_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_setting_list_1_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_setting_list_1_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_setting_list_1, &style_screen_setting_list_1_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_list_1_extra_btns_main_default
    static lv_style_t style_screen_setting_list_1_extra_btns_main_default;
    ui_init_style(&style_screen_setting_list_1_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_setting_list_1_extra_btns_main_default, 10);
    lv_style_set_pad_left(&style_screen_setting_list_1_extra_btns_main_default, 20);
    lv_style_set_pad_right(&style_screen_setting_list_1_extra_btns_main_default, 10);
    lv_style_set_pad_bottom(&style_screen_setting_list_1_extra_btns_main_default, 10);
    lv_style_set_border_width(&style_screen_setting_list_1_extra_btns_main_default, 2);
    lv_style_set_border_opa(&style_screen_setting_list_1_extra_btns_main_default, 255);
    lv_style_set_border_color(&style_screen_setting_list_1_extra_btns_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_screen_setting_list_1_extra_btns_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_text_color(&style_screen_setting_list_1_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_setting_list_1_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_setting_list_1_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_setting_list_1_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_setting_list_1_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_setting_list_1_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_setting_list_1_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_setting_list_1_item4, &style_screen_setting_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_setting_list_1_item3, &style_screen_setting_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_setting_list_1_item2, &style_screen_setting_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_setting_list_1_item1, &style_screen_setting_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_setting_list_1_item0, &style_screen_setting_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_list_1_extra_texts_main_default
    static lv_style_t style_screen_setting_list_1_extra_texts_main_default;
    ui_init_style(&style_screen_setting_list_1_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_setting_list_1_extra_texts_main_default, 10);
    lv_style_set_pad_left(&style_screen_setting_list_1_extra_texts_main_default, 20);
    lv_style_set_pad_right(&style_screen_setting_list_1_extra_texts_main_default, 10);
    lv_style_set_pad_bottom(&style_screen_setting_list_1_extra_texts_main_default, 10);
    lv_style_set_border_width(&style_screen_setting_list_1_extra_texts_main_default, 2);
    lv_style_set_border_opa(&style_screen_setting_list_1_extra_texts_main_default, 255);
    lv_style_set_border_color(&style_screen_setting_list_1_extra_texts_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_screen_setting_list_1_extra_texts_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_text_color(&style_screen_setting_list_1_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_setting_list_1_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_setting_list_1_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_setting_list_1_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_setting_list_1_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_setting_list_1_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_setting_list_1_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_setting_list_1_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //Write codes screen_setting_btn_exit
    ui->screen_setting_btn_exit = lv_button_create(ui->screen_setting_cont_setting);
    lv_obj_set_pos(ui->screen_setting_btn_exit, 9, 9);
    lv_obj_set_size(ui->screen_setting_btn_exit, 20, 20);
    lv_obj_add_flag(ui->screen_setting_btn_exit, LV_OBJ_FLAG_CHECKABLE);
    ui->screen_setting_btn_exit_label = lv_label_create(ui->screen_setting_btn_exit);
    lv_label_set_text(ui->screen_setting_btn_exit_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->screen_setting_btn_exit_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_setting_btn_exit_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_setting_btn_exit, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_setting_btn_exit_label, LV_PCT(100));

    //Write style for screen_setting_btn_exit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_btn_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_btn_exit, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_btn_exit, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_btn_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_btn_exit, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_btn_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_btn_exit, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_btn_exit, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_btn_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_btn_exit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_1
    ui->screen_setting_label_1 = lv_label_create(ui->screen_setting_cont_setting);
    lv_obj_set_pos(ui->screen_setting_label_1, 76, 12);
    lv_obj_set_size(ui->screen_setting_label_1, 80, 20);
    lv_label_set_text(ui->screen_setting_label_1, "Setting");
    lv_label_set_long_mode(ui->screen_setting_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_cont_sys_info
    ui->screen_setting_cont_sys_info = lv_obj_create(ui->screen_setting);
    lv_obj_set_pos(ui->screen_setting_cont_sys_info, 0, 0);
    lv_obj_set_size(ui->screen_setting_cont_sys_info, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_setting_cont_sys_info, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_setting_cont_sys_info, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_setting_cont_sys_info, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_setting_cont_sys_info, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_cont_sys_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_cont_sys_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_cont_sys_info, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_cont_sys_info, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_cont_sys_info, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_cont_sys_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_cont_sys_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_cont_sys_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_cont_sys_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_cont_sys_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_btn_2
    ui->screen_setting_btn_2 = lv_button_create(ui->screen_setting_cont_sys_info);
    lv_obj_set_pos(ui->screen_setting_btn_2, 5, 10);
    lv_obj_set_size(ui->screen_setting_btn_2, 20, 20);
    lv_obj_add_flag(ui->screen_setting_btn_2, LV_OBJ_FLAG_CHECKABLE);
    ui->screen_setting_btn_2_label = lv_label_create(ui->screen_setting_btn_2);
    lv_label_set_text(ui->screen_setting_btn_2_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->screen_setting_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_setting_btn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_setting_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_setting_btn_2_label, LV_PCT(100));

    //Write style for screen_setting_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_btn_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_btn_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_btn_2, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_3
    ui->screen_setting_label_3 = lv_label_create(ui->screen_setting_cont_sys_info);
    lv_obj_set_pos(ui->screen_setting_label_3, 35, 10);
    lv_obj_set_size(ui->screen_setting_label_3, 175, 20);
    lv_label_set_text(ui->screen_setting_label_3, "System Information");
    lv_label_set_long_mode(ui->screen_setting_label_3, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_3, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_spangroup_2
    ui->screen_setting_spangroup_2 = lv_spangroup_create(ui->screen_setting_cont_sys_info);
    lv_obj_set_pos(ui->screen_setting_spangroup_2, 0, 39);
    lv_obj_set_size(ui->screen_setting_spangroup_2, 230, 280);
    lv_spangroup_set_align(ui->screen_setting_spangroup_2, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_setting_spangroup_2, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_setting_spangroup_2, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_setting_spangroup_2_span = lv_spangroup_new_span(ui->screen_setting_spangroup_2);
    lv_span_set_text(ui->screen_setting_spangroup_2_span, "MCU : STM32F103VET6\nRAM : 64KB\nFLASH : 512KB\nID : 8372211");
    lv_style_set_text_color(lv_span_get_style(ui->screen_setting_spangroup_2_span), lv_color_hex(0x000000));
    lv_style_set_text_decor(lv_span_get_style(ui->screen_setting_spangroup_2_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->screen_setting_spangroup_2_span), &lv_font_montserratMedium_16);

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_spangroup_2_main_main_default
    static lv_style_t style_screen_setting_spangroup_2_main_main_default;
    ui_init_style(&style_screen_setting_spangroup_2_main_main_default);

    lv_style_set_border_width(&style_screen_setting_spangroup_2_main_main_default, 0);
    lv_style_set_radius(&style_screen_setting_spangroup_2_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_setting_spangroup_2_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_setting_spangroup_2_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_setting_spangroup_2_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_pad_top(&style_screen_setting_spangroup_2_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_setting_spangroup_2_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_setting_spangroup_2_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_setting_spangroup_2_main_main_default, 5);
    lv_style_set_shadow_width(&style_screen_setting_spangroup_2_main_main_default, 0);
    lv_obj_add_style(ui->screen_setting_spangroup_2, &style_screen_setting_spangroup_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_setting_spangroup_2);

    //Write codes screen_setting_cont_sd_card
    ui->screen_setting_cont_sd_card = lv_obj_create(ui->screen_setting);
    lv_obj_set_pos(ui->screen_setting_cont_sd_card, 0, 0);
    lv_obj_set_size(ui->screen_setting_cont_sd_card, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_setting_cont_sd_card, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_setting_cont_sd_card, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_setting_cont_sd_card, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_setting_cont_sd_card, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_cont_sd_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_cont_sd_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_cont_sd_card, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_cont_sd_card, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_cont_sd_card, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_cont_sd_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_cont_sd_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_cont_sd_card, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_cont_sd_card, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_cont_sd_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_btn_exit_sd
    ui->screen_setting_btn_exit_sd = lv_button_create(ui->screen_setting_cont_sd_card);
    lv_obj_set_pos(ui->screen_setting_btn_exit_sd, 8, 8);
    lv_obj_set_size(ui->screen_setting_btn_exit_sd, 20, 20);
    lv_obj_add_flag(ui->screen_setting_btn_exit_sd, LV_OBJ_FLAG_CHECKABLE);
    ui->screen_setting_btn_exit_sd_label = lv_label_create(ui->screen_setting_btn_exit_sd);
    lv_label_set_text(ui->screen_setting_btn_exit_sd_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->screen_setting_btn_exit_sd_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_setting_btn_exit_sd_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_setting_btn_exit_sd, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_setting_btn_exit_sd_label, LV_PCT(100));

    //Write style for screen_setting_btn_exit_sd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_btn_exit_sd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_btn_exit_sd, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_btn_exit_sd, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_btn_exit_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_btn_exit_sd, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_btn_exit_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_btn_exit_sd, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_btn_exit_sd, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_btn_exit_sd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_btn_exit_sd, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_title_sd
    ui->screen_setting_title_sd = lv_label_create(ui->screen_setting_cont_sd_card);
    lv_obj_set_pos(ui->screen_setting_title_sd, 36, 9);
    lv_obj_set_size(ui->screen_setting_title_sd, 160, 20);
    lv_label_set_text(ui->screen_setting_title_sd, "SD Card");
    lv_label_set_long_mode(ui->screen_setting_title_sd, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_title_sd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_title_sd, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_title_sd, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_title_sd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_title_sd, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_title_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_area_sd
    ui->screen_setting_area_sd = lv_spangroup_create(ui->screen_setting_cont_sd_card);
    lv_obj_set_pos(ui->screen_setting_area_sd, 0, 40);
    lv_obj_set_size(ui->screen_setting_area_sd, 230, 280);
    lv_spangroup_set_align(ui->screen_setting_area_sd, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_setting_area_sd, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_setting_area_sd, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_setting_area_sd_span = lv_spangroup_new_span(ui->screen_setting_area_sd);
    lv_span_set_text(ui->screen_setting_area_sd_span, "Type : SD Speed\nCapacity : 32GB\nLeft Capacity: 31GB\n-----------------------------------\nSD Protocal : SD 2.0\nID : 721949233");
    lv_style_set_text_color(lv_span_get_style(ui->screen_setting_area_sd_span), lv_color_hex(0x000000));
    lv_style_set_text_decor(lv_span_get_style(ui->screen_setting_area_sd_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->screen_setting_area_sd_span), &lv_font_montserratMedium_16);

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_area_sd_main_main_default
    static lv_style_t style_screen_setting_area_sd_main_main_default;
    ui_init_style(&style_screen_setting_area_sd_main_main_default);

    lv_style_set_border_width(&style_screen_setting_area_sd_main_main_default, 0);
    lv_style_set_radius(&style_screen_setting_area_sd_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_setting_area_sd_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_setting_area_sd_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_setting_area_sd_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_pad_top(&style_screen_setting_area_sd_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_setting_area_sd_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_setting_area_sd_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_setting_area_sd_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_setting_area_sd_main_main_default, 0);
    lv_obj_add_style(ui->screen_setting_area_sd, &style_screen_setting_area_sd_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_setting_area_sd);

    //The custom code of screen_setting.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_setting);

    //Init events for screen.
    events_init_screen_setting(ui);
}
