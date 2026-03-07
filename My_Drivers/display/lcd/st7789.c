#include"st7789.h"
#include"spi.h"
#include"fs_api.h"

//发送缓冲区
uint8_t st7789_sendBuf[ST7789_SEND_BUF_SIZE]={0};

ST7789_Handle st7789Handle={0};


/***************只需修改下面两个函数 */



//只包含发送数据部分代码，不包含是发送命令还是数据和CS拉低高
uint8_t ST7789_SendData_Only(uint8_t* data,uint16_t size)
{
    /**************这里需要写入自己的函数 SPI发送数据函数*************/
    return SPI_SendData(data,size,ST7789_EN_SPI_ID); /******************修改点1***********/
}

//以DMA方式发送多个字节数据。注意：发送完本次数据需要把CS(片选)引脚拉高
//0:成功
uint8_t ST7789_SendData_DMA(uint8_t* data,uint16_t size)
{
    if(size==0) return 1;
    ST7789_DC_H();
    ST7789_CS_L();

    uint8_t ret=SPI_SendData_DMA(data,size,ST7789_EN_SPI_ID); /******************修改点2***********/


    return ret;
}





//向液晶屏写一个8位数据
void ST7789_SendByte(uint8_t byte)		
{
    /**************这里需要写入自己的函数 SPI发送数据函数*************/

    ST7789_SendData_Only(&byte,1);
}

void ST7789_WriteCmd(uint8_t cmd)
{
    ST7789_DC_L();
    ST7789_CS_L();
    ST7789_SendByte(cmd);
    ST7789_CS_H();

    //ST7789_DC_H();
}
  //向液晶屏写一个8位数据
void ST7789_WriteData(uint8_t data)
{ 
    ST7789_DC_H();
    ST7789_CS_L();
    ST7789_SendByte(data);
    ST7789_CS_H();
}


//阻塞发送多个字节数据
uint8_t ST7789_SendData(uint8_t* data,uint16_t size)
{
    if(size==0) return 0;
    ST7789_DC_H();
    ST7789_CS_L();

    ST7789_SendData_Only(data,size);

    ST7789_CS_H();
}




//初始化
void ST7789_Init()
{
    SPI_Init();
    ST7789_RES_CLK_ENABLE();
    ST7789_DC_CLK_ENABLE();
    ST7789_BL_CLK_ENABLE();

    GPIO_InitTypeDef gpio={0};
    gpio.Pin=ST7789_RES_PIN;
    gpio.Mode=GPIO_MODE_OUTPUT_PP;
    gpio.Pull=GPIO_PULLUP;
    gpio.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ST7789_RES_GPIO_PORT,&gpio);

    gpio.Pin=ST7789_DC_PIN;
    HAL_GPIO_Init(ST7789_DC_GPIO_PORT,&gpio);

    gpio.Pin=ST7789_BL_PIN;
    HAL_GPIO_Init(ST7789_BL_GPIO_PORT,&gpio);
    HAL_GPIO_WritePin(ST7789_BL_GPIO_PORT,ST7789_BL_PIN,1);

    //初始化引脚
	ST7789_RES_L();
    ST7789_CS_H();
	delay_ms(200);
	ST7789_RES_H();
	delay_ms(200);

    ST7789_RES_L();
	delay_ms(10);
	ST7789_RES_H();
	delay_ms(120);




    //屏幕句柄配置
    ST7789_SetShowDir(0);
    st7789Handle.isColorInverse=0;


//-----------------------ST7789V Frame rate setting-----------------//
//************************************************
    ST7789_WriteCmd(0x3A);        //65k mode
    ST7789_WriteData(0x55);  // 16位RGB565格式

    ST7789_WriteCmd(0xC5); 		//VCOM
    ST7789_WriteData(0x1A);

    // ST7789_WriteCmd(0x36);                 // 屏幕显示方向设置
    // ST7789_WriteData(0x00);

    //-------------ST7789V Frame rate setting-----------//
    ST7789_WriteCmd(0xb2);		//Porch Setting
    ST7789_WriteData(0x05);
    ST7789_WriteData(0x05);
    ST7789_WriteData(0x00);
    ST7789_WriteData(0x33);
    ST7789_WriteData(0x33);

    ST7789_WriteCmd(0xb7);			//Gate Control
    ST7789_WriteData(0x05);			//12.2v   -10.43v
    //--------------ST7789V Power setting---------------//
    ST7789_WriteCmd(0xBB);//VCOM
    ST7789_WriteData(0x3F);

    ST7789_WriteCmd(0xC0); //Power control
    ST7789_WriteData(0x2c);

    ST7789_WriteCmd(0xC2);		//VDV and VRH Command Enable
    ST7789_WriteData(0x01);

    ST7789_WriteCmd(0xC3);			//VRH Set
    ST7789_WriteData(0x0F);		//4.3+( vcom+vcom offset+vdv)

    ST7789_WriteCmd(0xC4);			//VDV Set
    ST7789_WriteData(0x20);				//0v

    ST7789_WriteCmd(0xC6);				//Frame Rate Control in Normal Mode
    ST7789_WriteData(0X01);			//111Hz

    ST7789_WriteCmd(0xd0);				//Power Control 1
    ST7789_WriteData(0xa4);
    ST7789_WriteData(0xa1);

    ST7789_WriteCmd(0xE8);				//Power Control 1
    ST7789_WriteData(0x03);

    ST7789_WriteCmd(0xE9);				//Equalize time control
    ST7789_WriteData(0x09);
    ST7789_WriteData(0x09);
    ST7789_WriteData(0x08);
    //---------------ST7789V gamma setting-------------//
    ST7789_WriteCmd(0xE0); //Set Gamma
    ST7789_WriteData(0xD0);
    ST7789_WriteData(0x05);
    ST7789_WriteData(0x09);
    ST7789_WriteData(0x09);
    ST7789_WriteData(0x08);
    ST7789_WriteData(0x14);
    ST7789_WriteData(0x28);
    ST7789_WriteData(0x33);
    ST7789_WriteData(0x3F);
    ST7789_WriteData(0x07);
    ST7789_WriteData(0x13);
    ST7789_WriteData(0x14);
    ST7789_WriteData(0x28);
    ST7789_WriteData(0x30);
        
    ST7789_WriteCmd(0XE1); //Set Gamma
    ST7789_WriteData(0xD0);
    ST7789_WriteData(0x05);
    ST7789_WriteData(0x09);
    ST7789_WriteData(0x09);
    ST7789_WriteData(0x08);
    ST7789_WriteData(0x03);
    ST7789_WriteData(0x24);
    ST7789_WriteData(0x32);
    ST7789_WriteData(0x32);
    ST7789_WriteData(0x3B);
    ST7789_WriteData(0x14);
    ST7789_WriteData(0x13);
    ST7789_WriteData(0x28);
    ST7789_WriteData(0x2F);

    ST7789_WriteCmd(0x20); 		//反显
    ST7789_WriteCmd(0x11); //Exit Sleep // 退出睡眠模式
    delay_ms(120); 
    ST7789_WriteCmd(0x29); //Display on // 开显示


    #if ST7789_EN_LVGL==0

    //把清除数据全为0xFF，因为0xFFFF是白色，也就是默认白色
    for(uint16_t i=0;i<ST7789_SEND_BUF_SIZE;i++)
    {
        st7789_sendBuf[i]=0xFF;
    }

    ST7789_Clear_All(WHITE);

    #endif
    // ST7789_DisplayChinses_40X56(0,0,"任务码：无",5);
    // ST7789_DisplayChinses_40X56(0,1,"任务：扫码",5);
}


//设置屏幕的显示范围，设置完后，可直接发送数据给屏幕
void ST7789_SetShowArea(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    ST7789_WriteCmd(0x2a);     //Column address set
    ST7789_WriteData((uint8_t)(x1>>8));    //start column 高位
    ST7789_WriteData((uint8_t)x1); //低位
    ST7789_WriteData((uint8_t)(x2>>8));    //end column
    ST7789_WriteData((uint8_t)x2);

    ST7789_WriteCmd(0x2b);     //Row address set
    ST7789_WriteData((uint8_t)(y1>>8));    //start row
    ST7789_WriteData((uint8_t)y1); 
    ST7789_WriteData((uint8_t)(y2>>8));    //end row
    ST7789_WriteData((uint8_t)y2);

    ST7789_WriteCmd(0x2C);     //Memory write
}


/// @brief 设置屏幕显示方向，
/// @param dir 0：竖屏，1：横屏
void ST7789_SetShowDir(uint8_t dir)
{
    if(dir>=2)
    {
        printf("dir is 0 or 1\n");
        return;
    }

    st7789Handle.showDir=dir;
    if(dir==0)
    {
        ST7789_WriteCmd(0x36); // 设置屏幕显示方向设置
        ST7789_WriteData(0x00); //竖屏
        st7789Handle.width=ST7789_HEIGHT;
        st7789Handle.height=ST7789_WIDTH;
        return;
    }

    //X-Y交换地址，实现逐列式。也就是变为横屏
    ST7789_WriteCmd(0x36);
    ST7789_WriteData(0x60);   // 把 MV 置 1
    st7789Handle.width=ST7789_WIDTH;
    st7789Handle.height=ST7789_HEIGHT;
}




/// @brief 多种颜色填充指定区域
/// @param color 颜色数组
void ST7789_MultiColor_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t* color)
{
    ST7789_SetShowArea(x1,y1,x2,y2);

    for(uint32_t i=0;i<(x2+1-x1)*(y2+1-y1);i++)
    {
        color[i]=(color[i]<<8)+(color[i]>>8);  //这里时高低位 位置互换。因为ST7789先收颜色RGB565高位
    }

    #if ST7789_EN_DMA
    ST7789_SendData_DMA((uint8_t*)color,((x2+1-x1)*(y2+1-y1))*2);
    uint8_t time=200;
    while (1)
    {
       if(SPI_DMA_GetRxFlag()==1) break;
       delay_us(2);
       time--;
       if(time==0) 
       {
        printf("timeout\n");
        break;
       }
    }
    
    #else
    ST7789_SendData((uint8_t*)color,((x2+1-x1)*(y2+1-y1))*2);
    #endif
}



/// @brief 单一颜色填充指定区域
/// @param color 颜色 
void ST7789_Color_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    ST7789_SetShowArea(x1,y1,x2,y2);

    //color=(color<<8)+(color>>8);  //这里时高低位 位置互换。因为ST7789先收颜色RGB565高位
    uint32_t allPixel=(x2+1-x1)*(y2+1-y1)*2; //该区域总像素

    //把发送缓存区都填满一种颜色数据
    for(uint32_t i=0;i<ST7789_SEND_BUF_SIZE/2;i++)
    {
        st7789_sendBuf[i*2]=color>>8;
        st7789_sendBuf[i*2+1]=color;
    }
    for(uint16_t i=0;i<allPixel/ST7789_SEND_BUF_SIZE;i++)
    {
        ST7789_SendData(st7789_sendBuf,ST7789_SEND_BUF_SIZE);
    }
    ST7789_SendData(st7789_sendBuf,allPixel%ST7789_SEND_BUF_SIZE);
}



//清空屏幕
//黑或白
void ST7789_Clear_All(uint16_t color)
{
    memset(st7789_sendBuf,color,sizeof(st7789_sendBuf));
    ST7789_SetShowArea(0,0,st7789Handle.width-1,st7789Handle.height-1);

    for(uint16_t i=0;i<ST7789_SNED_NUM;i++)
    {
        ST7789_SendData(st7789_sendBuf,ST7789_SEND_BUF_SIZE);
    }

    //如果还有些屏幕没清除的话
    ST7789_SendData(st7789_sendBuf,(ST7789_WIDTH*ST7789_HEIGHT*2)%ST7789_SEND_BUF_SIZE);
}


uint32_t st7789_sendBufNum=0;
/// @brief 清除指定区域的屏幕数据
/// @color 黑或白 
/// @param x1,y1,x2,y2 区域坐标，左闭右闭原则。x[0,320-1] 等于就是 0~319
void ST7789_Clear(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    memset(st7789_sendBuf,color,sizeof(st7789_sendBuf));
    ST7789_SetShowArea(x1,y1,x2,y2);

    for(uint16_t i=0;i<(((x2+1-x1)*(y2+1-y1)*2)/ST7789_SEND_BUF_SIZE);i++)
    {
        ST7789_SendData(st7789_sendBuf,ST7789_SEND_BUF_SIZE);
    }
    ST7789_SendData(st7789_sendBuf,(st7789_sendBufNum%ST7789_SEND_BUF_SIZE));
}






void display_char16_16(unsigned int x,unsigned int y,unsigned long color,unsigned char word_serial_number)
{
    unsigned int column;
    unsigned char tm=0,temp=0,index=0;

    ST7789_WriteCmd(0x2a);    //Column address set
    ST7789_WriteData(x>>8);    //start column
    ST7789_WriteData(x);
    x=x+15;
    ST7789_WriteData(x>>8);    //end column
    ST7789_WriteData(x);

    ST7789_WriteCmd(0x2b);     //Row address set
    ST7789_WriteData(y>>8);    //start row
    ST7789_WriteData(y); 
    y=y+15;
    ST7789_WriteData(y>>8);    //end row
    ST7789_WriteData(y);
    ST7789_WriteCmd(0x2C);     //Memory write

    for(column=0;column<32;column++)  //column loop
    {
        temp=Chinese40X56[ word_serial_number ][index];

        //发送一字节RGB数据,带颜色
        for(tm=0;tm<8;tm++)
        {
            if(temp&0x01)
            {
                ST7789_WriteData(color>>8);
                ST7789_WriteData(color);
            }
            else 
            {
                ST7789_WriteData(0XFF);
                ST7789_WriteData(0XFF);
            }
        temp>>=1;
        }
        index++;
    }
}



/// @brief 
/// @param x 0~7
/// @param y 0~3
/// @param color RED,GREEN,BLUE
/// @param word_serial_number 汉字序号
void ST7789_DisplayChinese_40X56(unsigned int x,unsigned int y,unsigned long color,unsigned char word_serial_number)
{
    //待发送的数据，主要存每个像素的颜色(因为颜色需要两个字节，所以 X2)
    static uint8_t sendData[FONT_DATA_SIZE*2*8];
    uint8_t temp=0,tm=0;
    //横向遍历字体数据字节个数
    for(uint16_t i=0;i<FONT_DATA_SIZE;i++)  //column loop
    {
        temp=Chinese40X56[word_serial_number][i];

        //发送一字节RGB数据,带颜色
        for(tm=0;tm<8;tm++)
        {
            if(temp&0x01)
            {
                sendData[i*16+tm*2+0]=color>>8;
                sendData[i*16+tm*2+1]=color;
            }
            else 
            {
                sendData[i*16+tm*2+0]=0xFF;
                sendData[i*16+tm*2+1]=0xFF;
            }
        temp>>=1;
        }
    }


   //1,1
    uint8_t row=x;
    uint8_t col=y;
    y=(7-row)*FONT_WIDTH;
    x=FONT_HEIGHT*col;
    ST7789_WriteCmd(0x2a);    //Column address set
    ST7789_WriteData(x>>8);    //start column
    ST7789_WriteData(x);
    x=x+FONT_HEIGHT-1;
    ST7789_WriteData(x>>8);    //end column
    ST7789_WriteData(x);

    ST7789_WriteCmd(0x2b);     //Row address set
    ST7789_WriteData(y>>8);    //start row
    ST7789_WriteData(y); 
    y=y+FONT_WIDTH-1;
    ST7789_WriteData(y>>8);    //end row
    ST7789_WriteData(y);
    ST7789_WriteCmd(0x2C);     //Memory write

    

    ST7789_SendData(sendData,sizeof(sendData)/sizeof(uint8_t));
}


/// @brief 显示一堆中文
/// @param x 
/// @param y 
/// @param chineses 中文组 
/// @param size 中文的个数
void ST7789_DisplayChinses_40X56(unsigned int x,unsigned int y,uint8_t* chineses,uint8_t size)
{
    for(uint8_t i=0;i<size;i++)
    {
        if(x%8==0&&x!=0) 
        {
            x=0;
            y++;
        }
        ST7789_DisplayChinese_40X56(x++,y,RED,Font_GetFontId(chineses));
        chineses+=3;
    }
}



void ST7789_Display_Picture(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
   ST7789_SetShowArea(x1,y1,x2,y2);

    uint8_t temp=0;
    for(uint32_t i=0;i<5724;i++)             //ROW loop
    { 
        ST7789_WriteData(gImage_pic[i]);
        //temp=gImage_pic[i];
        //发送一字节RGB数据,带颜色
        // for(uint8_t tm=0;tm<8;tm++)
        // {
        //    if((temp&0x01)==0)
        //     {
        //         ST7789_WriteData(color>>8);
        //         ST7789_WriteData(color);
        //     }
        //     else 
        //     {
        //         ST7789_WriteData(0XFF);
        //         ST7789_WriteData(0XFF);
        //     }
        //     temp>>=1;
        // }
    }
}


//画点函数
void lcd_draw_point(uint16_t x,uint16_t y,uint32_t color)
{
    ST7789_Color_Fill(x,y,x,y,color);
}



uint16_t ST7789_GetCurWidth()
{
    return st7789Handle.width;
}
uint16_t ST7789_GetCurHeight()
{
    return st7789Handle.height;
}

//获取用于该驱动的显存指针，以便在屏幕显示 用DMA+双缓冲时 节省内存，不然又得开辟一块内存用于另一个缓存区
uint8_t* ST7789_GetBufPointer()
{
    return st7789_sendBuf;
}