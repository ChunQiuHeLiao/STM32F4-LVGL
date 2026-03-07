#ifndef _ILI9341_H
#define _ILI9341_H

#include"main.h"


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
 *  2025/11/11 ：目前使用DMA+双缓冲(2048B,经测试,缓存区再高提升的效果不明显),显示图片或视频，屏幕显示速度可以达到1.8MB/s。
 *     也就是刷新320*220分辨率的屏幕，刷新一帧需要时间为320*220*2/18000000=0.078s，大概帧率为：12.82
 * 
 *  2025/11/14：发现DMA+双缓冲(每个缓存区2048*3) 可以达到17帧，但是有个问题，缓存区大于2048B帧率就开始下降。
 *  缓冲区为1028*3，帧率直接为10帧，原因未知。
 *  2025/11/14：原因已知，因为等待DMA传输完成的循环是每1ms检测一次，但其实可能在3.1ms就完成，导致浪费0.9ms
 *  实际上延迟还是要1ms，因为上了freertos一般都是1ms
 *  2025/11/20:添加ILI9341_DispChar_8X12()函数用于显示字符
 * 
*/



#define ILI9341_USE_SPI_ID 1 //使用SPI几来发送数据等一系列操作
#define ILI9341_EN_DMA 1 //使用DMA+双缓冲区


//屏幕尺寸。这里是可以为竖屏和横屏。320*240
#define ILI9341_WIDTH 320 
#define ILI9341_HEIGHT 240

// #define ILI9341_SEND_BUF_SIZE (2048) //发送数据的缓存区的大小
// #define ILI9341_SNED_NUM ((ILI9341_WIDTH*ILI9341_HEIGHT*2)/ILI9341_SEND_BUF_SIZE) //一帧画面需要发送多少次。当每次发送的数据位缓存区大小时


//字体颜色

#define     ILI9341_RED          0XF800	  //红色
#define     ILI9341_GREEN        0X07E0	  //绿色
#define     ILI9341_BLUE         0X001F	  //蓝色
#define     ILI9341_WHITE        0XFFFF	  //白色
#define     ILI9341_BLACK        0x0000 //黑色



#define ILI9341_RES_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define ILI9341_DC_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define ILI9341_BL_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()


#define ILI9341_SCL_GPIO_PORT GPIOA
#define ILI9341_SCL_PIN       GPIO_PIN_5
#define ILI9341_SDA_GPIO_PORT GPIOA
#define ILI9341_SDA_PIN      GPIO_PIN_7

#define ILI9341_RES_GPIO_PORT GPIOC
#define ILI9341_RES_PIN       GPIO_PIN_4
#define ILI9341_DC_GPIO_PORT  GPIOC
#define ILI9341_DC_PIN        GPIO_PIN_5
#define ILI9341_CS_GPIO_PORT  GPIOA
#define ILI9341_CS_PIN        GPIO_PIN_4
#define ILI9341_BL_GPIO_PORT  GPIOA
#define ILI9341_BL_PIN        GPIO_PIN_0


/* ----------软件模拟SPI时 ---------- */
// #define ILI9341_SCL_H() HAL_GPIO_WritePin(ILI9341_SCL_GPIO_PORT, ILI9341_SCL_PIN, GPIO_PIN_SET)
// #define ILI9341_SCL_L() HAL_GPIO_WritePin(ILI9341_SCL_GPIO_PORT, ILI9341_SCL_PIN, GPIO_PIN_RESET)
// #define ILI9341_SDA_H() HAL_GPIO_WritePin(ILI9341_SDA_GPIO_PORT, ILI9341_SDA_PIN, GPIO_PIN_SET)
// #define ILI9341_SDA_L() HAL_GPIO_WritePin(ILI9341_SDA_GPIO_PORT, ILI9341_SDA_PIN, GPIO_PIN_RESET)
#define ILI9341_RES_H() HAL_GPIO_WritePin(ILI9341_RES_GPIO_PORT, ILI9341_RES_PIN, GPIO_PIN_SET)
#define ILI9341_RES_L() HAL_GPIO_WritePin(ILI9341_RES_GPIO_PORT, ILI9341_RES_PIN, GPIO_PIN_RESET)
#define ILI9341_DC_H()  HAL_GPIO_WritePin(ILI9341_DC_GPIO_PORT, ILI9341_DC_PIN, GPIO_PIN_SET)
#define ILI9341_DC_L()  HAL_GPIO_WritePin(ILI9341_DC_GPIO_PORT, ILI9341_DC_PIN, GPIO_PIN_RESET)
#define ILI9341_CS_H()  HAL_GPIO_WritePin(ILI9341_CS_GPIO_PORT, ILI9341_CS_PIN, GPIO_PIN_SET)
#define ILI9341_CS_L()  HAL_GPIO_WritePin(ILI9341_CS_GPIO_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET)




void ILI9341_Init(uint8_t* buf,uint32_t size);
uint8_t ILI9341_SendData(uint8_t* data,uint16_t size);
uint8_t ILI9341_SendData_DMA(uint8_t* data,uint16_t size);
uint8_t ILI9341_WaitDMA_Over(uint8_t waitTime);

void ILI9341_SetShowDir(uint8_t dir);
void ILI9341_SetShowArea(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

void ILI9341_Clear(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ILI9341_Clear_All(uint16_t color);

void ILI9341_Color_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ILI9341_MultiColor_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t* color);
void ILI9341_MultiColor_Fill_2(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t* color);

uint16_t ILI9341_GetCurWidth();
uint16_t ILI9341_GetCurHeight();


void ILI9341_DispChar_8X12(uint8_t ch,uint16_t x,uint16_t y,uint16_t fontColor,uint16_t bgColor);
void ILI9341_DispChar_16X16(uint8_t ch,uint16_t x,uint16_t y,uint16_t fontColor,uint16_t bgColor);
// void ILI9341_DisplayChinese_40X56(unsigned int x,unsigned int y,unsigned long color,unsigned char word_serial_number);
// void ILI9341_DisplayChinses_40X56(unsigned int x,unsigned int y,uint8_t* chineses,uint8_t size);


// void ILI9341_Display_Picture(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ILI9341_DrawCross(uint16_t cx,uint16_t cy,uint8_t r,uint16_t color);
void ILI9341_DrawPoint(uint16_t x,uint16_t y,uint32_t color);

#endif