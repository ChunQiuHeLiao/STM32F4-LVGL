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
#include"ui.h"


void setup_scr_screen_main(lv_ui *ui)
{
    ui_ScrMain_screen_init();
    guider_ui.screen_main = ui_ScrMain;
    return;

    //Write codes screen_main
    ui->screen_main = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_main, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_main, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_cont_app
    ui->screen_main_cont_app = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_cont_app, 0, 40);
    lv_obj_set_size(ui->screen_main_cont_app, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_main_cont_app, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_main_cont_app, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_cont_app, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_main_cont_app, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_main_cont_app, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_main_cont_app, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_cont_app, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_cont_app, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_main_cont_app, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_main_cont_app, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_cont_app, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_cont_app, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_cont_app, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_cont_app, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_cont_app, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_imgbtn_camera
    ui->screen_main_imgbtn_camera = lv_imagebutton_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_imgbtn_camera, 184, 74);
    lv_obj_set_size(ui->screen_main_imgbtn_camera, 40, 40);
    lv_obj_add_flag(ui->screen_main_imgbtn_camera, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imagebutton_set_src(ui->screen_main_imgbtn_camera, LV_IMAGEBUTTON_STATE_RELEASED, "D:\\Program\\ProjectDevelopmentFolder\\LVGL\\Setting\\Setting\\import\\image\\camera.jpg", NULL, NULL);
#else
    lv_imagebutton_set_src(ui->screen_main_imgbtn_camera, LV_IMAGEBUTTON_STATE_RELEASED, "S:img/camera.jpg", NULL, NULL);
#endif
    ui->screen_main_imgbtn_camera_label = lv_label_create(ui->screen_main_imgbtn_camera);
    lv_label_set_text(ui->screen_main_imgbtn_camera_label, "");
    lv_label_set_long_mode(ui->screen_main_imgbtn_camera_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_imgbtn_camera_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_imgbtn_camera, 0, LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_camera, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_camera, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_camera, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_camera, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_imgbtn_camera, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_camera, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_camera, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_camera, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_camera, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_camera, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_camera, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_camera, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_camera, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_main_imgbtn_camera, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_camera, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_camera, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_camera, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_camera, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_camera, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_camera, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_main_imgbtn_camera, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_camera, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_camera, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_main_imgbtn_music
    ui->screen_main_imgbtn_music = lv_imagebutton_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_imgbtn_music, 128, 70);
    lv_obj_set_size(ui->screen_main_imgbtn_music, 40, 40);
    lv_obj_add_flag(ui->screen_main_imgbtn_music, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imagebutton_set_src(ui->screen_main_imgbtn_music, LV_IMAGEBUTTON_STATE_RELEASED, "D:\\Program\\ProjectDevelopmentFolder\\LVGL\\Setting\\Setting\\import\\image\\music.jpg", NULL, NULL);
#else
    lv_imagebutton_set_src(ui->screen_main_imgbtn_music, LV_IMAGEBUTTON_STATE_RELEASED, "S:img/music.jpg", NULL, NULL);
#endif
    ui->screen_main_imgbtn_music_label = lv_label_create(ui->screen_main_imgbtn_music);
    lv_label_set_text(ui->screen_main_imgbtn_music_label, "");
    lv_label_set_long_mode(ui->screen_main_imgbtn_music_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_imgbtn_music_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_imgbtn_music, 0, LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_music, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_music, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_music, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_imgbtn_music, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_music, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_music, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_music, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_music, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_music, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_music, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_music, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_main_imgbtn_music, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_music, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_music, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_music, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_music, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_music, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_music, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_main_imgbtn_music, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_music, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_music, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_main_imgbtn_vedio
    ui->screen_main_imgbtn_vedio = lv_imagebutton_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_imgbtn_vedio, 72, 70);
    lv_obj_set_size(ui->screen_main_imgbtn_vedio, 40, 40);
    lv_obj_add_flag(ui->screen_main_imgbtn_vedio, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imagebutton_set_src(ui->screen_main_imgbtn_vedio, LV_IMAGEBUTTON_STATE_RELEASED, "D:\\Program\\ProjectDevelopmentFolder\\LVGL\\Setting\\Setting\\import\\image\\vedio.jpg", NULL, NULL);
#else
    lv_imagebutton_set_src(ui->screen_main_imgbtn_vedio, LV_IMAGEBUTTON_STATE_RELEASED, "S:img/vedio.jpg", NULL, NULL);
#endif
    ui->screen_main_imgbtn_vedio_label = lv_label_create(ui->screen_main_imgbtn_vedio);
    lv_label_set_text(ui->screen_main_imgbtn_vedio_label, "");
    lv_label_set_long_mode(ui->screen_main_imgbtn_vedio_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_imgbtn_vedio_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_imgbtn_vedio, 0, LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_vedio, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_vedio, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_vedio, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_vedio, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_imgbtn_vedio, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_vedio, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_vedio, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_vedio, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_vedio, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_vedio, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_vedio, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_vedio, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_vedio, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_main_imgbtn_vedio, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_vedio, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_vedio, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_vedio, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_vedio, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_vedio, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_vedio, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_main_imgbtn_vedio, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_vedio, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_vedio, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_main_imgbtn_album
    ui->screen_main_imgbtn_album = lv_imagebutton_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_imgbtn_album, 16, 70);
    lv_obj_set_size(ui->screen_main_imgbtn_album, 40, 40);
    lv_obj_add_flag(ui->screen_main_imgbtn_album, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imagebutton_set_src(ui->screen_main_imgbtn_album, LV_IMAGEBUTTON_STATE_RELEASED, "D:\\Program\\ProjectDevelopmentFolder\\LVGL\\Setting\\Setting\\import\\image\\pic.jpg", NULL, NULL);
#else
    lv_imagebutton_set_src(ui->screen_main_imgbtn_album, LV_IMAGEBUTTON_STATE_RELEASED, "S:img/pic.jpg", NULL, NULL);
#endif
    ui->screen_main_imgbtn_album_label = lv_label_create(ui->screen_main_imgbtn_album);
    lv_label_set_text(ui->screen_main_imgbtn_album_label, "");
    lv_label_set_long_mode(ui->screen_main_imgbtn_album_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_imgbtn_album_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_imgbtn_album, 0, LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_album, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_album, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_album, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_album, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_imgbtn_album, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_album, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_album, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_album, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_album, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_album, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_album, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_album, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_album, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_main_imgbtn_album, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_album, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_album, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_album, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_album, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_album, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_album, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_main_imgbtn_album, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_album, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_album, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_main_imgbtn_calendar
    ui->screen_main_imgbtn_calendar = lv_imagebutton_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_imgbtn_calendar, 72, 0);
    lv_obj_set_size(ui->screen_main_imgbtn_calendar, 40, 40);
    lv_obj_add_flag(ui->screen_main_imgbtn_calendar, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imagebutton_set_src(ui->screen_main_imgbtn_calendar, LV_IMAGEBUTTON_STATE_RELEASED, "D:\\Program\\ProjectDevelopmentFolder\\LVGL\\Setting\\Setting\\import\\image\\calender.jpg", NULL, NULL);
#else
    lv_imagebutton_set_src(ui->screen_main_imgbtn_calendar, LV_IMAGEBUTTON_STATE_RELEASED, "S:img/calender.jpg", NULL, NULL);
#endif
    ui->screen_main_imgbtn_calendar_label = lv_label_create(ui->screen_main_imgbtn_calendar);
    lv_label_set_text(ui->screen_main_imgbtn_calendar_label, "");
    lv_label_set_long_mode(ui->screen_main_imgbtn_calendar_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_imgbtn_calendar_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_imgbtn_calendar, 0, LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_calendar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_calendar, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_calendar, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_calendar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_imgbtn_calendar, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_calendar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_calendar, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_calendar, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_calendar, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_calendar, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_calendar, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_calendar, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_calendar, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_main_imgbtn_calendar, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_calendar, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_calendar, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_calendar, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_calendar, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_calendar, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_calendar, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_main_imgbtn_calendar, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_calendar, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_calendar, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_main_imgbtn_weather
    ui->screen_main_imgbtn_weather = lv_imagebutton_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_imgbtn_weather, 128, 0);
    lv_obj_set_size(ui->screen_main_imgbtn_weather, 40, 40);
    lv_obj_add_flag(ui->screen_main_imgbtn_weather, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imagebutton_set_src(ui->screen_main_imgbtn_weather, LV_IMAGEBUTTON_STATE_RELEASED, "D:\\Program\\ProjectDevelopmentFolder\\LVGL\\Setting\\Setting\\import\\image\\weather.jpg", NULL, NULL);
#else
    lv_imagebutton_set_src(ui->screen_main_imgbtn_weather, LV_IMAGEBUTTON_STATE_RELEASED, "S:img/weather.jpg", NULL, NULL);
#endif
    ui->screen_main_imgbtn_weather_label = lv_label_create(ui->screen_main_imgbtn_weather);
    lv_label_set_text(ui->screen_main_imgbtn_weather_label, "");
    lv_label_set_long_mode(ui->screen_main_imgbtn_weather_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_imgbtn_weather_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_imgbtn_weather, 0, LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_weather, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_weather, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_imgbtn_weather, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_weather, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_weather, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_weather, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_weather, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_weather, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_weather, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_weather, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_main_imgbtn_weather, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_weather, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_weather, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_weather, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_weather, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_weather, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_weather, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_main_imgbtn_weather, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_weather, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_weather, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_main_imgbtn_file
    ui->screen_main_imgbtn_file = lv_imagebutton_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_imgbtn_file, 184, 0);
    lv_obj_set_size(ui->screen_main_imgbtn_file, 40, 40);
    lv_obj_add_flag(ui->screen_main_imgbtn_file, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imagebutton_set_src(ui->screen_main_imgbtn_file, LV_IMAGEBUTTON_STATE_RELEASED, "D:\\Program\\ProjectDevelopmentFolder\\LVGL\\Setting\\Setting\\import\\image\\file_manager.jpg", NULL, NULL);
#else
    lv_imagebutton_set_src(ui->screen_main_imgbtn_file, LV_IMAGEBUTTON_STATE_RELEASED, "S:img/file_manager.jpg", NULL, NULL);
#endif
    ui->screen_main_imgbtn_file_label = lv_label_create(ui->screen_main_imgbtn_file);
    lv_label_set_text(ui->screen_main_imgbtn_file_label, "");
    lv_label_set_long_mode(ui->screen_main_imgbtn_file_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_imgbtn_file_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_imgbtn_file, 0, LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_file, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_file, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_file, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_file, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_imgbtn_file, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_file, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_file, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_file, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_file, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_file, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_file, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_file, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_file, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_main_imgbtn_file, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_file, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_file, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_file, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_file, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_file, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_file, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_main_imgbtn_file, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_file, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_file, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_main_imgbtn_setting
    ui->screen_main_imgbtn_setting = lv_imagebutton_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_imgbtn_setting, 16, 0);
    lv_obj_set_size(ui->screen_main_imgbtn_setting, 40, 40);
    lv_obj_add_flag(ui->screen_main_imgbtn_setting, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imagebutton_set_src(ui->screen_main_imgbtn_setting, LV_IMAGEBUTTON_STATE_RELEASED, "D:\\Program\\ProjectDevelopmentFolder\\LVGL\\Setting\\Setting\\import\\image\\setting.jpg", NULL, NULL);
#else
    lv_imagebutton_set_src(ui->screen_main_imgbtn_setting, LV_IMAGEBUTTON_STATE_RELEASED, "S:img/setting.jpg", NULL, NULL);
#endif
    ui->screen_main_imgbtn_setting_label = lv_label_create(ui->screen_main_imgbtn_setting);
    lv_label_set_text(ui->screen_main_imgbtn_setting_label, "");
    lv_label_set_long_mode(ui->screen_main_imgbtn_setting_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_imgbtn_setting_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_imgbtn_setting, 0, LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_setting, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_setting, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_imgbtn_setting, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_main_imgbtn_setting, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_setting, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_setting, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_setting, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_setting, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_setting, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_setting, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_main_imgbtn_setting, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_setting, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_setting, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_main_imgbtn_setting, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_main_imgbtn_setting, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_main_imgbtn_setting, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_main_imgbtn_setting, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_main_imgbtn_setting, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_imgbtn_setting, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_main_imgbtn_setting, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_main_label_1
    ui->screen_main_label_1 = lv_label_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_label_1, 8, 42);
    lv_obj_set_size(ui->screen_main_label_1, 56, 24);
    lv_label_set_text(ui->screen_main_label_1, "setting");
    lv_label_set_long_mode(ui->screen_main_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_2
    ui->screen_main_label_2 = lv_label_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_label_2, 64, 43);
    lv_obj_set_size(ui->screen_main_label_2, 56, 24);
    lv_label_set_text(ui->screen_main_label_2, "calendar");
    lv_label_set_long_mode(ui->screen_main_label_2, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_3
    ui->screen_main_label_3 = lv_label_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_label_3, 120, 43);
    lv_obj_set_size(ui->screen_main_label_3, 56, 24);
    lv_label_set_text(ui->screen_main_label_3, "weather");
    lv_label_set_long_mode(ui->screen_main_label_3, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_3, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_4
    ui->screen_main_label_4 = lv_label_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_label_4, 176, 43);
    lv_obj_set_size(ui->screen_main_label_4, 56, 24);
    lv_label_set_text(ui->screen_main_label_4, "file expolor");
    lv_label_set_long_mode(ui->screen_main_label_4, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_4, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_8
    ui->screen_main_label_8 = lv_label_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_label_8, 173, 114);
    lv_obj_set_size(ui->screen_main_label_8, 56, 24);
    lv_label_set_text(ui->screen_main_label_8, "camera");
    lv_label_set_long_mode(ui->screen_main_label_8, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_8, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_8, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_7
    ui->screen_main_label_7 = lv_label_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_label_7, 118, 114);
    lv_obj_set_size(ui->screen_main_label_7, 56, 24);
    lv_label_set_text(ui->screen_main_label_7, "music");
    lv_label_set_long_mode(ui->screen_main_label_7, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_7, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_6
    ui->screen_main_label_6 = lv_label_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_label_6, 63, 114);
    lv_obj_set_size(ui->screen_main_label_6, 56, 24);
    lv_label_set_text(ui->screen_main_label_6, "vedio");
    lv_label_set_long_mode(ui->screen_main_label_6, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_6, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_5
    ui->screen_main_label_5 = lv_label_create(ui->screen_main_cont_app);
    lv_obj_set_pos(ui->screen_main_label_5, 8, 114);
    lv_obj_set_size(ui->screen_main_label_5, 56, 24);
    lv_label_set_text(ui->screen_main_label_5, "album");
    lv_label_set_long_mode(ui->screen_main_label_5, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_main_label_5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_5, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_cont_icon
    ui->screen_main_cont_icon = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_cont_icon, 0, 0);
    lv_obj_set_size(ui->screen_main_cont_icon, 240, 40);
    lv_obj_set_scrollbar_mode(ui->screen_main_cont_icon, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_main_cont_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_cont_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_cont_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_cont_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_main_cont_icon, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_main_cont_icon, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_cont_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_cont_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_cont_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_cont_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_cont_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_main.

    lv_obj_update_layout(ui->screen_main);

    //Init events for screen.
    events_init_screen_main(ui);
}
