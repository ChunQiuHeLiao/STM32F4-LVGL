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


static uint8_t Album_Name_Show(lv_ui* ui);

void setup_scr_screen_album(lv_ui *ui)
{
    printf("enter album\n");
    //Write codes screen_album
    ui->screen_album = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_album, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_album, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_album, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_album, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_album_btn_1
    ui->screen_album_btn_1 = lv_button_create(ui->screen_album);
    lv_obj_set_pos(ui->screen_album_btn_1, 5, 5);
    lv_obj_set_size(ui->screen_album_btn_1, 20, 20);
    lv_obj_add_flag(ui->screen_album_btn_1, LV_OBJ_FLAG_CHECKABLE);
    ui->screen_album_btn_1_label = lv_label_create(ui->screen_album_btn_1);
    lv_label_set_text(ui->screen_album_btn_1_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->screen_album_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_album_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_album_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_album_btn_1_label, LV_PCT(100));

    //Write style for screen_album_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_album_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_album_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_album_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_album_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_album_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_album_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_album_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_album_btn_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_album_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_album_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_album_label_1
    ui->screen_album_label_1 = lv_label_create(ui->screen_album);
    lv_obj_set_pos(ui->screen_album_label_1, 76, 9);
    lv_obj_set_size(ui->screen_album_label_1, 80, 20);
    lv_label_set_text(ui->screen_album_label_1, "Album\n");
    lv_label_set_long_mode(ui->screen_album_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_album_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_album_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_album_label_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_album_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_album_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_album_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_album_list_album
    ui->screen_album_list_album = lv_list_create(ui->screen_album);
    lv_obj_set_pos(ui->screen_album_list_album, 0, 30);
    lv_obj_set_size(ui->screen_album_list_album, 240, 290);
    lv_obj_set_scrollbar_mode(ui->screen_album_list_album, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for &style_screen_album_list_album_main_main_default
    static lv_style_t style_screen_album_list_album_main_main_default;
    ui_init_style(&style_screen_album_list_album_main_main_default);

    lv_style_set_pad_top(&style_screen_album_list_album_main_main_default, 5);
    lv_style_set_pad_left(&style_screen_album_list_album_main_main_default, 5);
    lv_style_set_pad_right(&style_screen_album_list_album_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_album_list_album_main_main_default, 5);
    lv_style_set_bg_opa(&style_screen_album_list_album_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_album_list_album_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_album_list_album_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_album_list_album_main_main_default, 0);
    lv_style_set_radius(&style_screen_album_list_album_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_album_list_album_main_main_default, 0);
    lv_obj_add_style(ui->screen_album_list_album, &style_screen_album_list_album_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_album_list_album_main_scrollbar_default
    static lv_style_t style_screen_album_list_album_main_scrollbar_default;
    ui_init_style(&style_screen_album_list_album_main_scrollbar_default);

    lv_style_set_radius(&style_screen_album_list_album_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_album_list_album_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_album_list_album_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_album_list_album_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_album_list_album, &style_screen_album_list_album_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_album_list_album_extra_btns_main_default
    static lv_style_t style_screen_album_list_album_extra_btns_main_default;
    ui_init_style(&style_screen_album_list_album_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_album_list_album_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_screen_album_list_album_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_screen_album_list_album_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_album_list_album_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_screen_album_list_album_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_screen_album_list_album_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_album_list_album_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_album_list_album_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_album_list_album_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_album_list_album_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_album_list_album_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_album_list_album_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_screen_album_list_album_extra_texts_main_default
    static lv_style_t style_screen_album_list_album_extra_texts_main_default;
    ui_init_style(&style_screen_album_list_album_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_album_list_album_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_screen_album_list_album_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_screen_album_list_album_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_album_list_album_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_screen_album_list_album_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_album_list_album_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_album_list_album_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_album_list_album_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_album_list_album_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_album_list_album_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_album_list_album_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_album_list_album_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_album_list_album_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //The custom code of screen_album.
    printf("album1\n");
    Album_Name_Show(ui);

    //Update current screen layout.
    lv_obj_update_layout(ui->screen_album);

    //Init events for screen.
    events_init_screen_album(ui);
}



/*********自定义代码********/


/*主要处理点击了一个视频按钮后，跳转到播放页面*/
static void Album_Btn_Event_Handler(lv_event_t* e)
{
    lv_obj_t* btn = lv_event_get_target(e);
    lv_obj_t* vedio_text = lv_obj_get_child(btn, 1); /*得到视频名字。 索引0是文字符号，1是文本*/

    strcpy(albumHandle.name, lv_label_get_text(vedio_text)); /*这里返回是内部的缓存指针*/
    printf("img name:%s,%d\n", albumHandle.name, lv_obj_get_child_count(btn));

    /*记录下这张图片的索引值*/
    uint16_t index=lv_obj_get_index(btn);
    Album_SetImgIndex(index);

    /*这里会清除 lv_label_get_text 返回的指针呀，所以后面是野指针呀*/
    ui_load_scr_no_animation(&guider_ui, &guider_ui.screen_album_show, guider_ui.screen_album_show_del, &guider_ui.screen_album_del, setup_scr_screen_album_show);


    /*修改图片名称*/
    lv_label_set_text(guider_ui.screen_album_show_label_img_name, albumHandle.name);
    albumHandle.isDispImg=1;
}


/*在列表里显示当前视频文件夹有哪些视频*/
static uint8_t Album_Name_Show(lv_ui* ui)
{
    lv_fs_dir_t dir;
    lv_fs_res_t res = 0;
    res = lv_fs_dir_open(&dir, IMG_DIR);
    if (res)
    {
        printf("open dir fail:%d\n", res);
        return res;
    }

    /*读取目录下的文件*/
    char fileName[32] = { 0 };
    lv_obj_t* list = ui->screen_album_list_album;
    lv_obj_t* btn = NULL;
    uint16_t img_num = 0;
    while (1)
    {
        res = lv_fs_dir_read(&dir, fileName, sizeof(fileName));
        if (res)
        {
            printf("read dir fail:%d\n", res);
            break;
        }

        if (fileName[0] == 0) break; /*代表该目录文件读取完毕,获取判断该数组长度为0*/

        /*把文件内容加载到里面*/
        btn = lv_list_add_button(list, LV_SYMBOL_IMAGE, fileName);
        lv_obj_add_event_cb(btn, Album_Btn_Event_Handler, LV_EVENT_CLICKED, ui);
        img_num++;
    }
    Album_SetImgNum(img_num);

    printf("img_num:%d\n", img_num);

    lv_fs_dir_close(&dir);
    return res;
}
