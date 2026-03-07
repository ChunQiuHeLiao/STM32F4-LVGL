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



void Screen_Album_Show_UI_Init(lv_ui* ui);

void setup_scr_screen_album_show(lv_ui *ui)
{
    //Write codes screen_album_show
    ui->screen_album_show = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_album_show, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_album_show, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_album_show, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_album_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_album_show_cont_img_show
    ui->screen_album_show_cont_img_show = lv_obj_create(ui->screen_album_show);
    lv_obj_set_pos(ui->screen_album_show_cont_img_show, 0, 30);
    lv_obj_set_size(ui->screen_album_show_cont_img_show, 240, 290);
    lv_obj_set_scrollbar_mode(ui->screen_album_show_cont_img_show, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_album_show_cont_img_show, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_album_show_cont_img_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_album_show_cont_img_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_album_show_cont_img_show, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_album_show_cont_img_show, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_album_show_cont_img_show, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_album_show_cont_img_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_album_show_cont_img_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_album_show_cont_img_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_album_show_cont_img_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_album_show_cont_img_show, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_album_show_cont_title
    ui->screen_album_show_cont_title = lv_obj_create(ui->screen_album_show);
    lv_obj_set_pos(ui->screen_album_show_cont_title, 0, 0);
    lv_obj_set_size(ui->screen_album_show_cont_title, 240, 30);
    lv_obj_set_scrollbar_mode(ui->screen_album_show_cont_title, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_album_show_cont_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_album_show_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_album_show_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_album_show_cont_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_album_show_cont_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_album_show_cont_title, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_album_show_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_album_show_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_album_show_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_album_show_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_album_show_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_album_show_btn_exit
    ui->screen_album_show_btn_exit = lv_button_create(ui->screen_album_show_cont_title);
    lv_obj_set_pos(ui->screen_album_show_btn_exit, 5, 8);
    lv_obj_set_size(ui->screen_album_show_btn_exit, 20, 12);
    lv_obj_add_flag(ui->screen_album_show_btn_exit, LV_OBJ_FLAG_CHECKABLE);
    ui->screen_album_show_btn_exit_label = lv_label_create(ui->screen_album_show_btn_exit);
    lv_label_set_text(ui->screen_album_show_btn_exit_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->screen_album_show_btn_exit_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_album_show_btn_exit_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_album_show_btn_exit, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_album_show_btn_exit_label, LV_PCT(100));

    //Write style for screen_album_show_btn_exit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_album_show_btn_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_album_show_btn_exit, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_album_show_btn_exit, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_album_show_btn_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_album_show_btn_exit, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_album_show_btn_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_album_show_btn_exit, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_album_show_btn_exit, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_album_show_btn_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_album_show_btn_exit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_album_show_label_img_name
    ui->screen_album_show_label_img_name = lv_label_create(ui->screen_album_show_cont_title);
    lv_obj_set_pos(ui->screen_album_show_label_img_name, 60, 8);
    lv_obj_set_size(ui->screen_album_show_label_img_name, 120, 12);
    lv_label_set_text(ui->screen_album_show_label_img_name, "baizi.bin\n");
    lv_label_set_long_mode(ui->screen_album_show_label_img_name, LV_LABEL_LONG_WRAP);

    //Write style for screen_album_show_label_img_name, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_album_show_label_img_name, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_album_show_label_img_name, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_album_show_label_img_name, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_album_show_label_img_name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_album_show_label_img_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_album_show_btn_show_img_info
    ui->screen_album_show_btn_show_img_info = lv_button_create(ui->screen_album_show_cont_title);
    lv_obj_set_pos(ui->screen_album_show_btn_show_img_info, 200, 10);
    lv_obj_set_size(ui->screen_album_show_btn_show_img_info, 20, 20);
    ui->screen_album_show_btn_show_img_info_label = lv_label_create(ui->screen_album_show_btn_show_img_info);
    lv_label_set_text(ui->screen_album_show_btn_show_img_info_label, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_album_show_btn_show_img_info_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_album_show_btn_show_img_info_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_album_show_btn_show_img_info, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_album_show_btn_show_img_info_label, LV_PCT(100));

    //Write style for screen_album_show_btn_show_img_info, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_album_show_btn_show_img_info, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_album_show_btn_show_img_info, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_album_show_btn_show_img_info, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_album_show_btn_show_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_album_show_btn_show_img_info, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_album_show_btn_show_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_album_show_btn_show_img_info, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_album_show_btn_show_img_info, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_album_show_btn_show_img_info, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_album_show_btn_show_img_info, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

   
    
    //Write codes screen_album_show_cont_img_info
    ui->screen_album_show_cont_img_info = lv_obj_create(ui->screen_album_show);
    lv_obj_set_pos(ui->screen_album_show_cont_img_info, 50, 50);
    lv_obj_set_size(ui->screen_album_show_cont_img_info, 160, 160);
    lv_obj_set_scrollbar_mode(ui->screen_album_show_cont_img_info, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_album_show_cont_img_info, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_album_show_cont_img_info, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_album_show_cont_img_info, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_album_show_cont_img_info, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_album_show_cont_img_info, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_album_show_cont_img_info, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_album_show_cont_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_album_show_cont_img_info, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_album_show_cont_img_info, lv_color_hex(0xd2d2d2), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_album_show_cont_img_info, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_album_show_cont_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_album_show_cont_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_album_show_cont_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_album_show_cont_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_album_show_cont_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_album_show_label_img_info
    ui->screen_album_show_label_img_info = lv_label_create(ui->screen_album_show_cont_img_info);
    lv_obj_set_pos(ui->screen_album_show_label_img_info, 10, 10);
    lv_obj_set_size(ui->screen_album_show_label_img_info, 140, 140);
    lv_label_set_text(ui->screen_album_show_label_img_info, "None");
    lv_label_set_long_mode(ui->screen_album_show_label_img_info, LV_LABEL_LONG_CLIP);

    //Write style for screen_album_show_label_img_info, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_album_show_label_img_info, lv_color_hex(0x0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_album_show_label_img_info, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_album_show_label_img_info, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_album_show_label_img_info, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_album_show_label_img_info, 0, LV_PART_MAIN|LV_STATE_DEFAULT);


    //The custom code of screen_album_show.
    Screen_Album_Show_UI_Init(ui);

    //Update current screen layout.
    lv_obj_update_layout(ui->screen_album_show);

    //Init events for screen.
    events_init_screen_album_show(ui);

    lv_label_set_text(guider_ui.screen_album_show_label_img_name,albumHandle.name); /*修改图片标题*/
}


int img_index = 0;

static void Album_Show_EventHandler(lv_event_t* e)
{
    const char* str = (const char*)lv_event_get_user_data(e);
    if (strcmp(str, "scroll") == 0)
    {
        if(albumHandle.openMethod==2) return; /*如果是第三方应用使用相册，则只能查看此图*/
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act()); /*获取手势方向*/
        if (dir == LV_DIR_RIGHT)
        {
            printf("right\n");
            if (img_index == 0) return; /*到顶部一张*/
            img_index--;
        }
        else if (dir == LV_DIR_LEFT)
        {
            printf("left\n");
            if ((img_index+1) == Album_GetImgNum()) return; /*达到最后一张*/
            img_index++;
        }

        //albumHandle.isDispImg=1; //再次播放图片，消除显示的画面
        /*判断是哪种打开方式，是不是别的地方打开的*/

        albumHandle.isDispImg=1;
        albumHandle.index=img_index;

        lv_obj_add_flag(guider_ui.screen_album_show_cont_img_info,LV_OBJ_FLAG_HIDDEN);
        lv_obj_t* label_show_img_info=lv_obj_get_child(guider_ui.screen_album_show_btn_show_img_info,0);
        lv_label_set_text(label_show_img_info,LV_SYMBOL_DOWN);

        ML_GetFileName_By_Index(albumHandle.index,IMG_DIR,albumHandle.name,sizeof(albumHandle.name));
        lv_label_set_text(guider_ui.screen_album_show_label_img_name,albumHandle.name); /*修改图片标题*/
    }
}



void Screen_Album_Show_UI_Init(lv_ui* ui)
{
    lv_obj_t* screen = ui->screen_album_show;

    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE); // 关屏幕滚动
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF); // 不想看到条

    // lv_obj_t* widget = lv_obj_create(screen);
    // lv_obj_set_size(widget, scr_w + 5, scr_h-30);
    // lv_obj_set_pos(widget,0,30);
    // lv_obj_set_style_bg_color(widget, lv_color_hex(0xffffff), 0);

    lv_obj_add_event_cb(screen, Album_Show_EventHandler, LV_EVENT_GESTURE, (void*)"scroll");

    img_index = Album_GetImgIndex();
    Album_SetImgIndex(img_index); //设置索引值
}
