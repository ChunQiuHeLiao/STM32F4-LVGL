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


void setup_scr_screen_music(lv_ui *ui)
{
    //Write codes screen_music
    ui->screen_music = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_music, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_music, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_cont_main
    ui->screen_music_cont_main = lv_obj_create(ui->screen_music);
    lv_obj_set_pos(ui->screen_music_cont_main, 0, 30);
    lv_obj_set_size(ui->screen_music_cont_main, 240, 290);
    lv_obj_set_scrollbar_mode(ui->screen_music_cont_main, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_music_cont_main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_music_cont_main, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_music_cont_main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_music_cont_main, lv_color_hex(0xbcbcbc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_music_cont_main, LV_BORDER_SIDE_TOP, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_music_cont_main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_cont_main, lv_color_hex(0xf3f3f3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_cont_main, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_music_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_music_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_music_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_music_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_list_music
    ui->screen_music_list_music = lv_list_create(ui->screen_music_cont_main);
    lv_obj_set_pos(ui->screen_music_list_music, 30, 20);
    lv_obj_set_size(ui->screen_music_list_music, 180, 220);
    lv_obj_set_scrollbar_mode(ui->screen_music_list_music, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_music_list_music, LV_OBJ_FLAG_HIDDEN);
    ui->screen_music_list_music_item0 = lv_list_add_button(ui->screen_music_list_music, LV_SYMBOL_AUDIO, "3323232");

    //Write style state: LV_STATE_DEFAULT for &style_screen_music_list_music_main_main_default
    static lv_style_t style_screen_music_list_music_main_main_default;
    ui_init_style(&style_screen_music_list_music_main_main_default);

    lv_style_set_pad_top(&style_screen_music_list_music_main_main_default, 5);
    lv_style_set_pad_left(&style_screen_music_list_music_main_main_default, 5);
    lv_style_set_pad_right(&style_screen_music_list_music_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_music_list_music_main_main_default, 5);
    lv_style_set_bg_opa(&style_screen_music_list_music_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_music_list_music_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_music_list_music_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_music_list_music_main_main_default, 2);
    lv_style_set_border_opa(&style_screen_music_list_music_main_main_default, 255);
    lv_style_set_border_color(&style_screen_music_list_music_main_main_default, lv_color_hex(0xe0e0e0));
    lv_style_set_border_side(&style_screen_music_list_music_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_music_list_music_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_music_list_music_main_main_default, 2);
    lv_style_set_shadow_color(&style_screen_music_list_music_main_main_default, lv_color_hex(0xe8e8e8));
    lv_style_set_shadow_opa(&style_screen_music_list_music_main_main_default, 255);
    lv_style_set_shadow_spread(&style_screen_music_list_music_main_main_default, 2);
    lv_style_set_shadow_offset_x(&style_screen_music_list_music_main_main_default, 2);
    lv_style_set_shadow_offset_y(&style_screen_music_list_music_main_main_default, 2);
    lv_obj_add_style(ui->screen_music_list_music, &style_screen_music_list_music_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_music_list_music_main_scrollbar_default
    static lv_style_t style_screen_music_list_music_main_scrollbar_default;
    ui_init_style(&style_screen_music_list_music_main_scrollbar_default);

    lv_style_set_radius(&style_screen_music_list_music_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_music_list_music_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_music_list_music_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_music_list_music_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_music_list_music, &style_screen_music_list_music_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_music_list_music_extra_btns_main_default
    static lv_style_t style_screen_music_list_music_extra_btns_main_default;
    ui_init_style(&style_screen_music_list_music_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_music_list_music_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_screen_music_list_music_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_screen_music_list_music_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_music_list_music_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_screen_music_list_music_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_screen_music_list_music_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_music_list_music_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_music_list_music_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_music_list_music_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_music_list_music_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_music_list_music_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_music_list_music_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_music_list_music_item0, &style_screen_music_list_music_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_music_list_music_extra_texts_main_default
    static lv_style_t style_screen_music_list_music_extra_texts_main_default;
    ui_init_style(&style_screen_music_list_music_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_music_list_music_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_screen_music_list_music_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_screen_music_list_music_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_music_list_music_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_screen_music_list_music_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_music_list_music_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_music_list_music_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_music_list_music_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_music_list_music_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_music_list_music_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_music_list_music_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_music_list_music_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_music_list_music_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //Write codes screen_music_cont_play_area
    ui->screen_music_cont_play_area = lv_obj_create(ui->screen_music);
    lv_obj_set_pos(ui->screen_music_cont_play_area, 0, 280);
    lv_obj_set_size(ui->screen_music_cont_play_area, 240, 30);
    lv_obj_set_scrollbar_mode(ui->screen_music_cont_play_area, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_music_cont_play_area, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_music_cont_play_area, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_music_cont_play_area, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_music_cont_play_area, lv_color_hex(0xdbdbdb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_music_cont_play_area, LV_BORDER_SIDE_TOP, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_cont_play_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_music_cont_play_area, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_cont_play_area, lv_color_hex(0xe4e4e4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_cont_play_area, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_music_cont_play_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_music_cont_play_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_music_cont_play_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_music_cont_play_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_cont_play_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_btn_play
    ui->screen_music_btn_play = lv_button_create(ui->screen_music_cont_play_area);
    lv_obj_set_pos(ui->screen_music_btn_play, 132, 4);
    lv_obj_set_size(ui->screen_music_btn_play, 30, 20);
    ui->screen_music_btn_play_label = lv_label_create(ui->screen_music_btn_play);
    lv_label_set_text(ui->screen_music_btn_play_label, "" LV_SYMBOL_PLAY " ");
    lv_label_set_long_mode(ui->screen_music_btn_play_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_btn_play_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_btn_play, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_music_btn_play_label, LV_PCT(100));

    //Write style for screen_music_btn_play, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_btn_play, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_btn_play, lv_color_hex(0xe3e3e3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_btn_play, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_music_btn_play, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_btn_play, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_btn_play, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_btn_play, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_btn_play, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_btn_play, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_btn_play, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_label_music_name
    ui->screen_music_label_music_name = lv_label_create(ui->screen_music_cont_play_area);
    lv_obj_set_pos(ui->screen_music_label_music_name, 5, 10);
    lv_obj_set_size(ui->screen_music_label_music_name, 80, 12);
    lv_label_set_text(ui->screen_music_label_music_name, "a possible sing test sing");
    lv_label_set_long_mode(ui->screen_music_label_music_name, LV_LABEL_LONG_SCROLL_CIRCULAR);

    //Write style for screen_music_label_music_name, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_label_music_name, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_label_music_name, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_label_music_name, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_label_music_name, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_btn_1
    ui->screen_music_btn_1 = lv_button_create(ui->screen_music_cont_play_area);
    lv_obj_set_pos(ui->screen_music_btn_1, 172, 4);
    lv_obj_set_size(ui->screen_music_btn_1, 30, 20);
    ui->screen_music_btn_1_label = lv_label_create(ui->screen_music_btn_1);
    lv_label_set_text(ui->screen_music_btn_1_label, "" LV_SYMBOL_NEXT " ");
    lv_label_set_long_mode(ui->screen_music_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_music_btn_1_label, LV_PCT(100));

    //Write style for screen_music_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_btn_1, lv_color_hex(0xe3e3e3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_music_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_btn_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_btn_prev
    ui->screen_music_btn_prev = lv_button_create(ui->screen_music_cont_play_area);
    lv_obj_set_pos(ui->screen_music_btn_prev, 92, 4);
    lv_obj_set_size(ui->screen_music_btn_prev, 30, 20);
    ui->screen_music_btn_prev_label = lv_label_create(ui->screen_music_btn_prev);
    lv_label_set_text(ui->screen_music_btn_prev_label, " " LV_SYMBOL_PREV " ");
    lv_label_set_long_mode(ui->screen_music_btn_prev_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_btn_prev_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_btn_prev, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_music_btn_prev_label, LV_PCT(100));

    //Write style for screen_music_btn_prev, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_btn_prev, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_btn_prev, lv_color_hex(0xe3e3e3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_btn_prev, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_music_btn_prev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_btn_prev, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_btn_prev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_btn_prev, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_btn_prev, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_btn_prev, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_btn_prev, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_btn_show_list
    ui->screen_music_btn_show_list = lv_button_create(ui->screen_music_cont_play_area);
    lv_obj_set_pos(ui->screen_music_btn_show_list, 205, 0);
    lv_obj_set_size(ui->screen_music_btn_show_list, 30, 30);
    ui->screen_music_btn_show_list_label = lv_label_create(ui->screen_music_btn_show_list);
    lv_label_set_text(ui->screen_music_btn_show_list_label, "" LV_SYMBOL_UP " ");
    lv_label_set_long_mode(ui->screen_music_btn_show_list_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_btn_show_list_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_btn_show_list, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_music_btn_show_list_label, LV_PCT(100));

    //Write style for screen_music_btn_show_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_btn_show_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_music_btn_show_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_btn_show_list, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_btn_show_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_btn_show_list, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_btn_show_list, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_btn_show_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_btn_show_list, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_cont_title
    ui->screen_music_cont_title = lv_obj_create(ui->screen_music);
    lv_obj_set_pos(ui->screen_music_cont_title, 0, 0);
    lv_obj_set_size(ui->screen_music_cont_title, 240, 30);
    lv_obj_set_scrollbar_mode(ui->screen_music_cont_title, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_music_cont_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_music_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_music_cont_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_cont_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_cont_title, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_music_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_music_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_music_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_music_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_btn_exit
    ui->screen_music_btn_exit = lv_button_create(ui->screen_music_cont_title);
    lv_obj_set_pos(ui->screen_music_btn_exit, 5, 5);
    lv_obj_set_size(ui->screen_music_btn_exit, 20, 20);
    lv_obj_add_flag(ui->screen_music_btn_exit, LV_OBJ_FLAG_CHECKABLE);
    ui->screen_music_btn_exit_label = lv_label_create(ui->screen_music_btn_exit);
    lv_label_set_text(ui->screen_music_btn_exit_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->screen_music_btn_exit_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_btn_exit_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_btn_exit, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_music_btn_exit_label, LV_PCT(100));

    //Write style for screen_music_btn_exit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_btn_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_btn_exit, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_btn_exit, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_music_btn_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_btn_exit, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_btn_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_btn_exit, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_btn_exit, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_btn_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_btn_exit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_label_tiltle
    ui->screen_music_label_tiltle = lv_label_create(ui->screen_music_cont_title);
    lv_obj_set_pos(ui->screen_music_label_tiltle, 80, 5);
    lv_obj_set_size(ui->screen_music_label_tiltle, 80, 20);
    lv_label_set_text(ui->screen_music_label_tiltle, "MUSIC\n");
    lv_label_set_long_mode(ui->screen_music_label_tiltle, LV_LABEL_LONG_WRAP);

    //Write style for screen_music_label_tiltle, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_label_tiltle, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_label_tiltle, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_label_tiltle, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_label_tiltle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_label_tiltle, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_music.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_music);

    //Init events for screen.
    events_init_screen_music(ui);
}
