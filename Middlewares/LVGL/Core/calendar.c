#include"Core/main_lib.h"


static void Calendar_EventHandler(lv_event_t* e)
{

}



void Calendar()
{
    /* 1. 随便创建一个日历 */
    lv_obj_t* calendar = lv_calendar_create(lv_screen_active());
    lv_obj_set_size(calendar, scr_w, scr_h);
    lv_obj_center(calendar);

    lv_calendar_set_today_date(calendar, 2025, 11, 29);
    lv_calendar_set_showed_date(calendar, 2025, 11);
    //lv_calendar_header_dropdown_create(calendar);
    lv_calendar_set_chinese_mode(calendar, true);
    lv_obj_set_style_text_font(calendar, &lv_font_montserrat_14, LV_PART_MAIN);

    lv_obj_t* exit_btn=App_CreateExitBtn(NULL);
    lv_obj_set_size(exit_btn, 0.1 * scr_w, 0.06 * scr_h);
    lv_obj_align(exit_btn, LV_ALIGN_TOP_LEFT, 0, 4);
}
