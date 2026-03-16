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
#include"ex/util_mid.h"
#include"Core/setting.h"

static lv_obj_t* table = NULL; /*显示音乐名和删除按钮的表格*/

static void screen_set_waln_sw_waln_event_handler(lv_event_t *e);
static void Set_WALN_Init(lv_ui* ui);
static void wifi_show_timer_handler(lv_timer_t* t);
static void main_timer_handler(lv_timer_t* t);

static void wifi_event_cb(lv_event_t* e)
{
    /*表格行列，因为只有一列*/
    uint32_t row = 0;
    uint32_t col = 0;
    lv_obj_t* tb = lv_event_get_target_obj(e);
    lv_table_get_selected_cell(tb, &row, &col); /*获取行列*/
    const char* val=lv_table_get_cell_value(tb, row, col);

    //printf("row:%d,wifi:%s\n", row,val+5);

    /*跳转到wifi 连接界面*/
    lv_obj_remove_flag(guider_ui.screen_set_waln_cont_con_pwd, LV_OBJ_FLAG_HIDDEN);
    /*显示要连接的wifi名*/
    lv_label_set_text(guider_ui.screen_set_waln_label_titl, val + 5);
}


void setup_scr_screen_set_waln(lv_ui *ui)
{
    printf("waln enter\n");

    //Write codes screen_set_waln
    ui->screen_set_waln = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_set_waln, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_set_waln, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_waln, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_cont_waln_area
    ui->screen_set_waln_cont_waln_area = lv_obj_create(ui->screen_set_waln);
    lv_obj_set_pos(ui->screen_set_waln_cont_waln_area, 0, 80);
    lv_obj_set_size(ui->screen_set_waln_cont_waln_area, 240, 240);
    lv_obj_set_scrollbar_mode(ui->screen_set_waln_cont_waln_area, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_set_waln_cont_waln_area, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_set_waln_cont_waln_area, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_cont_waln_area, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_waln_cont_waln_area, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_waln_cont_waln_area, lv_color_hex(0xcecece), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_waln_cont_waln_area, LV_BORDER_SIDE_TOP, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_cont_waln_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_cont_waln_area, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_cont_waln_area, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_cont_waln_area, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_cont_waln_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_cont_waln_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_cont_waln_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_cont_waln_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_cont_waln_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);





    //Write codes screen_set_waln_btn_flush
    ui->screen_set_waln_btn_flush = lv_button_create(ui->screen_set_waln_cont_waln_area);
    lv_obj_set_pos(ui->screen_set_waln_btn_flush, 175, 2);
    lv_obj_set_size(ui->screen_set_waln_btn_flush, 60, 18);
    ui->screen_set_waln_btn_flush_label = lv_label_create(ui->screen_set_waln_btn_flush);
    lv_label_set_text(ui->screen_set_waln_btn_flush_label, "flush");
    lv_label_set_long_mode(ui->screen_set_waln_btn_flush_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_waln_btn_flush_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_waln_btn_flush, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_waln_btn_flush_label, LV_PCT(100));

    //Write style for screen_set_waln_btn_flush, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_btn_flush, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_btn_flush, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_btn_flush, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_waln_btn_flush, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_btn_flush, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_btn_flush, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_btn_flush, lv_color_hex(0x0055ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_btn_flush, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_btn_flush, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_btn_flush, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_label_1
    //ui->screen_set_waln_label_1 = lv_label_create(ui->screen_set_waln_cont_waln_area);
    //lv_obj_set_pos(ui->screen_set_waln_label_1, -731, 96);
    //lv_obj_set_size(ui->screen_set_waln_label_1, 120, 12);
    //lv_label_set_text(ui->screen_set_waln_label_1, "available network");
    //lv_label_set_long_mode(ui->screen_set_waln_label_1, LV_LABEL_LONG_WRAP);

    ////Write style for screen_set_waln_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    //lv_obj_set_style_border_width(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_radius(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_text_color(ui->screen_set_waln_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_text_font(ui->screen_set_waln_label_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_text_opa(ui->screen_set_waln_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_text_letter_space(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_text_line_space(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_text_align(ui->screen_set_waln_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_opa(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_pad_top(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_pad_right(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_pad_bottom(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_pad_left(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_shadow_width(ui->screen_set_waln_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_cont_up
    ui->screen_set_waln_cont_up = lv_obj_create(ui->screen_set_waln);
    lv_obj_set_pos(ui->screen_set_waln_cont_up, 0, 0);
    lv_obj_set_size(ui->screen_set_waln_cont_up, 240, 80);
    lv_obj_set_scrollbar_mode(ui->screen_set_waln_cont_up, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_waln_cont_up, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_cont_up, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_cont_up, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_cont_up, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_cont_up, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_cont_up, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_cont_up, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_cont_up, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_cont_up, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_cont_up, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_cont_up, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_label
    ui->screen_set_waln_label = lv_label_create(ui->screen_set_waln_cont_up);
    lv_obj_set_pos(ui->screen_set_waln_label, 10, 50);
    lv_obj_set_size(ui->screen_set_waln_label, 60, 20);
    lv_label_set_text(ui->screen_set_waln_label, "WALN\n");
    lv_label_set_long_mode(ui->screen_set_waln_label, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_waln_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_label, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_label_title
    ui->screen_set_waln_label_title = lv_label_create(ui->screen_set_waln_cont_up);
    lv_obj_set_pos(ui->screen_set_waln_label_title, 90, 10);
    lv_obj_set_size(ui->screen_set_waln_label_title, 60, 20);
    lv_label_set_text(ui->screen_set_waln_label_title, "WALN\n");
    lv_label_set_long_mode(ui->screen_set_waln_label_title, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_waln_label_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_label_title, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_label_title, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_label_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_label_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_btn_exit
    ui->screen_set_waln_btn_exit = lv_button_create(ui->screen_set_waln_cont_up);
    lv_obj_set_pos(ui->screen_set_waln_btn_exit, 11, 8);
    lv_obj_set_size(ui->screen_set_waln_btn_exit, 20, 20);
    lv_obj_add_flag(ui->screen_set_waln_btn_exit, LV_OBJ_FLAG_CHECKABLE);
    ui->screen_set_waln_btn_exit_label = lv_label_create(ui->screen_set_waln_btn_exit);
    lv_label_set_text(ui->screen_set_waln_btn_exit_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->screen_set_waln_btn_exit_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_waln_btn_exit_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_waln_btn_exit, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_waln_btn_exit_label, LV_PCT(100));

    //Write style for screen_set_waln_btn_exit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_btn_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_btn_exit, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_btn_exit, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_waln_btn_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_btn_exit, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_btn_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_btn_exit, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_btn_exit, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_btn_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_btn_exit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_sw_waln
    ui->screen_set_waln_sw_waln = lv_switch_create(ui->screen_set_waln_cont_up);
    lv_obj_set_pos(ui->screen_set_waln_sw_waln, 180, 50);
    lv_obj_set_size(ui->screen_set_waln_sw_waln, 40, 20);

    //Write style for screen_set_waln_sw_waln, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_sw_waln, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_sw_waln, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_sw_waln, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_waln_sw_waln, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_sw_waln, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_sw_waln, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_set_waln_sw_waln, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_sw_waln, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_set_waln_sw_waln, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_sw_waln, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_set_waln_sw_waln, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_set_waln_sw_waln, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_sw_waln, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_sw_waln, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_sw_waln, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_waln_sw_waln, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_sw_waln, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_cont_con_pwd
    ui->screen_set_waln_cont_con_pwd = lv_obj_create(ui->screen_set_waln);
    lv_obj_set_pos(ui->screen_set_waln_cont_con_pwd, 0, 0);
    lv_obj_set_size(ui->screen_set_waln_cont_con_pwd, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_set_waln_cont_con_pwd, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_set_waln_cont_con_pwd, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_set_waln_cont_con_pwd, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_set_waln_cont_con_pwd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_cont_con_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_cont_con_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_cont_con_pwd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_cont_con_pwd, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_cont_con_pwd, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_cont_con_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_cont_con_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_cont_con_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_cont_con_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_cont_con_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_cont_title
    ui->screen_set_waln_cont_title = lv_obj_create(ui->screen_set_waln_cont_con_pwd);
    lv_obj_set_pos(ui->screen_set_waln_cont_title, 0, 0);
    lv_obj_set_size(ui->screen_set_waln_cont_title, 240, 40);
    lv_obj_set_scrollbar_mode(ui->screen_set_waln_cont_title, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_waln_cont_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_cont_title, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_waln_cont_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_waln_cont_title, lv_color_hex(0xc0c0c0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_waln_cont_title, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_cont_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_cont_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_cont_title, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_btn_connect
    ui->screen_set_waln_btn_connect = lv_button_create(ui->screen_set_waln_cont_title);
    lv_obj_set_pos(ui->screen_set_waln_btn_connect, 185, 9);
    lv_obj_set_size(ui->screen_set_waln_btn_connect, 50, 20);
    ui->screen_set_waln_btn_connect_label = lv_label_create(ui->screen_set_waln_btn_connect);
    lv_label_set_text(ui->screen_set_waln_btn_connect_label, "连接");
    lv_label_set_long_mode(ui->screen_set_waln_btn_connect_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_waln_btn_connect_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_waln_btn_connect, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_waln_btn_connect_label, LV_PCT(100));

    //Write style for screen_set_waln_btn_connect, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_btn_connect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_btn_connect, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_btn_connect, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_waln_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_btn_connect, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_btn_connect, lv_color_hex(0x0055ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_btn_connect, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_btn_connect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_btn_connect, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_btn_cancel
    ui->screen_set_waln_btn_cancel = lv_button_create(ui->screen_set_waln_cont_title);
    lv_obj_set_pos(ui->screen_set_waln_btn_cancel, 5, 10);
    lv_obj_set_size(ui->screen_set_waln_btn_cancel, 50, 20);
    ui->screen_set_waln_btn_cancel_label = lv_label_create(ui->screen_set_waln_btn_cancel);
    lv_label_set_text(ui->screen_set_waln_btn_cancel_label, "返回");
    lv_label_set_long_mode(ui->screen_set_waln_btn_cancel_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_waln_btn_cancel_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_waln_btn_cancel, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_waln_btn_cancel_label, LV_PCT(100));

    //Write style for screen_set_waln_btn_cancel, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_btn_cancel, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_btn_cancel, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_btn_cancel, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_waln_btn_cancel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_btn_cancel, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_btn_cancel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_btn_cancel, lv_color_hex(0x0055ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_btn_cancel, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_btn_cancel, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_btn_cancel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_label_titl
    ui->screen_set_waln_label_titl = lv_label_create(ui->screen_set_waln_cont_title);
    lv_obj_set_pos(ui->screen_set_waln_label_titl, 31, 12);
    lv_obj_set_size(ui->screen_set_waln_label_titl, 160, 16);
    lv_label_set_text(ui->screen_set_waln_label_titl, "");
    lv_label_set_long_mode(ui->screen_set_waln_label_titl, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_waln_label_titl, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_label_titl, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_label_titl, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_label_titl, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_label_titl, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_label_titl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_cont_inp_pwd
    ui->screen_set_waln_cont_inp_pwd = lv_obj_create(ui->screen_set_waln_cont_con_pwd);
    lv_obj_set_pos(ui->screen_set_waln_cont_inp_pwd, 0, 40);
    lv_obj_set_size(ui->screen_set_waln_cont_inp_pwd, 240, 40);
    lv_obj_set_scrollbar_mode(ui->screen_set_waln_cont_inp_pwd, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_waln_cont_inp_pwd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_cont_inp_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_cont_inp_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_cont_inp_pwd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_cont_inp_pwd, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_cont_inp_pwd, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_cont_inp_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_cont_inp_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_cont_inp_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_cont_inp_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_cont_inp_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_btn_pwd_show
    ui->screen_set_waln_btn_pwd_show = lv_button_create(ui->screen_set_waln_cont_inp_pwd);
    lv_obj_set_pos(ui->screen_set_waln_btn_pwd_show, 205, 10);
    lv_obj_set_size(ui->screen_set_waln_btn_pwd_show, 30, 20);
    ui->screen_set_waln_btn_pwd_show_label = lv_label_create(ui->screen_set_waln_btn_pwd_show);
    lv_label_set_text(ui->screen_set_waln_btn_pwd_show_label, "" LV_SYMBOL_EYE_OPEN " ");
    lv_label_set_long_mode(ui->screen_set_waln_btn_pwd_show_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_waln_btn_pwd_show_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_waln_btn_pwd_show, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_waln_btn_pwd_show_label, LV_PCT(100));

    //Write style for screen_set_waln_btn_pwd_show, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_btn_pwd_show, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_btn_pwd_show, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_btn_pwd_show, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_waln_btn_pwd_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_btn_pwd_show, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_btn_pwd_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_btn_pwd_show, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_btn_pwd_show, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_btn_pwd_show, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_btn_pwd_show, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_label_pwd
    ui->screen_set_waln_label_pwd = lv_label_create(ui->screen_set_waln_cont_inp_pwd);
    lv_obj_set_pos(ui->screen_set_waln_label_pwd, 5, 12);
    lv_obj_set_size(ui->screen_set_waln_label_pwd, 50, 16);
    lv_label_set_text(ui->screen_set_waln_label_pwd, "密码:");
    lv_label_set_long_mode(ui->screen_set_waln_label_pwd, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_waln_label_pwd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_label_pwd, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_label_pwd, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_label_pwd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_label_pwd, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_label_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_ta_pwd
    ui->screen_set_waln_ta_pwd = lv_textarea_create(ui->screen_set_waln_cont_inp_pwd);
    lv_obj_set_pos(ui->screen_set_waln_ta_pwd, 61, 4);
    lv_obj_set_size(ui->screen_set_waln_ta_pwd, 140, 30);
    lv_textarea_set_text(ui->screen_set_waln_ta_pwd, "");
    lv_textarea_set_placeholder_text(ui->screen_set_waln_ta_pwd, "");
    lv_textarea_set_password_bullet(ui->screen_set_waln_ta_pwd, "*");
    lv_textarea_set_password_mode(ui->screen_set_waln_ta_pwd, false);
    lv_textarea_set_one_line(ui->screen_set_waln_ta_pwd, true);
    lv_textarea_set_accepted_chars(ui->screen_set_waln_ta_pwd, "");
    lv_textarea_set_max_length(ui->screen_set_waln_ta_pwd, 16);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->screen_set_waln_ta_pwd, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for screen_set_waln_ta_pwd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_waln_ta_pwd, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_ta_pwd, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_ta_pwd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_waln_ta_pwd, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_ta_pwd, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_ta_pwd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_ta_pwd, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_ta_pwd, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_waln_ta_pwd, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_waln_ta_pwd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_waln_ta_pwd, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_waln_ta_pwd, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_ta_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_ta_pwd, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_ta_pwd, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_ta_pwd, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_ta_pwd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_set_waln_ta_pwd, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_ta_pwd, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_ta_pwd, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_ta_pwd, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_ta_pwd, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_cont_conn_info
    ui->screen_set_waln_cont_conn_info = lv_obj_create(ui->screen_set_waln);
    lv_obj_set_pos(ui->screen_set_waln_cont_conn_info, 40, 110);
    lv_obj_set_size(ui->screen_set_waln_cont_conn_info, 160, 100);
    lv_obj_set_scrollbar_mode(ui->screen_set_waln_cont_conn_info, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_set_waln_cont_conn_info, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_set_waln_cont_conn_info, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_set_waln_cont_conn_info, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_waln_cont_conn_info, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_waln_cont_conn_info, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_waln_cont_conn_info, lv_color_hex(0xdcdcdc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_waln_cont_conn_info, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_cont_conn_info, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_waln_cont_conn_info, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_cont_conn_info, lv_color_hex(0xededed), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_cont_conn_info, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_waln_cont_conn_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_waln_cont_conn_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_waln_cont_conn_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_waln_cont_conn_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_cont_conn_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_waln_textarea_con_info
    ui->screen_set_waln_textarea_con_info = lv_spangroup_create(ui->screen_set_waln_cont_conn_info);
    lv_obj_set_pos(ui->screen_set_waln_textarea_con_info, 8, 10);
    lv_obj_set_size(ui->screen_set_waln_textarea_con_info, 140, 50);
    lv_spangroup_set_align(ui->screen_set_waln_textarea_con_info, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_set_waln_textarea_con_info, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_set_waln_textarea_con_info, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_set_waln_textarea_con_info_span = lv_spangroup_new_span(ui->screen_set_waln_textarea_con_info);
    lv_span_set_text(ui->screen_set_waln_textarea_con_info_span, "连接失败,如确定不是密码问题,重启设备再重新连接");
    lv_style_set_text_color(lv_span_get_style(ui->screen_set_waln_textarea_con_info_span), lv_color_hex(0x000000));
    lv_style_set_text_decor(lv_span_get_style(ui->screen_set_waln_textarea_con_info_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->screen_set_waln_textarea_con_info_span), &lv_font_montserratMedium_12);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_waln_textarea_con_info_main_main_default
    static lv_style_t style_screen_set_waln_textarea_con_info_main_main_default;
    ui_init_style(&style_screen_set_waln_textarea_con_info_main_main_default);

    lv_style_set_border_width(&style_screen_set_waln_textarea_con_info_main_main_default, 2);
    lv_style_set_border_opa(&style_screen_set_waln_textarea_con_info_main_main_default, 255);
    lv_style_set_border_color(&style_screen_set_waln_textarea_con_info_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_border_side(&style_screen_set_waln_textarea_con_info_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_waln_textarea_con_info_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_set_waln_textarea_con_info_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_waln_textarea_con_info_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_set_waln_textarea_con_info_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_pad_top(&style_screen_set_waln_textarea_con_info_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_set_waln_textarea_con_info_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_set_waln_textarea_con_info_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_set_waln_textarea_con_info_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_set_waln_textarea_con_info_main_main_default, 0);
    lv_obj_add_style(ui->screen_set_waln_textarea_con_info, &style_screen_set_waln_textarea_con_info_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_set_waln_textarea_con_info);

    //Write codes screen_set_waln_btn_1
    ui->screen_set_waln_btn_1 = lv_button_create(ui->screen_set_waln_cont_conn_info);
    lv_obj_set_pos(ui->screen_set_waln_btn_1, 61, 73);
    lv_obj_set_size(ui->screen_set_waln_btn_1, 40, 20);
    ui->screen_set_waln_btn_1_label = lv_label_create(ui->screen_set_waln_btn_1);
    lv_label_set_text(ui->screen_set_waln_btn_1_label, "确定");
    lv_label_set_long_mode(ui->screen_set_waln_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_waln_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_waln_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_waln_btn_1_label, LV_PCT(100));

    //Write style for screen_set_waln_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_waln_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_waln_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_waln_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_waln_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_waln_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_waln_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_waln_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_waln_btn_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_waln_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_waln_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_set_waln.

    table = lv_table_create(ui->screen_set_waln_cont_waln_area);
    lv_obj_set_size(table, lv_pct(100), lv_pct(100));
    lv_obj_center(table);
    lv_obj_set_scroll_dir(table, LV_DIR_VER); /*垂直滚动*/
    lv_obj_set_scrollbar_mode(table, LV_SCROLLBAR_MODE_OFF);
    lv_obj_update_layout(table); /*更新布局*/

    uint16_t w = lv_obj_get_width(table);
    uint16_t h = lv_obj_get_height(table);
    lv_table_set_row_count(table, 0);
    lv_table_set_column_count(table, 1);
    lv_table_set_column_width(table, 0, w);

    /*添加的wifi*/
    
    UtilMid_RefleshList(table, LV_SYMBOL_WIFI, "vivo-Y53t", 0);
    UtilMid_RefleshList(table, LV_SYMBOL_WIFI, "vivo-Y54t", 1);
    UtilMid_RefleshList(table, LV_SYMBOL_WIFI, "过来呀", 2);
    lv_obj_add_event_cb(table, wifi_event_cb, LV_EVENT_VALUE_CHANGED, NULL);


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_set_waln);

    //Init events for screen.
    events_init_screen_set_waln(ui);

    Set_WALN_Init(ui);
}



/*添加WIFI 的UI到屏幕上，也就是有哪些WIFI都显示出来*/
lv_obj_t* WIFI_List_Add(uint8_t index,const char* wifiName,uint8_t isConnected)
{
    if(isConnected)
    {
        lv_table_set_cell_value_fmt(table, index, 0, LV_SYMBOL_WIFI"  %s  [connected]",wifiName);
    }
    else
    {
        lv_table_set_cell_value_fmt(table, index, 0, LV_SYMBOL_WIFI"    %s",wifiName);
    }
}


/*清除WIFI 列表的内容，即WIFI的图标 名称*/
void WIFI_List_Clear()
{
    //lv_obj_del(table);
    lv_table_set_row_count(table,0);
}


lv_timer_t* wifi_reflush_timer=NULL; /*WIFI 刷新定时器*/
lv_timer_t* main_timer=NULL;


/*WIFI UI初始化*/
static void Set_WALN_Init(lv_ui* ui)
{
    if(setHandle.isOpenWifi==1)
    {
        lv_obj_remove_flag(guider_ui.screen_set_waln_cont_waln_area, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_state(guider_ui.screen_set_waln_sw_waln,LV_STATE_CHECKED);

        setHandle.isReflush=1;
        wifi_reflush_timer=lv_timer_create(wifi_show_timer_handler,10000,NULL); /*第一次就触发，在里面修改定时周期*/
        main_timer= lv_timer_create(main_timer_handler,1000,NULL);
    }   

    lv_obj_add_event_cb(guider_ui.screen_set_waln_sw_waln,screen_set_waln_sw_waln_event_handler,LV_EVENT_VALUE_CHANGED,NULL);
}




/*用户点击了 WIFI 按钮后，进入该函数，用于显示WIFI连接界面 和暂停刷新*/
static void wifi_connect_event_handler(lv_event_t* e)
{
    uint32_t row = 0, col = 0;
    lv_obj_t* table=lv_event_get_target_obj(e);
    lv_table_get_selected_cell(table, &row, &col); /*获取点击的那个表格的单元格的行列*/
    const char* val=lv_table_get_cell_value(table, row, col); /*获取里面的值*/
    
    /*判断是否已连接*/
    if (strcmp(val + strlen(val) - 11, "[connected]") == 0)
    {
        printf("already con\n");
        return;
    }

    const char* wifi_name=val+5;
    printf("clicked (%d,%d),val=%s\n",row,col,wifi_name); /*加7是因为 LV_SYMBOL_XXX 占3个字节，后面还有四个空格，过了7字节后才是wifi名*/
    
    /*跳转到wifi 连接界面*/
    lv_obj_remove_flag(guider_ui.screen_set_waln_cont_con_pwd, LV_OBJ_FLAG_HIDDEN);
    /*显示要连接的wifi名*/
    lv_label_set_text(guider_ui.screen_set_waln_label_titl, val + 5);

    lv_timer_pause(wifi_reflush_timer); /*暂停wifi刷新定时器*/

}


/*自动刷新WIFI的定时器 并显示出来*/
static void wifi_show_timer_handler(lv_timer_t* t)
{
    printf("enter wifi reflush\n");
    
    if(setHandle.isOpenWifi==0 || setHandle.isExitWifi==1) 
    {
        if(setHandle.isExitWifi) setHandle.isExitWifi=0;
        printf("del wifi timer,%d\n",setHandle.isOpenWifi);
        lv_timer_del(main_timer);
        lv_timer_del(t); /*WIFI 关闭，这个自动刷新WIFI的定时器也销毁掉*/
        return;
    }
    setHandle.isReflush=1;

}



/*WIFI 定时 处理器。主要用来处理 WIFI任务的*/
static void main_timer_handler(lv_timer_t* t)
{
    if(setHandle.isExitPwd==1) /*退出界面*/
    {
        setHandle.isExitPwd=0;
        lv_timer_resume(wifi_reflush_timer); /*重新启动wifi 刷新定时器*/
    }
    else if(setHandle.isExitWifi==1)
    {
        setHandle.isExitWifi=0;
        lv_timer_del(wifi_reflush_timer); /*删除wifi 刷新定时器*/
    }
    else if(setHandle.isOverScanAp==1) /*扫描WIFI结束*/
    {
        setHandle.isOverScanAp=0;   
        
        /*获取WIFI 名*/
        WIFI_List_Clear();
        char wifiName[64]={0}; /*扫描的wifi名*/
        char* p=setHandle.wifiResponse;
        lv_obj_t* btn=NULL;

        printf("scan ok\n");

        uint16_t index=0;
        /*添加WIFI信息到界面。从扫描的响应中得到扫描的wifi名*/
        while((p=WIFI_APP_GetWifiName(p,wifiName,sizeof(wifiName)))!=NULL)
        {
            printf("%s\n",wifiName);
            if(strcmp(wifiName,setHandle.wifiName)==0) btn=WIFI_List_Add(index,wifiName,1);
            else btn=WIFI_List_Add(index,wifiName,0);
        }
        printf("over\n");
        
        //free(setHandle.wifiResponse);
    }
    else if(setHandle.isOverConnWifi)
    {
        setHandle.isOverConnWifi=0;
        
        /*显示连接结果*/
        lv_obj_remove_flag(guider_ui.screen_set_waln_cont_conn_info, LV_OBJ_FLAG_HIDDEN);
        
        lv_obj_t* spangroup_result=guider_ui.screen_set_waln_textarea_con_info;
        lv_span_t *first_span =lv_spangroup_get_child(spangroup_result,0);

        /*这里不置0，因为还需要这个标志位判断点了OK后，如果isConnectSuc=1则直接退出该连接界面
        *如果为0，则代表连接失败，继续停留该界面，以便继续输入密码连接
        */
        if(setHandle.isConnectSuc)  
        {
            lv_span_set_text(first_span,"connect suc");
        }
        else  lv_span_set_text(first_span,"connect fail");
    }
}


/*switch开关的打开wifi的时间函数*/
static void screen_set_waln_sw_waln_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) 
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;

        switch (status) {
        case (true):
        {
            lv_obj_remove_flag(guider_ui.screen_set_waln_cont_waln_area, LV_OBJ_FLAG_HIDDEN);
            setHandle.isOpenWifi=1;
            setHandle.isReflush=1;

            wifi_reflush_timer=lv_timer_create(wifi_show_timer_handler,10000,NULL); /*第一次就触发，在里面修改定时周期*/
            main_timer=lv_timer_create(main_timer_handler,1000,NULL);
            break;
        }
        case (false):
        {
            lv_obj_add_flag(guider_ui.screen_set_waln_cont_waln_area, LV_OBJ_FLAG_HIDDEN);
            setHandle.isOpenWifi=0;
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