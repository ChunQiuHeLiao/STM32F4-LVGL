/*
* 该文件包含一些 通用的UI和逻辑代码结合得到API
*
*
*
* 2026.3.1 
*/

#ifndef __UTIL_MID_H
#define __UTIL_MID_H

#include"main.h"
#include"lvgl.h"


uint8_t UtilMid_RefleshFileTable(lv_obj_t* table, const char* fileDir, uint16_t* fileNum);

void UtilMid_RefleshList(lv_obj_t* table, const char* symbol, const char* str, uint16_t row);

#endif
