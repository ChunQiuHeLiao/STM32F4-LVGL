/**
 * @file lvgl.h
 * Include all LVGL related headers
 */

#ifndef LVGL_H
#define LVGL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************
 * CURRENT VERSION OF LVGL
 ***************************/
#include "lv_version.h"

/*********************
 *      INCLUDES
 *********************/
#include "lv_init.h"
#include"string.h"

// #include "lv_mem.h"
// #include "lv_string.h"
// #include "lv_sprintf.h"

#include "lv_log.h"
#include "lv_timer.h"
#include "lv_math.h"
#include "lv_array.h"
#include "lv_async.h"
#include "lv_anim_timeline.h"
#include "lv_profiler_builtin.h"
#include "lv_rb.h"
#include "lv_utils.h"

#include "lv_tick.h"

#include "lv_obj.h"
#include "lv_group.h"
#include "lv_indev.h"
#include "lv_refr.h"
#include "lv_display.h"

#include "lv_font.h"
#include "lv_binfont_loader.h"
#include "lv_font_fmt_txt.h"

#include "widgets/animimage/lv_animimage.h"
#include "widgets/arc/lv_arc.h"
#include "widgets/bar/lv_bar.h"
#include "widgets/button/lv_button.h"
#include "widgets/buttonmatrix/lv_buttonmatrix.h"
#include "widgets/calendar/lv_calendar.h"
#include "widgets/canvas/lv_canvas.h"
#include "widgets/chart/lv_chart.h"
#include "widgets/checkbox/lv_checkbox.h"
#include "widgets/dropdown/lv_dropdown.h"
#include "widgets/image/lv_image.h"
#include "widgets/imagebutton/lv_imagebutton.h"
#include "widgets/keyboard/lv_keyboard.h"
#include "widgets/label/lv_label.h"
#include "widgets/led/lv_led.h"
#include "widgets/line/lv_line.h"
#include "widgets/list/lv_list.h"
#include "widgets/lottie/lv_lottie.h"
#include "widgets/menu/lv_menu.h"
#include "widgets/msgbox/lv_msgbox.h"
#include "widgets/roller/lv_roller.h"
#include "widgets/scale/lv_scale.h"
#include "widgets/slider/lv_slider.h"
#include "widgets/span/lv_span.h"
#include "widgets/spinbox/lv_spinbox.h"
#include "widgets/spinner/lv_spinner.h"
#include "widgets/switch/lv_switch.h"
#include "widgets/table/lv_table.h"
#include "widgets/tabview/lv_tabview.h"
#include "widgets/textarea/lv_textarea.h"
#include "widgets/tileview/lv_tileview.h"
#include "widgets/win/lv_win.h"

#include "./lv_snapshot.h"
#include "lv_sysmon.h"
#include "lv_monkey.h"
#include "lv_gridnav.h"
#include "lv_fragment.h"
#include "lv_imgfont.h"
#include "lv_observer.h"
#include "lv_ime_pinyin.h"
#include "lv_file_explorer.h"

#include "lv_barcode.h"
#include "lv_bin_decoder.h"
#include "lv_bmp.h"
#include "lv_rle.h"
#include "lv_fsdrv.h"
#include "lv_lodepng.h"
#include "lv_libpng.h"
#include "lv_gif.h"
#include "lv_qrcode.h"
#include "lv_tjpgd.h"
#include "lv_libjpeg_turbo.h"
#include "lv_freetype.h"
#include "lv_rlottie.h"
#include "lv_ffmpeg.h"
#include "lv_tiny_ttf.h"

#include "lv_layout.h"

#include "lv_draw.h"
#include "lv_draw_buf.h"
#include "lv_draw_vector.h"
#include "sw/lv_draw_sw.h"

#include "lv_theme.h"

#include "lv_drivers.h"

// #include "lv_api_map_v8.h"
// #include "lv_api_map_v9_0.h"
// #include "lv_api_map_v9_1.h"

#if LV_USE_PRIVATE_API
#include "lvgl_private.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/** Gives 1 if the x.y.z version is supported in the current version
 * Usage:
 *
 * - Require v6
 * #if LV_VERSION_CHECK(6,0,0)
 *   new_func_in_v6();
 * #endif
 *
 *
 * - Require at least v5.3
 * #if LV_VERSION_CHECK(5,3,0)
 *   new_feature_from_v5_3();
 * #endif
 *
 *
 * - Require v5.3.2 bugfixes
 * #if LV_VERSION_CHECK(5,3,2)
 *   bugfix_in_v5_3_2();
 * #endif
 *
 */
#define LV_VERSION_CHECK(x,y,z) (x == LVGL_VERSION_MAJOR && (y < LVGL_VERSION_MINOR || (y == LVGL_VERSION_MINOR && z <= LVGL_VERSION_PATCH)))

/**
 * Wrapper functions for VERSION macros
 */

static inline int lv_version_major(void)
{
    return LVGL_VERSION_MAJOR;
}

static inline int lv_version_minor(void)
{
    return LVGL_VERSION_MINOR;
}

static inline int lv_version_patch(void)
{
    return LVGL_VERSION_PATCH;
}

static inline const char * lv_version_info(void)
{
    return LVGL_VERSION_INFO;
}

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_H*/
