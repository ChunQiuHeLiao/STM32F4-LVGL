#ifndef __TOUCH_LCD_H
#define __TOUCH_LCD_H
#include"main.h"
#include"spi.h"





//物理坐标指触摸屏上点的实际位置，通常以液晶上点的个数来度量。
//逻辑坐标指这点被触摸时A/D 转换后的坐标值。
typedef struct __TP_Handle
{
    float kx; //x轴缩放因子。x物理值差/ x逻辑值差
    float ky; //x轴缩放因子。
    uint16_t lcx; //屏幕逻辑x坐标中心点
    uint16_t lcy; //屏幕逻辑y坐标中心点(logic center y)
    uint16_t cx; //物理x坐标中心点(w/2)
    uint16_t cy; //物理y坐标中心点(h/2)

    uint16_t lcd_w;
    uint16_t lcd_h;
}TP_Handle;


#define TP_X_CMD 0xD0
#define TP_Y_CMD 0x90


#define TP_PEN_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define TP_PEN_GPIO_PORT GPIOB
#define TP_PEN_GPIO_PIN GPIO_PIN_1


#define TP_CS_GPIO_PORT  SPIx2_CS_GPIO_PORT
#define TP_CS_GPIO_PIN  SPIx2_CS_PIN



#define TP_CS_H() HAL_GPIO_WritePin(TP_CS_GPIO_PORT,TP_CS_GPIO_PIN,1)
#define TP_CS_L() HAL_GPIO_WritePin(TP_CS_GPIO_PORT,TP_CS_GPIO_PIN,0)

#define TP_PEN_Read() HAL_GPIO_ReadPin(TP_PEN_GPIO_PORT,TP_PEN_GPIO_PIN)






void TP_Init();
// uint16_t TP_GetADCValue(uint8_t cmd);

uint8_t TP_IsPressed();
uint16_t TP_GetX();
uint16_t TP_GetY();
void TP_SetShowDir(uint8_t dir);
#endif