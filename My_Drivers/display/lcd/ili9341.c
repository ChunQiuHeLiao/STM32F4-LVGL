#include"ili9341.h"
#include"st7789_font.h"

#include"spi.h"



typedef struct _ILI9341Handle
{
   uint16_t width;
   uint16_t height;
   uint8_t  showDir; //显示方向，即水平显示，还是竖直显示，分别代表横屏和竖屏。0：横屏，1：竖屏 
   uint8_t  isColorInverse; //是否颜色反转，0：不反转，1：反转
   
   uint8_t* buf;
   uint32_t size; //缓冲区大小
   uint16_t clearNum; //清屏一次需要发送数据的次数。可由(ILI9341_WIDTH*ILI9341_HEIGHT*2)/size 得到
}ILI9341_Handle;
ILI9341_Handle ili9341Handle={0};


/***************只需修改下面两个函数*****************/

//只包含发送数据部分代码，不包含是发送命令还是数据和CS拉低高
uint8_t ILI9341_SendData_Only(uint8_t* data,uint16_t size)
{
    /**************这里需要写入自己的函数 SPI发送数据函数*************/
    return SPI_SendData(data,size,ILI9341_USE_SPI_ID); /******************修改点1***********/
}


#ifdef ILI9341_EN_DMA
//以DMA方式发送多个字节数据。注意：发送完本次数据需要把CS(片选)引脚拉高
//0:成功
uint8_t ILI9341_SendData_DMA(uint8_t* data,uint16_t size)
{
    if(size==0) return 1;
    ILI9341_DC_H();
    ILI9341_CS_L();

    uint8_t ret=SPI_SendData_DMA(data,size,ILI9341_USE_SPI_ID); /******************修改点2***********/

    return ret;
}


/// @brief 等待DMA搬运数据完成。函数内部自己回拉高片选
/// @param waitTime 等待时长(ms)
/// @return 0:搬运成功，2超时
uint8_t ILI9341_WaitDMA_Over(uint8_t waitTime)
{
    //等待DMA搬运数据完成
    uint16_t dma_timeout=(200*waitTime)/10; //等待
    while(SPI_DMA_GetRxFlag(ILI9341_USE_SPI_ID)==0)  /*修改 SPI_DMA_GetRxFlag为自己的那个等待DMA结束函数即可*/
    {
        delay_us(10);
        dma_timeout--;
        if(dma_timeout==0) 
        {
            ILI9341_CS_H(); //片选拉高，结束本次通信
            return 2;
        }
    }
    ILI9341_CS_H(); //片选拉高，结束本次通信
    return 0;
}

#endif

/***********************修改结束*************************/



//向液晶屏写一个8位数据
void ILI9341_SendByte(uint8_t byte)		
{
    /**************这里需要写入自己的函数 SPI发送数据函数*************/
    uint8_t ret=ILI9341_SendData_Only(&byte,1);
    if(ret) printf("block spi:%d\n",ret);
}

void ILI9341_WriteCmd(uint8_t cmd)
{
    ILI9341_DC_L();
    ILI9341_CS_L();
    ILI9341_SendByte(cmd);
    ILI9341_CS_H();

    //ILI9341_DC_H();
}


//向液晶屏写一个8位数据
void ILI9341_WriteData(uint8_t data)
{ 
    ILI9341_DC_H();
    ILI9341_CS_L();
    ILI9341_SendByte(data);
    ILI9341_CS_H();
}


//阻塞发送多个字节数据
uint8_t ILI9341_SendData(uint8_t* data,uint16_t size)
{
    if(size==0) return 0;
    ILI9341_DC_H();
    ILI9341_CS_L();

    ILI9341_SendData_Only(data,size);

    ILI9341_CS_H();
}



/// @brief ILI9342驱动初始化
/// @param buf 缓冲区。该缓冲区必须有，作为发送数据的缓冲区
/// @param size 缓冲区大小
void ILI9341_Init(uint8_t* buf,uint32_t size)
{
    ili9341Handle.buf=buf;
    ili9341Handle.size=size;
    ili9341Handle.clearNum=(ILI9341_WIDTH*ILI9341_HEIGHT*2)/size;

    #if ILI9341_USE_SPI_ID==1
    SPI_Init();
    #elif ILI9341_USE_SPI_ID==2
    SPI2_Init();
    #elif ILI9341_USE_SPI_ID==3
    SPI3_Init();
    #endif

    ILI9341_RES_CLK_ENABLE();
    ILI9341_DC_CLK_ENABLE();
    ILI9341_BL_CLK_ENABLE();

    GPIO_InitTypeDef gpio={0};
    gpio.Pin=ILI9341_RES_PIN;
    gpio.Mode=GPIO_MODE_OUTPUT_PP;
    gpio.Pull=GPIO_PULLUP;
    gpio.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ILI9341_RES_GPIO_PORT,&gpio);

    gpio.Pin=ILI9341_DC_PIN;
    HAL_GPIO_Init(ILI9341_DC_GPIO_PORT,&gpio);

    gpio.Pin=ILI9341_BL_PIN;
    HAL_GPIO_Init(ILI9341_BL_GPIO_PORT,&gpio);
    HAL_GPIO_WritePin(ILI9341_BL_GPIO_PORT,ILI9341_BL_PIN,1);

    //初始化引脚，片选默认拉高，需要复位，先低再高再低，再高
	ILI9341_RES_L();
    ILI9341_CS_H();
	delay_ms(200);
	ILI9341_RES_H();
	delay_ms(200);

    ILI9341_RES_L();
	delay_ms(10);
	ILI9341_RES_H();
	delay_ms(120);



    // SOFTWARE RESET
    ILI9341_WriteCmd(0x01);
    delay_ms(120);

    // POWER CONTROL A
    ILI9341_WriteCmd(0xCB);
    ILI9341_WriteData(0x39);
    ILI9341_WriteData(0x2C);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x34);
    ILI9341_WriteData(0x02);

    // POWER CONTROL B
    ILI9341_WriteCmd(0xCF);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x81);
    ILI9341_WriteData(0x30);

    // DRIVER TIMING CONTROL A
    ILI9341_WriteCmd(0xE8);
    ILI9341_WriteData(0x85);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x78);

    // DRIVER TIMING CONTROL B
    ILI9341_WriteCmd(0xEA);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x00);

    // POWER ON SEQUENCE CONTROL
    ILI9341_WriteCmd(0xED);
    ILI9341_WriteData(0x64);
    ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x12);
    ILI9341_WriteData(0x81);

    // PUMP RATIO CONTROL
    ILI9341_WriteCmd(0xF7);
    ILI9341_WriteData(0x20);

    // POWER CONTROL,VRH[5:0]
    ILI9341_WriteCmd(0xC0);
    ILI9341_WriteData(0x1B);  // 降低电压

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteCmd(0xC1);
    ILI9341_WriteData(0x10);

    // VCM CONTROL
    ILI9341_WriteCmd(0xC5);
    ILI9341_WriteData(0x2A);
    ILI9341_WriteData(0x2A);

    // VCM CONTROL 2
    ILI9341_WriteCmd(0xC7);
    ILI9341_WriteData(0xC0);

    // MEMORY ACCESS CONTROL
    ILI9341_WriteCmd(0x36);
    ILI9341_WriteData(0x48);

    // PIXEL FORMAT
    ILI9341_WriteCmd(0x3A);
    ILI9341_WriteData(0x55);

    // FRAME RATIO CONTROL
    ILI9341_WriteCmd(0xB1);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x18);

    // DISPLAY FUNCTION CONTROL
    ILI9341_WriteCmd(0xB6);
    ILI9341_WriteData(0x08);
    ILI9341_WriteData(0x82);
    ILI9341_WriteData(0x27);

    // 3GAMMA FUNCTION DISABLE
    ILI9341_WriteCmd(0xF2);
    ILI9341_WriteData(0x00);

    // GAMMA CURVE SELECTED
    ILI9341_WriteCmd(0x26);
    ILI9341_WriteData(0x01);

    // 使用更柔和的伽马校正
    // POSITIVE GAMMA CORRECTION
    ILI9341_WriteCmd(0xE0);
    ILI9341_WriteData(0x0F);
    ILI9341_WriteData(0x29);
    ILI9341_WriteData(0x24);
    ILI9341_WriteData(0x0C);
    ILI9341_WriteData(0x0E);
    ILI9341_WriteData(0x09);
    ILI9341_WriteData(0x4E);
    ILI9341_WriteData(0x78);
    ILI9341_WriteData(0x3C);
    ILI9341_WriteData(0x09);
    ILI9341_WriteData(0x13);
    ILI9341_WriteData(0x05);
    ILI9341_WriteData(0x17);
    ILI9341_WriteData(0x11);
    ILI9341_WriteData(0x00);

    // NEGATIVE GAMMA CORRECTION
    ILI9341_WriteCmd(0xE1);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x16);
    ILI9341_WriteData(0x1B);
    ILI9341_WriteData(0x04);
    ILI9341_WriteData(0x11);
    ILI9341_WriteData(0x07);
    ILI9341_WriteData(0x31);
    ILI9341_WriteData(0x33);
    ILI9341_WriteData(0x42);
    ILI9341_WriteData(0x05);
    ILI9341_WriteData(0x0C);
    ILI9341_WriteData(0x0A);
    ILI9341_WriteData(0x28);
    ILI9341_WriteData(0x2F);
    ILI9341_WriteData(0x0F);

    // EXIT SLEEP
    ILI9341_WriteCmd(0x11);
    delay_ms(120);

    // TURN ON DISPLAY
    ILI9341_WriteCmd(0x29);
    delay_ms(50);


    //屏幕句柄配置
    ILI9341_SetShowDir(0);
    ili9341Handle.isColorInverse=0;



    ILI9341_Clear_All(ILI9341_WHITE);

    // ILI9341_DisplayChinses_40X56(0,0,"任务码：无",5);
    // ILI9341_DisplayChinses_40X56(0,1,"任务：扫码",5);
}


//设置屏幕的显示范围，设置完后，可直接发送数据给屏幕
void ILI9341_SetShowArea(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    ILI9341_WriteCmd(0x2a);     //Column address set
    ILI9341_WriteData((uint8_t)(x1>>8));    //start column 高位
    ILI9341_WriteData((uint8_t)x1); //低位
    ILI9341_WriteData((uint8_t)(x2>>8));    //end column
    ILI9341_WriteData((uint8_t)x2);

    ILI9341_WriteCmd(0x2b);     //Row address set
    ILI9341_WriteData((uint8_t)(y1>>8));    //start row
    ILI9341_WriteData((uint8_t)y1); 
    ILI9341_WriteData((uint8_t)(y2>>8));    //end row
    ILI9341_WriteData((uint8_t)y2);

    ILI9341_WriteCmd(0x2C);     //Memory write
}


/// @brief 设置屏幕显示方向。
//先ILI9341_WriteCmd(0x36);
// 根据你的显示方向设置：
// 0x48: 正常方向
// 0x28: 旋转90度  
// 0x88: 旋转180度
// 0xE8: 旋转270度
/// @param dir 0：竖屏，1：横屏
void ILI9341_SetShowDir(uint8_t dir)
{
    if(dir>=2)
    {
        printf("dir is 0 or 1\n");
        return;
    }

    ili9341Handle.showDir=dir;
    ILI9341_WriteCmd(0x36);
    switch(dir)
    {
        case 0:     // 0度
            ili9341Handle.width=ILI9341_HEIGHT;
            ili9341Handle.height=ILI9341_WIDTH;
            ILI9341_WriteData(0x48);
            break;

        case 1:     // 90度
            ili9341Handle.width=ILI9341_WIDTH;
            ili9341Handle.height=ILI9341_HEIGHT;
            ILI9341_WriteData(0x28);
            break;
            
        case 2:     // 180度
            ili9341Handle.width=ILI9341_HEIGHT;
            ili9341Handle.height=ILI9341_WIDTH;
            ILI9341_WriteData(0x88);
            break;

        case 3:     // 270度
            ili9341Handle.width=ILI9341_WIDTH;
            ili9341Handle.height=ILI9341_HEIGHT;
            ILI9341_WriteData(0xE8);
            break;
            
        default:
            break;
    }

    //     case 0:  // 正常方向
    //     ILI9341_WriteData(0x48);  // MX=0, MY=0, MV=1, ML=0, BGR=1
    //     break;
    // case 1:  // 旋转90度
    //     ILI9341_WriteData(0x28);  // MX=0, MY=0, MV=1, ML=0, BGR=1
    //     break;
    // case 2:  // 旋转180度
    //     ILI9341_WriteData(0x88);  // MX=0, MY=1, MV=0, ML=0, BGR=1
    //     break;
    // case 3:  // 旋转270度
    //     ILI9341_WriteData(0xE8);  // MX=1, MY=1, MV=1, ML=0, BGR=1
    //     break;
}




/// @brief 多种颜色填充指定区域。区间[0,screen_width-1]
/// @param color 颜色数组
void ILI9341_MultiColor_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t* color)
{
    ILI9341_SetShowArea(x1,y1,x2,y2);

    for(uint32_t i=0;i<(x2+1-x1)*(y2+1-y1);i++)
    {
        color[i]=(color[i]<<8)+(color[i]>>8);  //这里时高低位 位置互换。因为ILI9341先收颜色RGB565高位
    }

    #if ILI9341_EN_DMA
    ILI9341_SendData_DMA((uint8_t*)color,((x2+1-x1)*(y2+1-y1))*2);

    if(ILI9341_WaitDMA_Over(20)==2) printf("dma timeout\n");
    
    #else
    ILI9341_SendData((uint8_t*)color,((x2+1-x1)*(y2+1-y1))*2);
    #endif
}


//专用于LVGL服务
void ILI9341_MultiColor_Fill_2(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t* color)
{
    ILI9341_SetShowArea(x1,y1,x2,y2);

    #if ILI9341_EN_DMA
    ILI9341_SendData_DMA((uint8_t*)color,((x2+1-x1)*(y2+1-y1))*2);
    
    #else
    ILI9341_SendData((uint8_t*)color,((x2+1-x1)*(y2+1-y1))*2);
    #endif
}

/// @brief 单一颜色填充指定区域
/// @param color 颜色 
void ILI9341_Color_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    ILI9341_SetShowArea(x1,y1,x2,y2);

    //color=(color<<8)+(color>>8);  //这里时高低位 位置互换。因为ILI9341先收颜色RGB565高位
    uint32_t allPixel=(x2+1-x1)*(y2+1-y1)*2; //该区域总像素

    //把发送缓存区都填满一种颜色数据
    for(uint32_t i=0;i<ili9341Handle.size/2;i++)
    {
        ili9341Handle.buf[i*2]=color>>8;
        ili9341Handle.buf[i*2+1]=color;
    }
    for(uint16_t i=0;i<allPixel/ili9341Handle.size;i++)
    {
        ILI9341_SendData(ili9341Handle.buf,ili9341Handle.size);
    }
    ILI9341_SendData(ili9341Handle.buf,allPixel%ili9341Handle.size);
}



//清空屏幕
//黑或白
void ILI9341_Clear_All(uint16_t color)
{
    memset(ili9341Handle.buf,color,ili9341Handle.size);
    ILI9341_SetShowArea(0,0,ili9341Handle.width-1,ili9341Handle.height-1);
    printf("w:%d,h:%d\n",ili9341Handle.width,ili9341Handle.height);
    for(uint16_t i=0;i<ili9341Handle.clearNum;i++)
    {
        ILI9341_SendData(ili9341Handle.buf,ili9341Handle.size);
    }
    //如果还有些屏幕没清除的话
    ILI9341_SendData(ili9341Handle.buf,(ILI9341_WIDTH*ILI9341_HEIGHT*2)%ili9341Handle.size);
}



/// @brief 清除指定区域的屏幕数据
/// @color 黑或白 
/// @param x1,y1,x2,y2 区域坐标，左闭右闭原则。x[0,320-1] 等于就是 0~319
void ILI9341_Clear(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    memset(ili9341Handle.buf,(uint8_t)(color),ili9341Handle.size);
    ILI9341_SetShowArea(x1,y1,x2,y2);

    uint32_t allSize=(x2+1-x1)*(y2+1-y1)*2;
    for(uint16_t i=0;i<(allSize/ili9341Handle.size);i++)
    {
        ILI9341_SendData(ili9341Handle.buf,ili9341Handle.size);
    }
    ILI9341_SendData(ili9341Handle.buf,(allSize%ili9341Handle.size));
}









/// @brief 显示一个字。切记  ili9341Handle.size 必须是2的倍数
/// @param fontData 字体像素数据
/// @param fontW 
/// @param fontH 
/// @param fontColor 字体颜色
/// @param bgColor 字体背景颜色(即不显示字体部分的颜色)
void ILI9341_DispFont(uint8_t* fontData,uint8_t fontW,uint8_t fontH,uint16_t fontColor,uint16_t bgColor)
{
    uint16_t size=fontW*fontH; //字体总像素值 

    uint8_t tmp=0;
    uint32_t recordSendNum=0; //记录发送的数据个数  
    for(uint16_t i=0;i<(size/8);i++) 
    {
        tmp=fontData[i];
        for(uint8_t j=0;j<8;j++)
        {
            if(tmp&0x01)
            {
                // ili9341Handle.buf[i*16+j*2]=fontColor>>8;
                // ili9341Handle.buf[i*16+j*2+1]=fontColor;
                ili9341Handle.buf[recordSendNum++]=fontColor>>8;
                ili9341Handle.buf[recordSendNum++]=fontColor;
            }
            else
            {
                // ili9341Handle.buf[i*16+j*2]=bgColor>>8;
                // ili9341Handle.buf[i*16+j*2+1]=bgColor;
                ili9341Handle.buf[recordSendNum++]=bgColor>>8;
                ili9341Handle.buf[recordSendNum++]=bgColor;
            }
            tmp>>=1;

            if(recordSendNum==ili9341Handle.size) 
            {
                recordSendNum=0;
                ILI9341_SendData(ili9341Handle.buf,ili9341Handle.size);
            }
        }      
    }

    ILI9341_SendData(ili9341Handle.buf,recordSendNum);
}



void ILI9341_DispChar_8X12(uint8_t ch,uint16_t x,uint16_t y,uint16_t fontColor,uint16_t bgColor)
{
    ILI9341_SetShowArea(x,y,x+8-1,y+12-1);
    if(ch=='.') ILI9341_DispFont(char8X12[26],8,12,fontColor,bgColor);
    else if(ch==':') ILI9341_DispFont(char8X12[27],8,12,fontColor,bgColor);
    else if(ch==',') ILI9341_DispFont(char8X12[28],8,12,fontColor,bgColor);
    else ILI9341_DispFont(char8X12[ch-97],8,12,fontColor,bgColor);
}


void ILI9341_DispChar_16X16(uint8_t ch,uint16_t x,uint16_t y,uint16_t fontColor,uint16_t bgColor)
{
    ILI9341_SetShowArea(x,y,x+16-1,y+16-1);

    ILI9341_DispFont(char16X16[ch-97],16,16,fontColor,bgColor);
}



//画点函数
void ILI9341_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
{
    ILI9341_SetShowArea(x,y,x,y);
    uint8_t data[2]={color>>8,color};
    ILI9341_SendData(data,2);
}



/// @brief 画十字 
/// @param cx 中心坐标
/// @param cy 
/// @param r 十字的半径。建议不要超过20，这里是没优化的,太大会内存会满。即以中心开始的上下左右的四条边(不包含中心点像素)
void ILI9341_DrawCross(uint16_t cx,uint16_t cy,uint8_t r,uint16_t color)
{
    //x就是十字中心的上中 坐标，y是左中坐标
    for(uint16_t x=cx-r;x<cx+r;x++) ILI9341_DrawPoint(x,cy,color);
    for(uint16_t y=cy-r;y<cy+r;y++) ILI9341_DrawPoint(cx,y,color);
}


uint16_t ILI9341_GetCurWidth()
{
    return ili9341Handle.width;
}
uint16_t ILI9341_GetCurHeight()
{
    return ili9341Handle.height;
}