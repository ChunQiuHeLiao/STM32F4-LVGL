/**
 * 使用指南：
 * 1. F1系列基本定时器:        通用定时器：  高级定时器：
 *              
 */
#ifndef _TIM_H
#include"main.h"
//#include"stm32f1xx_hal_tim.h"


#define TIM_EN_LVGL  //为LVGL提供时基,定时器7提供时基



/***************************需要修改的参数*******************************/
//注意：如果定时器改变，中断服务函数也要改，莫忘-------------?

#define TIM_BASE_MODE 1 //定时器 时基模式
#define TIM_PWM_MODE 0 //定时器PWM模式(输出比较)(用于输出PWM波形,周期为(ARR+1)) 
#define TIM_IC_MODE 0 //定时器输入捕获模式
#define TIM_OC_MODE 0 //输出比较模式(1.有输出PWM模式，可翻转。周期为2*(ARR+1)，可认为OC包含PWM )
#define TIM_PULSE_MODE 0 //脉冲计数模式

//高级定时器开关。使用时必须启用(1==启用，0关闭)
#define TIM_ADVANCE_ENABLE_MODE 0 //TIM1 和TIM8 f1系列




/*-------------------------时基部分---------------------*/

#if TIM_BASE_MODE==1
#define TIMx_BASE TIM4
#define TIMx_BASE_CLK_ENABLE()  __HAL_RCC_TIM4_CLK_ENABLE()
#define TIMx_BASE_IRQn TIM4_IRQn
#endif



/*------------------------------PWM部分-----------------------*/

/*********输出PWM的那个引脚********/
#if TIM_PWM_MODE==1

#define TIMx_PWM TIM2
#define TIMx_PWM_CLK_ENABLE()  __HAL_RCC_TIM2_CLK_ENABLE()
#define TIMx_PWM_CLK_DISABLE() __HAL_RCC_TIM2_CLK_DISABLE()
#define TIM_PWM_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE();
#define TIM_PWM_GPIO_PORT GPIOA
#define TIM_PWM_GPIO_PIN GPIO_PIN_0

#define TIM_PWM_CHANNELx TIM_CHANNEL_1 //定时器的通道，1~4

#define TIMx_PWM_IRQn TIM2_IRQn //TIM8定时器重复计数器更新中断，TIM1也一样

#if TIM_ADVANCE_ENABLE_MODE==1

#endif

#endif




/*------------------------------输出比较部分-----------------------*/

/*********输出PWM的那个引脚********/
#if TIM_OC_MODE==1

#define TIMx_OC TIM8
#define TIMx_OC_CLK_ENABLE()  __HAL_RCC_TIM8_CLK_ENABLE()
#define TIMx_OC_CLK_DISABLE() __HAL_RCC_TIM8_CLK_DISABLE()
#define TIM_OC_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE();
#define TIM_OC_GPIO_PORT GPIOC
#define TIM_OC_GPIO_PIN GPIO_PIN_6

#define TIM_OC_CHANNELx TIM_CHANNEL_1 //定时器的通道，1~4

#define TIMx_OC_IRQn TIM8_UP_IRQn //TIM8定时器重复计数器更新中断，TIM1也一样

#if TIM_ADVANCE_ENABLE_MODE==1

#endif

#endif






/*--------------------输入捕获部分------------------*/

#if TIM_IC_MODE==1
#define TIMx_IC TIM2
#define TIMx_IC_CLK_ENABLE()  __HAL_RCC_TIM2_CLK_ENABLE()
#define TIM_IC_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE();
#define TIM_IC_GPIO_PORT GPIOA
#define TIM_IC_GPIO_PIN GPIO_PIN_1 
#define TIMx_IC_IRQn TIM2_IRQn

#define TIM_IC_ICPOLARITY TIM_ICPOLARITY_RISING //检测的边沿
#define TIM_IC_CHANNELx TIM_CHANNEL_2 //定时器的通道，1~4
#endif

#if TIM_PULSE_MODE==1
#define TIMx_PULSE TIM3
#define TIMx_PULSE_CLK_ENABLE()  __HAL_RCC_TIM3_CLK_ENABLE()
#define TIM_PULSE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE();
#define TIM_PULSE_GPIO_PORT GPIOD
#define TIM_PULSE_GPIO_PIN GPIO_PIN_2
#define TIMx_PULSE_IRQn TIM2_IRQn

#define TIM_PULSE_CHANNELx TIM_CHANNEL_1 //定时器的通道，1~4
#endif



/**************************结束参数修改*******************************/





#if TIM_BASE_MODE==1
uint8_t TIM_Base_Init(uint16_t clkPsc,uint16_t reload);
#endif


#if TIM_PWM_MODE==1
uint8_t TIM_PWM_Init(uint16_t clkPsc,uint16_t pulseVal,uint16_t reloadVal,uint8_t repVal);
void TIM_PWM_SetDutyCycle(float dutyVal);

#if TIM_ADVANCE_ENABLE_MODE==1
void TIM_PWM_SetRepeatitionCounter(uint8_t val);
void TIM_PWM_Restart();
#endif
#endif


#if TIM_IC_MODE==1
uint8_t TIM_IC_Init(uint16_t clkPsc,uint16_t reloadVal);
uint8_t TIM_IC_GetCaptureFlag(); 
void TIM_IC_ClearCaptureFlage();
uint32_t TIM_IC_GetCaptureTime();
#endif


#if TIM_PULSE_MODE==1
uint8_t TIM_Pulse_Init(uint16_t clkPsc);
uint16_t TIM_PULSE_GetCount();
uint16_t TIM_PULSE_ClearCount();
#endif



#ifdef TIM_EN_LVGL

#endif


#endif