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



void setup_scr_screen_setting_sys_update(lv_ui *ui)
{
    //Write codes screen_setting_sys_update
    ui->screen_setting_sys_update = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_setting_sys_update, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_setting_sys_update, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_setting_sys_update, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_cont_main
    ui->screen_setting_sys_update_cont_main = lv_obj_create(ui->screen_setting_sys_update);
    lv_obj_set_pos(ui->screen_setting_sys_update_cont_main, 0, 0);
    lv_obj_set_size(ui->screen_setting_sys_update_cont_main, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_setting_sys_update_cont_main, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_setting_sys_update_cont_main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_cont_main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_cont_main, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_cont_main, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_sys_update_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_sys_update_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_sys_update_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_sys_update_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_btn_3
    ui->screen_setting_sys_update_btn_3 = lv_button_create(ui->screen_setting_sys_update_cont_main);
    lv_obj_set_pos(ui->screen_setting_sys_update_btn_3, 0, 85);
    lv_obj_set_size(ui->screen_setting_sys_update_btn_3, 230, 40);
    ui->screen_setting_sys_update_btn_3_label = lv_label_create(ui->screen_setting_sys_update_btn_3);
    lv_label_set_text(ui->screen_setting_sys_update_btn_3_label, "      Check Update");
    lv_label_set_long_mode(ui->screen_setting_sys_update_btn_3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_setting_sys_update_btn_3_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_setting_sys_update_btn_3, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_setting_sys_update_btn_3_label, LV_PCT(100));

    //Write style for screen_setting_sys_update_btn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_btn_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_btn_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_setting_sys_update_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_setting_sys_update_btn_3, lv_color_hex(0xd4d4d4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_setting_sys_update_btn_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_btn_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_sys_update_btn_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_sys_update_btn_3, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_sys_update_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_sys_update_btn_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_btn_2
    ui->screen_setting_sys_update_btn_2 = lv_button_create(ui->screen_setting_sys_update_cont_main);
    lv_obj_set_pos(ui->screen_setting_sys_update_btn_2, 0, 40);
    lv_obj_set_size(ui->screen_setting_sys_update_btn_2, 230, 40);
    ui->screen_setting_sys_update_btn_2_label = lv_label_create(ui->screen_setting_sys_update_btn_2);
    lv_label_set_text(ui->screen_setting_sys_update_btn_2_label, "      Update History ");
    lv_label_set_long_mode(ui->screen_setting_sys_update_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_setting_sys_update_btn_2_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_setting_sys_update_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_setting_sys_update_btn_2_label, LV_PCT(100));

    //Write style for screen_setting_sys_update_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_btn_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_setting_sys_update_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_setting_sys_update_btn_2, lv_color_hex(0xb8b8b8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_setting_sys_update_btn_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_btn_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_sys_update_btn_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_sys_update_btn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_sys_update_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_sys_update_btn_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_btn_1
    ui->screen_setting_sys_update_btn_1 = lv_button_create(ui->screen_setting_sys_update_cont_main);
    lv_obj_set_pos(ui->screen_setting_sys_update_btn_1, 5, 10);
    lv_obj_set_size(ui->screen_setting_sys_update_btn_1, 20, 20);
    lv_obj_add_flag(ui->screen_setting_sys_update_btn_1, LV_OBJ_FLAG_CHECKABLE);
    ui->screen_setting_sys_update_btn_1_label = lv_label_create(ui->screen_setting_sys_update_btn_1);
    lv_label_set_text(ui->screen_setting_sys_update_btn_1_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->screen_setting_sys_update_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_setting_sys_update_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_setting_sys_update_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_setting_sys_update_btn_1_label, LV_PCT(100));

    //Write style for screen_setting_sys_update_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_sys_update_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_sys_update_btn_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_sys_update_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_sys_update_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_label_1
    ui->screen_setting_sys_update_label_1 = lv_label_create(ui->screen_setting_sys_update_cont_main);
    lv_obj_set_pos(ui->screen_setting_sys_update_label_1, 36, 9);
    lv_obj_set_size(ui->screen_setting_sys_update_label_1, 160, 20);
    lv_label_set_text(ui->screen_setting_sys_update_label_1, "System Update");
    lv_label_set_long_mode(ui->screen_setting_sys_update_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_sys_update_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_sys_update_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_sys_update_label_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_sys_update_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_sys_update_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_cont_2
    ui->screen_setting_sys_update_cont_2 = lv_obj_create(ui->screen_setting_sys_update);
    lv_obj_set_pos(ui->screen_setting_sys_update_cont_2, 0, 0);
    lv_obj_set_size(ui->screen_setting_sys_update_cont_2, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_setting_sys_update_cont_2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_setting_sys_update_cont_2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_setting_sys_update_cont_2, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_setting_sys_update_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_sys_update_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_sys_update_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_sys_update_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_sys_update_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_btn_4
    ui->screen_setting_sys_update_btn_4 = lv_button_create(ui->screen_setting_sys_update_cont_2);
    lv_obj_set_pos(ui->screen_setting_sys_update_btn_4, 5, 10);
    lv_obj_set_size(ui->screen_setting_sys_update_btn_4, 20, 20);
    lv_obj_add_flag(ui->screen_setting_sys_update_btn_4, LV_OBJ_FLAG_CHECKABLE);
    ui->screen_setting_sys_update_btn_4_label = lv_label_create(ui->screen_setting_sys_update_btn_4);
    lv_label_set_text(ui->screen_setting_sys_update_btn_4_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->screen_setting_sys_update_btn_4_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_setting_sys_update_btn_4_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_setting_sys_update_btn_4, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_setting_sys_update_btn_4_label, LV_PCT(100));

    //Write style for screen_setting_sys_update_btn_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_btn_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_btn_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_btn_4, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_sys_update_btn_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_sys_update_btn_4, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_sys_update_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_sys_update_btn_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_label_2
    ui->screen_setting_sys_update_label_2 = lv_label_create(ui->screen_setting_sys_update_cont_2);
    lv_obj_set_pos(ui->screen_setting_sys_update_label_2, 36, 9);
    lv_obj_set_size(ui->screen_setting_sys_update_label_2, 160, 20);
    lv_label_set_text(ui->screen_setting_sys_update_label_2, "Update History ");
    lv_label_set_long_mode(ui->screen_setting_sys_update_label_2, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_sys_update_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_sys_update_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_sys_update_label_2, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_sys_update_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_sys_update_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_spangroup_1
    ui->screen_setting_sys_update_spangroup_1 = lv_spangroup_create(ui->screen_setting_sys_update_cont_2);
    lv_obj_set_pos(ui->screen_setting_sys_update_spangroup_1, 0, 40);
    lv_obj_set_size(ui->screen_setting_sys_update_spangroup_1, 240, 280);
    lv_spangroup_set_align(ui->screen_setting_sys_update_spangroup_1, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_setting_sys_update_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_setting_sys_update_spangroup_1, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_setting_sys_update_spangroup_1_span = lv_spangroup_new_span(ui->screen_setting_sys_update_spangroup_1);
    lv_span_set_text(ui->screen_setting_sys_update_spangroup_1_span, "version : 1.0 \ndatetime : 2025/11/20\n\n");
    lv_style_set_text_color(lv_span_get_style(ui->screen_setting_sys_update_spangroup_1_span), lv_color_hex(0x000000));
    lv_style_set_text_decor(lv_span_get_style(ui->screen_setting_sys_update_spangroup_1_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->screen_setting_sys_update_spangroup_1_span), &lv_font_montserratMedium_16);
    ui->screen_setting_sys_update_spangroup_1_span = lv_spangroup_new_span(ui->screen_setting_sys_update_spangroup_1);
    lv_span_set_text(ui->screen_setting_sys_update_spangroup_1_span, "1.Add 8 apps in this version.\n2.System run smoothly than last version\n ");
    lv_style_set_text_color(lv_span_get_style(ui->screen_setting_sys_update_spangroup_1_span), lv_color_hex(0x000000));
    lv_style_set_text_decor(lv_span_get_style(ui->screen_setting_sys_update_spangroup_1_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->screen_setting_sys_update_spangroup_1_span), &lv_font_montserratMedium_12);

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_sys_update_spangroup_1_main_main_default
    static lv_style_t style_screen_setting_sys_update_spangroup_1_main_main_default;
    ui_init_style(&style_screen_setting_sys_update_spangroup_1_main_main_default);

    lv_style_set_border_width(&style_screen_setting_sys_update_spangroup_1_main_main_default, 0);
    lv_style_set_radius(&style_screen_setting_sys_update_spangroup_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_setting_sys_update_spangroup_1_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_setting_sys_update_spangroup_1_main_main_default, lv_color_hex(0xe3e3e3));
    lv_style_set_bg_grad_dir(&style_screen_setting_sys_update_spangroup_1_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_pad_top(&style_screen_setting_sys_update_spangroup_1_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_setting_sys_update_spangroup_1_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_setting_sys_update_spangroup_1_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_setting_sys_update_spangroup_1_main_main_default, 5);
    lv_style_set_shadow_width(&style_screen_setting_sys_update_spangroup_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_setting_sys_update_spangroup_1, &style_screen_setting_sys_update_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_setting_sys_update_spangroup_1);

    //Write codes screen_setting_sys_update_spinner_update
    ui->screen_setting_sys_update_spinner_update = lv_spinner_create(ui->screen_setting_sys_update);
    lv_obj_set_pos(ui->screen_setting_sys_update_spinner_update, 100, 140);
    lv_obj_set_size(ui->screen_setting_sys_update_spinner_update, 40, 40);
    lv_obj_add_flag(ui->screen_setting_sys_update_spinner_update, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_setting_sys_update_spinner_update, LV_OBJ_FLAG_HIDDEN);
    lv_spinner_set_anim_params(ui->screen_setting_sys_update_spinner_update, 2000, 200);

    //Write style for screen_setting_sys_update_spinner_update, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->screen_setting_sys_update_spinner_update, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_sys_update_spinner_update, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_sys_update_spinner_update, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_sys_update_spinner_update, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_spinner_update, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_spinner_update, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_spinner_update, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->screen_setting_sys_update_spinner_update, 12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_setting_sys_update_spinner_update, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_setting_sys_update_spinner_update, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->screen_setting_sys_update_spinner_update, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_spinner_update, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_setting_sys_update_spinner_update, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->screen_setting_sys_update_spinner_update, 4, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_setting_sys_update_spinner_update, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_setting_sys_update_spinner_update, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->screen_setting_sys_update_spinner_update, true, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_msgbox_update_info
    ui->screen_setting_sys_update_msgbox_update_info = lv_msgbox_create(ui->screen_setting_sys_update);
    lv_obj_set_pos(ui->screen_setting_sys_update_msgbox_update_info, 20, 80);
    lv_obj_set_size(ui->screen_setting_sys_update_msgbox_update_info, 200, 160);
    lv_obj_add_flag(ui->screen_setting_sys_update_msgbox_update_info, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_setting_sys_update_msgbox_update_info, LV_OBJ_FLAG_HIDDEN);
    lv_msgbox_add_title(ui->screen_setting_sys_update_msgbox_update_info, "");
    lv_msgbox_add_text(ui->screen_setting_sys_update_msgbox_update_info, "Check Update fail. Server have \nproblams . Do you  try again?");
    lv_obj_align_to(ui->screen_setting_sys_update_msgbox_update_info, ui->screen_setting_sys_update, LV_ALIGN_TOP_LEFT, 20, 80);
    ui->screen_setting_sys_update_msgbox_update_info_item0 = lv_msgbox_add_footer_button(ui->screen_setting_sys_update_msgbox_update_info, "Yes");
    lv_obj_set_size(ui->screen_setting_sys_update_msgbox_update_info_item0, 80, 20);
    ui->screen_setting_sys_update_msgbox_update_info_item1 = lv_msgbox_add_footer_button(ui->screen_setting_sys_update_msgbox_update_info, "No");
    lv_obj_set_size(ui->screen_setting_sys_update_msgbox_update_info_item1, 80, 20);

    //Write style for screen_setting_sys_update_msgbox_update_info, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_msgbox_update_info, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_msgbox_update_info, lv_color_hex(0xecfff8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_msgbox_update_info, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_msgbox_update_info, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_setting_sys_update_msgbox_update_info, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_setting_sys_update_msgbox_update_info, lv_color_hex(0xe3e3e3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_setting_sys_update_msgbox_update_info, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_msgbox_update_info, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_msgbox_update_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default
    static lv_style_t style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default;
    ui_init_style(&style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default);

    lv_style_set_bg_opa(&style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default, 255);
    lv_style_set_bg_color(&style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default, lv_color_hex(0x4e4e4e));
    lv_style_set_text_font(&style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default, 255);
    lv_style_set_text_letter_space(&style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default, 0);
    lv_style_set_text_line_space(&style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default, 15);
    lv_obj_add_style(lv_msgbox_get_header(ui->screen_setting_sys_update_msgbox_update_info), &style_screen_setting_sys_update_msgbox_update_info_extra_title_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_sys_update_msgbox_update_info_extra_content_main_default
    static lv_style_t style_screen_setting_sys_update_msgbox_update_info_extra_content_main_default;
    ui_init_style(&style_screen_setting_sys_update_msgbox_update_info_extra_content_main_default);

    lv_style_set_text_color(&style_screen_setting_sys_update_msgbox_update_info_extra_content_main_default, lv_color_hex(0x4e4e4e));
    lv_style_set_text_font(&style_screen_setting_sys_update_msgbox_update_info_extra_content_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_setting_sys_update_msgbox_update_info_extra_content_main_default, 255);
    lv_style_set_text_letter_space(&style_screen_setting_sys_update_msgbox_update_info_extra_content_main_default, 0);
    lv_style_set_text_line_space(&style_screen_setting_sys_update_msgbox_update_info_extra_content_main_default, 10);
    lv_obj_add_style(lv_msgbox_get_content(ui->screen_setting_sys_update_msgbox_update_info), &style_screen_setting_sys_update_msgbox_update_info_extra_content_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default
    static lv_style_t style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default;
    ui_init_style(&style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default);

    lv_style_set_bg_opa(&style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, 255);
    lv_style_set_bg_color(&style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, 0);
    lv_style_set_radius(&style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, 0);
    lv_style_set_text_color(&style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, lv_color_hex(0x4e4e4e));
    lv_style_set_text_font(&style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, 255);
    lv_obj_add_style(ui->screen_setting_sys_update_msgbox_update_info_item0, &style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_setting_sys_update_msgbox_update_info_item1, &style_screen_setting_sys_update_msgbox_update_info_extra_btns_items_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_cont_updating
    ui->screen_setting_sys_update_cont_updating = lv_obj_create(ui->screen_setting_sys_update);
    lv_obj_set_pos(ui->screen_setting_sys_update_cont_updating, 20, 99);
    lv_obj_set_size(ui->screen_setting_sys_update_cont_updating, 200, 100);
    lv_obj_set_scrollbar_mode(ui->screen_setting_sys_update_cont_updating, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_setting_sys_update_cont_updating, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_setting_sys_update_cont_updating, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_setting_sys_update_cont_updating, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_cont_updating, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_setting_sys_update_cont_updating, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_setting_sys_update_cont_updating, lv_color_hex(0xd7d7d7), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_setting_sys_update_cont_updating, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_cont_updating, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_cont_updating, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_cont_updating, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_cont_updating, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_sys_update_cont_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_sys_update_cont_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_sys_update_cont_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_sys_update_cont_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_cont_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_bar_updating
    ui->screen_setting_sys_update_bar_updating = lv_bar_create(ui->screen_setting_sys_update_cont_updating);
    lv_obj_set_pos(ui->screen_setting_sys_update_bar_updating, 20, 40);
    lv_obj_set_size(ui->screen_setting_sys_update_bar_updating, 160, 6);
    lv_obj_set_style_anim_duration(ui->screen_setting_sys_update_bar_updating, 1000, 0);
    lv_bar_set_mode(ui->screen_setting_sys_update_bar_updating, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_setting_sys_update_bar_updating, 0, 100);
    lv_bar_set_value(ui->screen_setting_sys_update_bar_updating, 30, LV_ANIM_OFF);

    //Write style for screen_setting_sys_update_bar_updating, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_bar_updating, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_bar_updating, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_bar_updating, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_bar_updating, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_bar_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_setting_sys_update_bar_updating, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_bar_updating, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_sys_update_bar_updating, lv_color_hex(0x3f3636), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_sys_update_bar_updating, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_bar_updating, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_setting_sys_update_label_updating
    ui->screen_setting_sys_update_label_updating = lv_label_create(ui->screen_setting_sys_update_cont_updating);
    lv_obj_set_pos(ui->screen_setting_sys_update_label_updating, 20, 20);
    lv_obj_set_size(ui->screen_setting_sys_update_label_updating, 150, 12);
    lv_label_set_text(ui->screen_setting_sys_update_label_updating, "updating  30%");
    lv_label_set_long_mode(ui->screen_setting_sys_update_label_updating, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_sys_update_label_updating, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_sys_update_label_updating, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_sys_update_label_updating, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_sys_update_label_updating, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_sys_update_label_updating, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_sys_update_label_updating, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_setting_sys_update.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_setting_sys_update);

    //Init events for screen.
    events_init_screen_setting_sys_update(ui);
}
