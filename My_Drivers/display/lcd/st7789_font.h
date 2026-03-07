#ifndef _ST7789_FONT_H
#define _ST7789_FONT_H
#include"main.h"



//字体数据

#define FONT_WIDTH 40
#define FONT_HEIGHT 64
#define FONT_DATA_SIZE (FONT_WIDTH*FONT_HEIGHT)/8 //一个字需要的数据

extern const uint8_t wecahtData[]; 
extern const uint8_t Chinese40X56[][FONT_DATA_SIZE+3];
extern const unsigned char gImage_pic[5724];

extern const uint8_t char8X12[][12];
extern const uint8_t char16X16[][32];


uint8_t Font_GetFontId(uint8_t* str);
#endif