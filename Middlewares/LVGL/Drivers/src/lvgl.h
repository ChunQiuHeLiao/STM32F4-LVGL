/**
 * @file lvgl.h
 * This file exists only to be compatible with Arduino's library structure
 */

#ifndef LVGL_SRC_H
#define LVGL_SRC_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl.h"
#include "lv_conf_internal.h"
#include"string.h"

#define lv_strcmp(ch1,ch2) strcmp(ch1,ch2)
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

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* LVGL_SRC_H */
