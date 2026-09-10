#include"touch_lcd.h"
#include"ili9341.h"

TP_Handle tpHandle={0};

uint8_t TP_SendData_Only(uint8_t data)
{
    return SPI_SendData(&data,1,2);
} 

uint8_t TP_SendRev(uint8_t* sendData,uint8_t* revData,uint16_t size)
{
    return SPI_TransmitRev(sendData, revData, size);
} 


//获取屏幕ADC转换的值
uint16_t TP_GetADCValue(uint8_t cmd)
{
    uint8_t  tx[3] = {cmd, 0, 0};   // 命令 + 16 个空时钟
    uint8_t  rx[3] = {0};
    
    TP_CS_L();
    uint8_t ret=TP_SendRev(tx, rx, 3);
    TP_CS_H();
    if(ret)
    {
        printf("tx rx fail:%d\n",ret);
        return ret;
    }
    
    // 0111 1111  1111 1000
     
    // if(data==0x90)
    // printf("%d,%d\n",rx[1],rx[2]);
    
    //数据总共16位。因为是全双工读取，所以发送数据时也会读取数据。 所以读取的第一位 数据是0(因为这消除忙信号) +7位有效数据+5位有效数据+3位0
    return (((uint16_t)rx[1] << 5) +(rx[2] >> 3));
}






//触摸屏，屏幕校准，让屏幕触摸更准。电阻屏才需要校准
uint8_t TP_SreenCalibration()
{
    uint16_t lx[5]={0}; //存放x逻辑值
    uint16_t ly[5]={0};

   
    tpHandle.lcd_w=ILI9341_GetCurWidth();
    tpHandle.lcd_h=ILI9341_GetCurHeight();
    tpHandle.cx=ILI9341_GetCurWidth()/2;
    tpHandle.cy=ILI9341_GetCurHeight()/2;

    uint16_t w=tpHandle.lcd_w;
    uint16_t h=tpHandle.lcd_h;

    uint16_t xy_pos[10]=
    {
        0.5*w,0.5*h,
        0.1*w,0.1*h,
        0.9*w,0.1*h,
        0.1*w,0.9*h,
        0.9*w,0.9*h
    };

    uint8_t times=0;
    while (1)
    {
        //开始画第times个校准点。中lx[0]，左上lx[1]，右上，左下，右下
        ILI9341_DrawCross(xy_pos[times*2],xy_pos[times*2+1],10,ILI9341_RED);

        //等待屏幕按下
        while (TP_PEN_Read()==1) delay_ms(1);

        delay_ms(10); //延迟10ms，以采取稳定的数据
        lx[times]=TP_GetADCValue(TP_X_CMD);
        ly[times]=TP_GetADCValue(TP_Y_CMD);
        printf("t:%d,x:%d,y:%d\n",times,lx[times],ly[times]);

        //等待屏幕松开
        while (TP_PEN_Read()==0) delay_ms(1);
        printf("(%d,%d),(%d,%d)\n",xy_pos[times*2]-10,xy_pos[times*2+1]-10,xy_pos[times*2]+10,xy_pos[times*2+1]+10);
        ILI9341_Clear(xy_pos[times*2]-10,xy_pos[times*2+1]-10,xy_pos[times*2]+10,xy_pos[times*2+1]+10,ILI9341_WHITE);
        times++;
        if(times==5) break;
    }

    tpHandle.kx=( (lx[2]-lx[1])+(lx[4]-lx[3]) )/2.0f/(float)(xy_pos[4]-xy_pos[2]); 
    tpHandle.ky=((ly[3]-ly[1])+(ly[4]-ly[2]))/2.0f/(float)(xy_pos[7]-xy_pos[5]);

    float k1=tpHandle.kx/tpHandle.ky; //x y缩放因子比
    float k2=(float)tpHandle.lcd_h/tpHandle.lcd_w;  //宽高比。固定值，不变

    // printf("kx:%.4f,ky:%.4f,k1:%.4f,k2:%.4f\n",tpHandle.kx,tpHandle.ky,k1,k2);

    if(k1>k2&&(k1-k2)>0.2f) return 1; //超过
    else if(k1<k2&&(k2-k1)>0.2f) return 1;

    //把屏幕中心点的逻辑值记录下来
    // tpHandle.lcx=lx[0];
    // tpHandle.lcy=ly[0];

    //方法二：中心点根据四个点均值得到
    tpHandle.lcx=(lx[1]+lx[2])/4.0f+(lx[3]+lx[4])/4.0f;
    tpHandle.lcy=(ly[1]+ly[3])/4.0f+(ly[2]+ly[4])/4.0f;

    /*14.036,10.353*/
    printf("kx:%.4f,ky:%.4f,lcx:%.4f,lcy:%.4f\n",tpHandle.kx,tpHandle.ky,tpHandle.lcx,tpHandle.lcy);
    return 0;
}  





//电阻触摸屏初始化
void TP_Init()
{
    SPI2_Init();

    TP_PEN_CLK_ENABLE();
    GPIO_InitTypeDef gpioInit={0};
    gpioInit.Mode=GPIO_MODE_INPUT;
    gpioInit.Pin=TP_PEN_GPIO_PIN;
    gpioInit.Pull=GPIO_PULLUP;
    gpioInit.Speed=GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(TP_PEN_GPIO_PORT,&gpioInit);

    //屏幕校准失败就一直校
    while (TP_SreenCalibration()==1)delay_ms(1);
}


/// @brief 判断触摸屏有没有按下
/// @return 1：按下，0，没有
uint8_t TP_IsPressed()
{
    if(TP_PEN_Read()==1) return 0;
    else return 1; 
}

static uint8_t isSwitchDir=0; /*切换为横屏，0：竖屏，1：横屏*/

//获取当前屏幕点击的X像素坐标。一定要先判断
uint16_t TP_GetX()
{
    //if(TP_PEN_Read()==1) return 0xFFFF; 

    if(isSwitchDir==0)
    {
        uint16_t lx=TP_GetADCValue(TP_X_CMD);
        return  (lx-tpHandle.lcx)/tpHandle.kx+tpHandle.cx;
    }
    else if(isSwitchDir==1)
    {
        uint16_t ly=TP_GetADCValue(TP_Y_CMD);
        return (ly-tpHandle.lcy)/tpHandle.ky+tpHandle.cy;
    }
    
}

//获取当前屏幕点击的Y像素坐标
uint16_t TP_GetY()
{
    //if(TP_PEN_Read()==1) return 0xFFFF; 
    if(isSwitchDir==0)
    {
        uint16_t ly=TP_GetADCValue(TP_Y_CMD);
        return (ly-tpHandle.lcy)/tpHandle.ky+tpHandle.cy;
    }
    else if(isSwitchDir==1)
    {
        uint16_t lx=TP_GetADCValue(TP_X_CMD);
        return ILI9341_HEIGHT-((lx-tpHandle.lcx)/tpHandle.kx+tpHandle.cx);
    }
}

/*设置横屏和竖屏的触摸坐标(x,y)*/
void TP_SetShowDir(uint8_t dir)
{
    isSwitchDir=dir;
}




