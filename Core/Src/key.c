#include"key.h"

//按键按下标志，0代表没按下，1代表按下
uint8_t keyPress_1=0;
uint8_t keyPress_2=0;

void KEY_Init()
{
    //有PA0，PC13
    __HAL_RCC_GPIOA_CLK_ENABLE(); //hal库的GPIOB的时钟启动
    __HAL_RCC_GPIOC_CLK_ENABLE(); //hal库的GPIOB的时钟启动
    //配置GPIO
    GPIO_InitTypeDef KEY_GPIO_Init;

    KEY_GPIO_Init.Mode=GPIO_MODE_IT_FALLING; //下降沿时触发中断
    KEY_GPIO_Init.Pin=GPIO_PIN_0;
    KEY_GPIO_Init.Pull=GPIO_PULLUP;
    KEY_GPIO_Init.Speed=GPIO_SPEED_FREQ_LOW; //GPIO电平翻转速度
    HAL_GPIO_Init(GPIOA,&KEY_GPIO_Init); 

    KEY_GPIO_Init.Pin=GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC,&KEY_GPIO_Init); 

    //使用中断来实现按键1按下识别。PA0的
    HAL_NVIC_SetPriority(EXTI0_IRQn,0,0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn); 

    //使用中断来实现 按键2按下识别。PC0的
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,1,0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); 
 
}


/// @brief 获取按键按下状况
/// @param keyVal 哪个按键，1和2 
/// @return 返回按键标志，1代表按下，0代表未按下
uint8_t KEY_GetFlag(uint8_t keyVal)
{
    if(keyVal==1) return keyPress_1;
    else if(keyVal==2) return keyPress_2;
    return 0;
}

/// @brief 清除按键按下标志
/// @param keyVal  哪个按键，1和2 
void KEY_ClearFlag(uint_least8_t keyVal)
{
    if(keyVal==1) keyPress_1=0;
    else if(keyVal==2) keyPress_2=0;
}

void EXTI0_IRQHandler()
{
    //即1。此时按键1按下了.(实际上是判断按上的)
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0)!=RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0); //清除中断标志
        keyPress_1=1;
    }
}

void EXTI15_10_IRQHandler()
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13)!=RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
        keyPress_2=1;
    }
}

uint8_t KEY_Scan(GPIO_TypeDef* GPIOx,uint8_t GPIO_Pin)
{
    //set=1,reset=0
    //高电平代表按下
    if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin)==GPIO_PIN_SET)
    {
        HAL_Delay(10); //延迟10ms
        if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin)==GPIO_PIN_RESET)
        {
            return KEY_ON;
        }   
        return KEY_OFF; //否则返回没按下
    }
    else return KEY_OFF;
}