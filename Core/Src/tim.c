#include"tim.h"
// #include"lvgl.h"

TIM_HandleTypeDef timBaseHandle;
TIM_HandleTypeDef timPwmHandle;
TIM_HandleTypeDef timICHandle;
TIM_HandleTypeDef timPulseHandle;

uint16_t reloadValue=0; //ARR的值
uint16_t clkPrescaler=0;


#if TIM_BASE_MODE==1

/// @brief 时基初始化。即定时器的基本功能的初始化，即定时(增加，减少，中心对齐的计时)
/// @param clkPsc 对输入时钟(maxf=72MHz)进行分频。这个是你配置的定时器的输入时钟
/// @param reload 重装载值
/// @return 返回1代表异常，0代表正常
uint8_t TIM_Base_Init(uint16_t clkPsc,uint16_t reloadVal)
{
    timBaseHandle.Instance=TIMx_BASE;
    timBaseHandle.Init.Prescaler=clkPsc;
    timBaseHandle.Init.CounterMode=TIM_COUNTERMODE_UP;
    timBaseHandle.Init.Period=reloadVal;
    
    HAL_TIM_Base_Init(&timBaseHandle); //会进入HAL_TIM_Base_MspInit函数

    //以中断方式启动基本定时器
    HAL_TIM_Base_Start_IT(&timBaseHandle);
    
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_handle)
{
    #if TIM_BASE_MODE==1
    if(tim_handle->Instance==TIMx_BASE)
    {
        //启动时钟
        TIMx_BASE_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIMx_BASE_IRQn,1,3); //设置中断优先级
        HAL_NVIC_EnableIRQ(TIMx_BASE_IRQn); //使能中断请求 interrupt request
    }
    #endif
}


//基本定时器中断服务函数 TIM6 TIM7
void TIM4_IRQHandler()
{
    //跳转回调函数，并自动清除中断标志
    HAL_TIM_IRQHandler(&timBaseHandle);
}

#endif



#if TIM_PWM_MODE==1

/// @brief PWM初始化。
/// @param clkPsc 对输入时钟(maxf=72MHz)进行分频。这个是你配置的定时器的输入时钟。
///当clkPsc=72-1时，计数器时钟为1MHz(输入时钟为72MHz时),计数一次耗时1us。当clkPsc=72*2-1时，为2us,
///以此类推72*n-1 则计数一次耗时n us
/// @param pulseVal 比较值。注意，比较值为1，实际是+2。分频系数那也一样
/// @param reloadVal 重装载值。注意，重装载值为1，实际是+2。分频系数那也一样
/// @param repVal (高级定时器才需要配置,其它可填0) 重复计数器寄存器的值。0~255。注意，重复计数器寄存器值为1，实际是2。即输入n，实际n+1
/// @return 返回1代表异常，0代表正常
uint8_t TIM_PWM_Init(uint16_t clkPsc,uint16_t pulseVal,uint16_t reloadVal,uint8_t repVal)
{
    timPwmHandle.Instance=TIMx_PWM;
    timPwmHandle.Init.Prescaler=clkPsc;
    timPwmHandle.Init.Period=reloadVal;
    timPwmHandle.Init.CounterMode=TIM_COUNTERMODE_UP; //定时器递增模式
    
    //使用了TIM1/8高级寄存器
    #if TIM_ADVANCE_ENABLE_MODE==1
    timPwmHandle.Init.RepetitionCounter=repVal; //重复寄存器的值
    #endif

    HAL_TIM_PWM_Init(&timPwmHandle);

    TIM_OC_InitTypeDef timOC={0}; //输出比较
    timOC.OCMode=TIM_OCMODE_PWM1; //选用PWM1
    timOC.Pulse=pulseVal; //这里设置比较值为重装载值的一般，占空比 Pulse/ARR
    timOC.OCPolarity=TIM_OCPOLARITY_HIGH;   //决定 PWM 信号的有效电平
    HAL_TIM_PWM_ConfigChannel(&timPwmHandle,&timOC,TIM_PWM_CHANNELx);

    __HAL_TIM_ENABLE_IT(&timPwmHandle,TIM_IT_UPDATE); //使能定时器中断
    HAL_TIM_PWM_Start(&timPwmHandle,TIM_PWM_CHANNELx);

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_handle)
{
     GPIO_InitTypeDef gpioInit={0};
    if(tim_handle->Instance==TIMx_PWM)
    {
        TIMx_PWM_CLK_ENABLE();
        TIM_PWM_GPIO_CLK_ENABLE();
        gpioInit.Pin=TIM_PWM_GPIO_PIN;
        gpioInit.Mode=GPIO_MODE_AF_PP;
        gpioInit.Pull=GPIO_PULLUP;
        gpioInit.Speed=GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(TIM_PWM_GPIO_PORT,&gpioInit);

        
        HAL_NVIC_SetPriority(TIMx_PWM_IRQn,1,3);
        HAL_NVIC_EnableIRQ(TIMx_PWM_IRQn);
       
        //__HAL_RCC_AFIO_CLK_ENABLE(); //使能 AFIO 时钟（重映射必须!)
       // __HAL_AFIO_REMAP_TIM3_PARTIAL(); //TIM3部分重映射
       // __HAL_AFIO_REMAP_TIM3_ENABLE(); //TIM3完全重映射使能
    }
}

//设置PWM的占空比
//@param dutyVal 0~1 占空比值
void TIM_PWM_SetDutyCycle(float dutyVal)
{   
    //得到比较值
    uint16_t compareVal=dutyVal*(__HAL_TIM_GET_AUTORELOAD(&timPwmHandle)+1); 
    __HAL_TIM_SET_COMPARE(&timPwmHandle,TIM_PWM_CHANNELx,compareVal);
    HAL_TIM_GenerateEvent(&timPwmHandle,TIM_EventSource_Update); //软件模拟产生更新事件
    HAL_TIM_PWM_Start(&timPwmHandle,TIM_PWM_CHANNELx); //启动PWM
}


#if TIM_ADVANCE_ENABLE_MODE==1

//设置定时器重复计数器的值。注意实际会记val+1个数
void TIM_PWM_SetRepeatitionCounter(uint8_t val)
{
    TIM8->RCR=val;
    HAL_TIM_GenerateEvent(&timPwmHandle,TIM_EventSource_Update); //软件模拟产生更新事件
    HAL_TIM_PWM_Start(&timPwmHandle,TIM_PWM_CHANNELx); //启动PWM
}

//产生固定个PWM后，会停止产生PWM，可以使用该函数再次产生PWM
void TIM_PWM_Restart()
{
    HAL_TIM_PWM_Start(&timPwmHandle,TIM_PWM_CHANNELx); //启动PWM
}


void TIM8_UP_IRQHandler()
{
    HAL_TIM_IRQHandler(&timPwmHandle);
}

#endif

#endif



#if TIM_IC_MODE==1 || TIM_PULSE_MODE==1
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* htim)
{
    GPIO_InitTypeDef gpioInit={0};
    
    #if TIM_IC_MODE==1 

    if(htim->Instance==TIMx_IC)
    {
        TIMx_IC_CLK_ENABLE();
        TIM_IC_GPIO_CLK_ENABLE();
        gpioInit.Pin=TIM_IC_GPIO_PIN;
        gpioInit.Mode=GPIO_MODE_AF_INPUT;
        gpioInit.Pull=GPIO_PULLDOWN; //这里连的是按键，按键是按下时高电平，那你肯定要下拉呀
        gpioInit.Speed=GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(TIM_IC_GPIO_PORT,&gpioInit);

        //使能中断
        HAL_NVIC_SetPriority(TIMx_IC_IRQn,1,2);
        HAL_NVIC_EnableIRQ(TIMx_IC_IRQn);
    }
    #endif

    #if TIM_PULSE_MODE==1
    
    if(htim->Instance==TIMx_PULSE)
    {
        TIMx_PULSE_CLK_ENABLE();
        TIM_PULSE_GPIO_CLK_ENABLE();
        gpioInit.Pin=TIM_PULSE_GPIO_PIN;
        gpioInit.Mode=GPIO_MODE_AF_INPUT;
        gpioInit.Pull=GPIO_PULLDOWN; //这里连的是按键，按键是按下时高电平，那你肯定要下拉呀
        gpioInit.Speed=GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(TIM_PULSE_GPIO_PORT,&gpioInit);

        // __HAL_RCC_AFIO_CLK_ENABLE();
        // __HAL_AFIO_REMAP_TIM2_PARTIAL_1();
        //__HAL_RCC_AFIO_CLK_ENABLE(); //使能 AFIO 时钟（重映射必须!)
       // __HAL_AFIO_REMAP_TIM3_PARTIAL(); //TIM3部分重映射
        // //使能中断
        // HAL_NVIC_SetPriority(TIMx_PULSE_IRQn,1,2);
        // HAL_NVIC_EnableIRQ(TIMx_PULSE_IRQn);
    }
    #endif
}

#endif

#if TIM_IC_MODE==1

/// @brief IC初始化。
/// @param clkPsc 对输入时钟(maxf=72MHz)进行分频。这个是你配置的定时器的输入时钟。
///当clkPsc=72-1时，计数器时钟为1MHz(输入时钟为72MHz时),计数一次耗时1us。当clkPsc=72*2-1时，为2us,
///以此类推72*n-1 则计数一次耗时n us
/// @param reloadVal 重装载值。注意，重装载值为1，实际是+2。分频系数那也一样
/// @return 返回1代表异常，0代表正常
uint8_t TIM_IC_Init(uint16_t clkPsc,uint16_t reloadVal)
{
    reloadValue=reloadVal;
    clkPrescaler=clkPsc;
    //基本定时功能的配置。计数模式递增
    timICHandle.Instance=TIMx_IC;
    timICHandle.Init.Prescaler=clkPsc;
    timICHandle.Init.CounterMode=TIM_COUNTERMODE_UP;
    timICHandle.Init.Period=reloadVal; 
    if(HAL_TIM_IC_Init(&timICHandle)!=HAL_OK)
    {
        return 1;
    }

    
    //
    TIM_IC_InitTypeDef timICInit={0}; //局部变量要赋初值，不然会是随机值
    timICInit.ICPolarity=TIM_IC_ICPOLARITY; 
    timICInit.ICSelection=TIM_ICSELECTION_DIRECTTI; //定时器输入通道1(还有2),不是输入捕获通道1哦
    timICInit.ICPrescaler=TIM_ICPSC_DIV1; //不分频，即只要产生一个边沿就触发IC中断
    timICInit.ICFilter=0; //不过滤
    HAL_TIM_IC_ConfigChannel(&timICHandle,&timICInit,TIM_IC_CHANNELx);

    //使能定时器溢出中断
    __HAL_TIM_ENABLE_IT(&timICHandle,TIM_IT_UPDATE);
    //该函数是专门用于启动输入捕获的，只会响应输入捕获的中断，不会响应定时器溢出中断
    HAL_TIM_IC_Start_IT(&timICHandle,TIM_IC_CHANNELx); //中断模式启动输入捕获
    
    return 0;
}



//边沿标志。默认是0，输入捕获检测到一次上升沿则置1，下降沿则置2
uint8_t timIC_edgeFlag=0; 
uint8_t timIC_captureFlage=0; //0还未捕获完成。1代表捕获成功一次上升与下降沿，2代表超出捕获的最大时间(异常)。

uint8_t timIC_overflowNum=0; //定时器溢出次数
uint16_t timIC_timStopVal=0; //即捕获到下降沿后，此刻的比较值(CCR)



//获取捕获标志
//@return 0代表还没有捕获完成。1代表成功捕获到了一次上升沿和下降沿。2代表捕获超时，
//迟迟未等到下降沿
uint8_t TIM_IC_GetCaptureFlag()
{
    return timIC_captureFlage;
}

//清除捕获标志,和其它
void TIM_IC_ClearCaptureFlage()
{
    timIC_captureFlage=0;
    timIC_edgeFlag=0;
    timIC_overflowNum=0;
    timIC_timStopVal=0;
    
    HAL_TIM_IC_Start(&timICHandle,TIM_IC_CHANNELx);  //启动输入捕获定时器
}

//获取捕获需要的时间(us)
uint32_t TIM_IC_GetCaptureTime()
{
    uint32_t tmp=0;
    tmp=(timIC_overflowNum*(reloadValue+1)+timIC_timStopVal);

    return (uint32_t)((clkPrescaler+1)/72.0f*tmp); //72.0f是72MHz的定时器时钟频率
}


void TIM5_IRQHandler()
{
    //定时器共用处理函数，会自动清除中断标志
    HAL_TIM_IRQHandler(&timICHandle); 
}


//定时器输入捕获回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIMx_IC)
    {
        //这次产生上升沿，那上一次就是下降沿喽
        if(timIC_edgeFlag==0)
        {
            timIC_edgeFlag=1; //上升沿

            //直接设置计数器的值就行，不需要关闭定时器，关闭的那段时间处于高电平，会
            //导致这段时间不会被记上，所以会结果会偏小
            //HAL_TIM_IC_Stop(&timICHandle,TIM_IC_CHANNELx); //关闭定时器
            __HAL_TIM_SET_COUNTER(&timICHandle,0); //设置定时器的计数值为0

            //设置定时器运行时的捕获极性，本质是先清除原来的极性设置，然后开始设好
            __HAL_TIM_SET_CAPTUREPOLARITY(&timICHandle,TIM_IC_CHANNELx,TIM_ICPOLARITY_FALLING);
            //HAL_TIM_IC_Start(&timICHandle,TIM_IC_CHANNELx);  //使能定时器
        }   
        else if(timIC_edgeFlag==1) //捕获到下降沿
        {
            timIC_edgeFlag=2;
            timIC_captureFlage=1; //成功捕获到一次上升与下降沿
            HAL_TIM_IC_Stop(&timICHandle,TIM_IC_CHANNELx); ; //关闭定时器。可以不用设置定时器的计数值为0
            //读取产生下降沿时刻，捕获比较寄存器的值(CCR)
            timIC_timStopVal=HAL_TIM_ReadCapturedValue(&timICHandle,TIM_IC_CHANNELx);
            //__HAL_TIM_GET_COUNTER(&timICHandle);
            
            //设置定时器运行时的捕获极性，本质是先清除原来的极性设置，然后开始设好
            __HAL_TIM_SET_CAPTUREPOLARITY(&timICHandle,TIM_IC_CHANNELx,TIM_ICPOLARITY_RISING);
        }
    }
}

#endif


#if TIM_PULSE_MODE==1

/// @brief 脉冲计数初始化。最多计数65536个数
/// @param clkPsc 对输入时钟(maxf=72MHz)进行分频。这个是你配置的定时器的输入时钟。
///如：72MHz时钟需要脉冲2次才能得到一次定时器时钟。
///当clkPsc=72-1时，计数器时钟为1MHz(输入时钟为72MHz时),计数一次耗时1us。当clkPsc=72*2-1时，为2us,
///以此类推72*n-1 则计数一次耗时n us
/// @return 返回1代表异常，0代表正常
uint8_t TIM_Pulse_Init(uint16_t clkPsc)
{
    //基本定时功能的配置。计数模式递增
    timPulseHandle.Instance=TIMx_PULSE;
    timPulseHandle.Init.Prescaler=clkPsc;
    timPulseHandle.Init.CounterMode=TIM_COUNTERMODE_UP;
    timPulseHandle.Init.Period=65535; 
    if(HAL_TIM_IC_Init(&timPulseHandle)!=HAL_OK)
    {
        return 1;
    }
    //配置定时器从模式
    TIM_SlaveConfigTypeDef timSlaveInit={0};  //局部变量要赋初值，不然会是随机值
    timSlaveInit.SlaveMode=TIM_SLAVEMODE_EXTERNAL1;
    timSlaveInit.InputTrigger=TIM_TS_ETRF; //
    timSlaveInit.TriggerPolarity= TIM_TRIGGERPOLARITY_FALLING; //下升沿触发
    timSlaveInit.TriggerPrescaler=0; //不分频，即分频1
    HAL_TIM_SlaveConfigSynchro(&timPulseHandle,&timSlaveInit);

    HAL_TIM_IC_Start(&timPulseHandle,TIM_PULSE_CHANNELx); //中断模式启动输入捕获
    
    return 0;
}



//边沿标志。默认是0，输入捕获检测到一次上升沿则置1，下降沿则置2
uint8_t timPULSE_edgeFlag=0; 
uint8_t timPULSE_captureFlage=0; //0还未捕获完成。1代表捕获成功一次上升与下降沿，2代表超出捕获的最大时间(异常)。

uint8_t timPULSE_overflowNum=0; //定时器溢出次数
uint16_t timPULSE_timStopVal=0; //即捕获到下降沿后，此刻的比较值(CCR)



//获取捕获标志
//@return 0代表还没有捕获完成。1代表成功捕获到了一次上升沿和下降沿。2代表捕获超时，
//迟迟未等到下降沿
uint8_t TIM_PULSE_GetCaptureFlag()
{
    return timPULSE_captureFlage;
}

//清除捕获标志,和其它
void TIM_PULSE_ClearCaptureFlage()
{
    timPULSE_captureFlage=0;
    timPULSE_edgeFlag=0;
    timPULSE_overflowNum=0;
    timPULSE_timStopVal=0;
    
    __HAL_TIM_ENABLE(&timPulseHandle); //启动输入捕获定时器
}

//获取当前的脉冲计数值
uint16_t TIM_PULSE_GetCount()
{
    return __HAL_TIM_GET_COUNTER(&timPulseHandle);
}

//清除当前的脉冲计数值
uint16_t TIM_PULSE_ClearCount()
{
    return __HAL_TIM_SET_COUNTER(&timPulseHandle,0);
}



// void TIM5_IRQHandler()
// {
//     //定时器共用处理函数，会自动清除中断标志
//     HAL_TIM_IRQHandler(&timPulseHandle); 
// }


//定时器输入捕获回调函数
void HAL_TIM_PULSE_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIMx_PULSE)
    {
        //这次产生上升沿，那上一次就是下降沿喽
        if(timPULSE_edgeFlag==0)
        {
            timPULSE_edgeFlag=1; //上升沿
            __HAL_TIM_DISABLE(&timPulseHandle); //关闭定时器
            __HAL_TIM_SET_COUNTER(&timPulseHandle,0); //设置定时器的计数值为0

            //设置定时器运行时的捕获极性，本质是先清除原来的极性设置，然后开始设好
            __HAL_TIM_SET_CAPTUREPOLARITY(&timPulseHandle,TIM_PULSE_CHANNELx,TIM_ICPOLARITY_FALLING);
            __HAL_TIM_ENABLE(&timPulseHandle); //使能定时器
        }   
        else if(timPULSE_edgeFlag==1) //捕获到下降沿
        {
            timPULSE_edgeFlag=2;
            timPULSE_captureFlage=1; //成功捕获到一次上升与下降沿
            __HAL_TIM_DISABLE(&timPulseHandle); //关闭定时器。可以不用设置定时器的计数值为0
            //读取产生下降沿时刻，捕获比较寄存器的值(CCR)
            timPULSE_timStopVal=HAL_TIM_ReadCapturedValue(&timPulseHandle,TIM_PULSE_CHANNELx);
            //__HAL_TIM_GET_COUNTER(&timPulseHandle);
            
            //设置定时器运行时的捕获极性，本质是先清除原来的极性设置，然后开始设好
            __HAL_TIM_SET_CAPTUREPOLARITY(&timPulseHandle,TIM_PULSE_CHANNELx,TIM_ICPOLARITY_RISING);
        }
    }
}

#endif



/************************改*******/




#include"lvgl.h"
//定时器计数溢出回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    #if TIM_BASE_MODE==1
    if(htim->Instance==TIMx_BASE)
    {
       lv_tick_inc(1);
       //HAL_IncTick();
    }
    #endif
    
    #if TIM_IC_MODE==1
    if(htim->Instance==TIMx_IC)
    {
        if(timIC_edgeFlag==1) //捕获到了上升沿才开始计时
        {
            if(timIC_overflowNum>=255) //溢出了，高电平持续时间太长。捕获失败
            {
                timIC_captureFlage=2; //超出捕获的最大时间。
                timIC_edgeFlag=2; //当做是捕获到了下降沿

                __HAL_TIM_DISABLE(&timICHandle); //关闭定时器
                //重新开始捕获上升沿。
                __HAL_TIM_SET_CAPTUREPOLARITY(&timICHandle,TIM_IC_CHANNELx,TIM_ICPOLARITY_RISING);
                //获取ARR寄存器的值，因为这次又溢出了，所以也就是timIC_overflowNum再+1，但是timIC_overflowNum是u8类型
                timIC_timStopVal=__HAL_TIM_GET_AUTORELOAD(&timICHandle)+1; 
            }
            else 
            {
                timIC_overflowNum++;
            }
        }
        
    }

    #endif

    #if TIM_ADVANCE_ENABLE_MODE==1
    if(htim->Instance==TIM8)
    {
        UART_Transmit_AllString("enter over\r\n");
        //__HAL_TIM_DISABLE(&timPwmHandle);
        HAL_TIM_PWM_Stop(&timPwmHandle,TIM_PWM_CHANNELx);
    }
    #endif
}
