/**
 * 起始日期：2025/11/14
 * 
 * 修改日期：2025/12/1添加适配LVGL的视频播放器的函数LCD_DispVedio_LVGL
 */


#ifndef __LCD_H
#define __LCD_H
#include"main.h"

#include"st7789.h"
#include"ili9341.h"

#include"fs_api.h"



#define LCD_MAX_LENGTH 320
#define LCD_SPI_ID ILI9341_USE_SPI_ID //使用SPI几，当使用DMA时会用到
#define LCD_EN_DMA 2 //是否启用DMA以便加快传输速度。1启用单缓冲，2启用双缓冲
#define LCD_EN_RTOS 0 //是否启用RTOS。记住，启用RTOS最好就启用DMA。避免阻塞
#define LCD_EN_LVGL 1 //是否启用LVGL



/************使用屏幕的驱动*********/

// #define LCD_EN_ST7789 
#define LCD_EN_ILI9341



/*************使用的是哪种触摸屏，没用就全注释了****************/
#define LCD_RES_TOUCH //电阻触摸



/// @brief 对齐方式
typedef enum __ALIGN_MODE
{
    ALIGN_NORMAL=0,
    ALIGN_CENTER=1,
    ALIGN_CENTER_UP_30=2 /*偏离顶部30后再居中*/
}ALIGN_MODE;

typedef struct _Vedio_Info
{
    uint32_t readSize; //当前读取文件的字节数
    uint32_t allSize; //视频 显示数据总大小
    uint32_t oneFrameDataSize; /*刷新一帧图像的需要数据大小*/

    uint16_t allFrameNum; //总帧数
    uint16_t frameIndex; //当前播放的帧索引

    uint8_t isBufHaveData:1; //缓存区是否有数据
    uint8_t bufNum:2; //如果缓存区有数据，哪个有，1还是2，如无，则0

    /*视频在屏幕的位置*/
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;

    FIL fp;
}Vedio_Info;

typedef uint8_t(*FrameFunc)(Vedio_Info* vedioInfo);


uint8_t LCD_Init();

void LCD_Clear_All(uint16_t color);
void LCD_Clear(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);


uint8_t LCD_DrawImg(uint16_t x1, uint16_t y1,const uint8_t* imgPath);
uint8_t LCD_DispImg(const uint8_t* imgPath);
uint8_t LCD_PlayVedio(const char* vedioPath,FrameFunc frameFunc);


void LCD_SetShowDir(uint8_t showDir);
void LCD_SetShowArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
uint16_t LCD_GetWidth();
uint16_t LCD_GetHeight();
uint8_t LCD_WaitDMA_Over();

uint8_t LCD_GetMediaSize(const char* filePath,uint16_t* w,uint16_t* h);
uint8_t LCD_GetFileSize(const char* filePath,uint32_t* size);

uint8_t LCD_SendData(uint8_t *data, uint16_t size);


/*播放视频和图片三部曲*/
uint8_t LCD_DispMedia_Init(uint16_t x1,uint16_t y1,char* filePath,ALIGN_MODE align,Vedio_Info* vedioInfo);
uint8_t LCD_DispMedia_OneFrame(Vedio_Info* vedioInfo);
uint8_t LCD_DispMedia(uint16_t x1, uint16_t y1,const char *filePath,ALIGN_MODE align,FrameFunc frameFunc);

#endif

