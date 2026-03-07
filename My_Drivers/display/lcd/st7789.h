#ifndef _ST7789_H
#define _ST7789_H

#include"main.h"
#include"st7789_font.h"


//这是闭区间[0,320-1][0,240-1]
/*************SPI配置函数*******************
SCL空闲时低电平，第一个上升沿采样
模拟SPI
******************************************/

/*******************
1.所有屏幕x y都是从0开始，如：分辨率320*240的屏幕 x=[0,319],y=[0,239]，且函数的参数 x1,x2都是左右都包含的
2.
****************************/



/***********************开发日志*************************
 *  说明：此阶段使用的是 stm32f103vet6 
 *  2025/11/11 ：目前使用DMA+双缓冲(2048B,经测试,缓存区再高提升的效果不明显),显示图片或视频，屏幕显示速度可以达到2.2MB/s。
 *     也就是刷新320*240分辨率的屏幕，刷新一帧需要时间为320*240*2/2200000=0.0698s，大概帧率为：14.32
 * 
 *  2025/11/14：发现DMA+双缓冲(每个缓存区2048*5) 可以达到17帧，但是有个问题，缓存区大于2048B帧率就开始下降。
 *  缓冲区为1028*3，帧率直接为10帧，原因未知。
 *  2025/11/14：原因已知，因为等待DMA传输完成的循环是每1ms检测一次，但其实可能在3.1ms就完成，导致浪费0.9ms
 *  实际上延迟还是要1ms，因为上了freertos一般都是1ms
 * 
*/



#define ST7789_EN_SPI_ID 1 //使用SPI几
#define ST7789_EN_LVGL 0
#define ST7789_EN_DMA 0



//字体颜色

#define     RED          0XF800	  //红色
#define     GREEN        0X07E0	  //绿色
#define     BLUE         0X001F	  //蓝色
#define     WHITE        0XFFFF	  //白色
#define BLACK 0x0000 //黑色



typedef struct _ST7789Handle
{
   uint16_t  width;
   uint16_t height;
   uint8_t  showDir; //显示方向，即水平显示，还是竖直显示，分别代表横屏和竖屏。0：横屏，1：竖屏 
   uint8_t  isColorInverse; //是否颜色反转，0：不反转，1：反转
}ST7789_Handle;



//屏幕尺寸。这里是可以为竖屏和横屏。320*240
#define ST7789_WIDTH 320 
#define ST7789_HEIGHT 240

#define ST7789_SEND_BUF_SIZE (2048) //发送数据的缓存区的大小
#define ST7789_SNED_NUM ((ST7789_WIDTH*ST7789_HEIGHT*2)/ST7789_SEND_BUF_SIZE) //一帧画面需要发送多少次。当每次发送的数据位缓存区大小时


// #define PIC_NUM 28800			//图片数据大小
// enum
// {
//    ALIGN_NORMAL=0
//    ALIGN_CENTER,
// }ALIGN_MODE;



// #define ST7789_SCL_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
// #define ST7789_SDA_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
// #define ST7789_RES_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
// #define ST7789_DC_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
// #define ST7789_CS_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
// #define ST7789_BL_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()




// #define ST7789_SCL_GPIO_PORT GPIOB
// #define ST7789_SCL_PIN       GPIO_PIN_13
// #define ST7789_SDA_GPIO_PORT GPIOB
// #define ST7789_SDA_PIN      GPIO_PIN_15
// #define ST7789_RES_GPIO_PORT GPIOD
// #define ST7789_RES_PIN       GPIO_PIN_9
// #define ST7789_DC_GPIO_PORT  GPIOD
// #define ST7789_DC_PIN        GPIO_PIN_11
// #define ST7789_CS_GPIO_PORT  GPIOD
// #define ST7789_CS_PIN        GPIO_PIN_13
// #define ST7789_BL_GPIO_PORT  GPIOD
// #define ST7789_BL_PIN        GPIO_PIN_15


#define ST7789_RES_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define ST7789_DC_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define ST7789_CS_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ST7789_BL_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()


#define ST7789_SCL_GPIO_PORT GPIOB
#define ST7789_SCL_PIN       GPIO_PIN_13
#define ST7789_SDA_GPIO_PORT GPIOB
#define ST7789_SDA_PIN      GPIO_PIN_15

#define ST7789_RES_GPIO_PORT GPIOC
#define ST7789_RES_PIN       GPIO_PIN_7
#define ST7789_DC_GPIO_PORT  GPIOC
#define ST7789_DC_PIN        GPIO_PIN_6
#define ST7789_CS_GPIO_PORT  GPIOB
#define ST7789_CS_PIN        GPIO_PIN_14
#define ST7789_BL_GPIO_PORT  GPIOB
#define ST7789_BL_PIN        GPIO_PIN_12


/* ----------软件模拟SPI时 ---------- */
// #define ST7789_SCL_H() HAL_GPIO_WritePin(ST7789_SCL_GPIO_PORT, ST7789_SCL_PIN, GPIO_PIN_SET)
// #define ST7789_SCL_L() HAL_GPIO_WritePin(ST7789_SCL_GPIO_PORT, ST7789_SCL_PIN, GPIO_PIN_RESET)
// #define ST7789_SDA_H() HAL_GPIO_WritePin(ST7789_SDA_GPIO_PORT, ST7789_SDA_PIN, GPIO_PIN_SET)
// #define ST7789_SDA_L() HAL_GPIO_WritePin(ST7789_SDA_GPIO_PORT, ST7789_SDA_PIN, GPIO_PIN_RESET)
#define ST7789_RES_H() HAL_GPIO_WritePin(ST7789_RES_GPIO_PORT, ST7789_RES_PIN, GPIO_PIN_SET)
#define ST7789_RES_L() HAL_GPIO_WritePin(ST7789_RES_GPIO_PORT, ST7789_RES_PIN, GPIO_PIN_RESET)
#define ST7789_DC_H()  HAL_GPIO_WritePin(ST7789_DC_GPIO_PORT, ST7789_DC_PIN, GPIO_PIN_SET)
#define ST7789_DC_L()  HAL_GPIO_WritePin(ST7789_DC_GPIO_PORT, ST7789_DC_PIN, GPIO_PIN_RESET)
#define ST7789_CS_H()  HAL_GPIO_WritePin(ST7789_CS_GPIO_PORT, ST7789_CS_PIN, GPIO_PIN_SET)
#define ST7789_CS_L()  HAL_GPIO_WritePin(ST7789_CS_GPIO_PORT, ST7789_CS_PIN, GPIO_PIN_RESET)



void ST7789_Init();
uint8_t ST7789_SendData(uint8_t* data,uint16_t size);
uint8_t ST7789_SendData_DMA(uint8_t* data,uint16_t size);

void ST7789_SetShowDir(uint8_t dir);
void ST7789_SetShowArea(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

void ST7789_Color_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ST7789_MultiColor_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t* color);

void ST7789_Clear(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ST7789_Clear_All(uint16_t color);

void ST7789_DisplayChinese_40X56(unsigned int x,unsigned int y,unsigned long color,unsigned char word_serial_number);
void ST7789_DisplayChinses_40X56(unsigned int x,unsigned int y,uint8_t* chineses,uint8_t size);


void ST7789_Display_Picture(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);


// uint8_t ST7789_DrawImg_From_SD(uint16_t x1,uint16_t y1,uint8_t* imgPath);
// uint8_t ST7789_DispImg_From_SD(uint8_t* imgPath);
// uint8_t ST7789_PlayVedio_From_SD(const uint8_t* vedioPath);


uint16_t ST7789_GetCurWidth();
uint16_t ST7789_GetCurHeight();
uint8_t* ST7789_GetBufPointer();

void lcd_draw_point(uint16_t x,uint16_t y,uint32_t color);
#endif