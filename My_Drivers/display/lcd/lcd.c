/*
    2025/12/2:
        1.重构了代码，将全局变量都放到一个结构体里，且缓冲区以指针形式放入缓冲区。
        2.将播放媒体的代码进行优化，使帧率计算更合理，使视频每一帧都可以调用自己传入的函数，以便对每一帧的事件进行处理

*/

#include"lcd.h"
#include"lv_port_disp_template.h"


#if LCD_EN_LVGL==1
#include"lvgl.h"
#include"main_screen.h"
#endif


typedef struct _LCD
{
    uint16_t width;
    uint16_t height;

    uint8_t* buf1;
    uint8_t* buf2;
    uint32_t bufSize;
}LCD_Handle;

LCD_Handle lcd={0};


uint8_t LCD_Init()
{
    #ifdef LCD_EN_ST7789
    ST7789_Init();

    /**************此处需要修改********/
    lcd.buf1=ST7789_GetBufPointer(); //获取自己驱动给的缓存指针
    #endif


    #ifdef LCD_EN_ILI9341
    lcd.buf1=lv_port_get_buf1(); /*改为自己的1*/
    lcd.bufSize=LV_PORT_BUF_SIZE;/*2*/
    #if LCD_EN_DMA
    lcd.buf2=lv_port_get_buf2(); /*3*/
    #endif

    ILI9341_Init(lcd.buf1,lcd.bufSize); /*需要给驱动一个显示缓冲区*/

    /**************此处需要修改********/
    #endif

    lcd.width=ILI9341_GetCurWidth();
    lcd.height=ILI9341_GetCurHeight();
}


//设置屏幕的显示范围，设置完后，可直接发送数据给屏幕，以显示文件
void LCD_SetShowArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    #ifdef LCD_EN_ST7789
    ST7789_SetShowArea(x1,y1,x2,y2);
    #endif

    #ifdef LCD_EN_ILI9341
    ILI9341_SetShowArea(x1,y1,x2,y2);
    #endif
}


//屏幕清空为黑色或白色
void LCD_Clear_All(uint16_t color) 
{
    #ifdef LCD_EN_ST7789
    ST7789_Clear_All(color);
    #endif


    #ifdef LCD_EN_ILI9341
    ILI9341_Clear_All(color);
    #endif
}


/*清除任意一块区域*/ 
void LCD_Clear(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color) 
{
    #ifdef LCD_EN_ST7789
    ST7789_Clear(x1,y1,x2,y2,color);
    #endif


    #ifdef LCD_EN_ILI9341
    ILI9341_Clear(x1,y1,x2,y2,color);
    #endif
}

uint8_t LCD_SendData(uint8_t *data, uint16_t size)
{
    #ifdef LCD_EN_ST7789

    #if LCD_EN_DMA
    return ST7789_SendData_DMA(data,size);
    #else 
    return ST7789_SendData(data,size);
    #endif

    #endif


    #ifdef LCD_EN_ILI9341
    #if LCD_EN_DMA
    return ILI9341_SendData_DMA(data,size);
    #else 
    return ILI9341_SendData(data,size);
    #endif
    #endif
}

//设置屏幕显示方向。0：竖屏，1：横屏
void LCD_SetShowDir(uint8_t showDir)
{
    #ifdef LCD_EN_ST7789
    ST7789_SetShowDir(showDir);
    #endif
    

    #ifdef LCD_EN_ILI9341
    ILI9341_SetShowDir(showDir);
    #endif

    lcd.width=ILI9341_GetCurWidth();
    lcd.height=ILI9341_GetCurHeight();
}

#if LCD_EN_DMA 
//等待DMA完成的标志，0数据搬运完成，2超时。函数内部自己回拉高片选，可不加CS_H
uint8_t LCD_WaitDMA_Over()
{
    return ILI9341_WaitDMA_Over(100);
}
#endif



/*****************************修改结束**************************/



uint16_t LCD_GetWidth(){return lcd.width;}
uint16_t LCD_GetHeight(){return lcd.height;}




/// @brief 获取指定图片/视频的宽高 
/// @return 0:成功。1:失败
uint8_t LCD_GetMediaSize(const char* filePath,uint16_t* w,uint16_t* h)
{
    //1.获取读文件指针
    FIL fp;

    uint8_t ret=FS_API_Get_FP(filePath,&fp,0);
    if(ret)
    {
        printf("get fp fail:%d\n",ret);
        return 1;
    }
    //2.从SD卡获取文件的宽，高
    uint32_t readSize=0; //单次读取的数据大小
    ret=FS_API_Read_By_FP(&fp,lcd.buf1,4,&readSize);
    if(ret||readSize!=4)
    {
        printf("read w,h fail:%d,or read size=%d\n",ret,readSize);
        FS_API_Close(&fp); //关闭该文件
        return 1;
    }

    (*w)=((uint16_t)lcd.buf1[1]<<8)+lcd.buf1[0];
    (*h)=((uint16_t)lcd.buf1[3]<<8)+lcd.buf1[2];

    FS_API_Close(&fp); //关闭该文件
    return 0;
}



/// @brief 获取文件大小
/// @param filePath 文件路径
/// @param size 文件大小
/// @return 0:成功
uint8_t LCD_GetFileSize(const char* filePath,uint32_t* size)
{
    //1.获取读文件指针
    FIL fp;

    uint8_t ret=FS_API_Get_FP(filePath,&fp,0);
    if(ret)
    {
        printf("get fp fail:%d\n",ret);
        return 1;
    }

    //2.获取数据大小
    *size=f_size(&fp);

    /*3.关闭文件*/
    FS_API_Close(&fp);

    return 0;
}


/*初始化媒体数据，得到视频宽高，总数据大小，还有播放的左上角坐标(x,y)*/
uint8_t LCD_DispMedia_Init(uint16_t x1,uint16_t y1,char* filePath,ALIGN_MODE align,Vedio_Info* vedioInfo)
{
    printf("disp init:%s\n",filePath);

    //1.获取读文件指针
    FIL* fp=&vedioInfo->fp;

    uint8_t ret=FS_API_Get_FP(filePath,fp,0);
    if(ret)
    {
        printf("get fp fail:%d\n",ret);
        return 1;
    }


    //2.获取该文件的文件大小
    uint32_t fileSize=f_size(fp); 
    if(fileSize/1024<1024) Printf("picture size:%fKB\n",fileSize/1024.0f);
    else Printf("vedio size:%fMB\n",fileSize/1024.0f/1024.0f);
    

    //3.从SD卡获取文件的宽，高
    uint32_t readSize=0; //单次读取的数据大小
    ret=FS_API_Read_By_FP(fp,lcd.buf1,4,&readSize);
    if(ret||readSize!=4)
    {
        printf("read w,h fail:%d,or read size=%d\n",ret,readSize);
        FS_API_Close(fp);//关闭该文件
        return 1;
    }


    uint16_t width=((uint16_t)lcd.buf1[1]<<8)+lcd.buf1[0];
    uint16_t height=((uint16_t)lcd.buf1[3]<<8)+lcd.buf1[2];
    printf("pic w,h=%d,%d\n",width,height);

    /*判断视频宽高，如果超了就要就此返回*/
    if(width>lcd.width ||width==0 ||height>lcd.height ||height==0) 
    {
        printf("lcd width,height error,may be vedio data have problem\n");
        return 1;
    } 


    //对齐方式
    if(align==ALIGN_NORMAL) LCD_SetShowArea(x1,y1,width+x1-1,height+y1-1);  
    else if(align==ALIGN_CENTER)
    {
        x1=(lcd.width-width)/2;
        y1=(lcd.height-height)/2;
        LCD_Clear(x1,y1,x1+width-1,y1+height-1,WHITE);
        LCD_SetShowArea(x1,y1,x1+width-1,y1+height-1);

        //printf("x1:%d,y2:%d,x2:%d,y2:%d,w:%d,h:%d\n",x1,y1,x1+width-1,y1+height-1,lcd.width,lcd.height);
    }
    else if(align==ALIGN_CENTER_UP_30)
    {
        x1=(lcd.width-width)/2;
        y1=(lcd.height-30-height)/2+30; 
        LCD_Clear(x1,y1,x1+width-1,y1+height-1,WHITE);
        LCD_SetShowArea(x1,y1,x1+width-1,y1+height-1);

        //printf("x1:%d,y2:%d,x2:%d,y2:%d,w:%d,h:%d\n",x1,y1,x1+width-1,y1+height-1,lcd.width,lcd.height);
    }

    vedioInfo->allSize=fileSize;
    vedioInfo->x=x1;
    vedioInfo->y=y1;
    vedioInfo->w=width;
    vedioInfo->h=height;
    vedioInfo->allFrameNum=(fileSize-4)/(width*height*2);
    vedioInfo->oneFrameDataSize=width*height*2;

    return 0;
}



/// @brief 播放一张图片或一帧视频
/// @return 0成功
uint8_t LCD_DispMedia_OneFrame(Vedio_Info* vedioInfo)
{
    uint8_t ret=0;
    FIL* fp=&vedioInfo->fp;

    uint8_t swtichBufFlag=0; //用与切换缓冲区。双缓存区+DMA时用
    uint8_t isFirst=1; 

    uint32_t oneFrameReadSize=0;
    uint32_t readSize=0; /*单次读取文件时，读取的文件数据大小*/
    uint32_t sendDataSize=lcd.bufSize; //每次发送的图像数据大小
    uint32_t oneFrameDataSize=(2*vedioInfo->w*vedioInfo->h); //一帧视频数据需要的数据大小。用于判断一帧到了每，以便调用帧函数 


    //(阻塞读->DMA写->阻塞读)->(DMA写->阻塞读)    
    while(1)
    {
        //5.开始第一次从SD卡读取视频数据。如果使用双缓冲区+DMA则只执行一次该函数
        if(isFirst==1)
        {
            isFirst=0;
            #if LCD_EN_DMA
            ret=FS_API_Read_By_FP(fp,lcd.buf1,lcd.bufSize,&readSize);
            if(ret) 
            {
                printf("read fail:%d\n",ret);
                break;
            }
            #endif
        }
        

        //printf("ss:%d\n",readSize);

        //6.发送数据给屏幕.
        if(swtichBufFlag==0) ret=LCD_SendData(lcd.buf1,readSize);
        #if LCD_EN_DMA
        else ret=LCD_SendData(lcd.buf2,readSize);
        #endif
        if(ret)
        {
            printf("send fail:%d\n",ret);
            break;
        }

        oneFrameReadSize+=readSize; //读取数据加上


        //目的是使能完整播放完一帧数据
        sendDataSize=oneFrameDataSize-(oneFrameReadSize%oneFrameDataSize);
        if(sendDataSize>lcd.bufSize) sendDataSize= lcd.bufSize;


        #if LCD_EN_DMA 
        if(oneFrameReadSize!=oneFrameDataSize) 
        {
            //7.继续从SD卡读取文件数据
            if(swtichBufFlag==0) ret=FS_API_Read_By_FP(fp,lcd.buf2,sendDataSize,&readSize);
            else ret=FS_API_Read_By_FP(fp,lcd.buf1,sendDataSize,&readSize);
            if(ret) 
            {
                printf("read fail:%d\n",ret);
                break;
            }
        }
        
        

        //等待DMA搬运数据屏幕显示的数据完成
        ret=LCD_WaitDMA_Over();
        if(ret)printf("dma timeout\n");
        swtichBufFlag=(swtichBufFlag==0)?1:0; //0,1变换
        #endif /*双缓冲区结束*/


        /*发送完一帧数据*/
        if(oneFrameReadSize==oneFrameDataSize) //已经发送一帧数据，可以调用帧函数了
        {
            memset(lcd.buf1,0,lcd.bufSize);
            memset(lcd.buf2,0,lcd.bufSize);
            return 0;
        }   
    }

    return 0;
}




/// @brief 播放媒体(图片或视频)
/// @param x1,y1 媒体播放的左上角坐标
/// @param filePath 文件路径
/// @return 0:成功，其它:失败
uint8_t LCD_DispMedia(uint16_t x1, uint16_t y1,const char *filePath,ALIGN_MODE align,FrameFunc frameFunc)
{
    uint8_t ret=0;
    Vedio_Info vedioInfo={0};

    /*1. 初始化媒体*/
    ret=LCD_DispMedia_Init(x1,y1,filePath,align,&vedioInfo);
    if(ret)
    {
        printf("media init fail:%d\n",ret);
        return ret;
    }

    
    uint32_t startTime=HAL_GetTick(); //计算播放整个文件需要的时间
    uint32_t secendStartTime=HAL_GetTick(); //视频每秒开始的系统时间。 视频用于计算每秒帧率
    uint32_t oneSecondReadSize=0; /*一秒时间读取的数据大小*/
    uint32_t showDataSize=vedioInfo.allSize-4; /*用于显示的数据大小*/

    /*2. 播放媒体数据,表示可以发数据了*/
    printf("all frame:%d\n",vedioInfo.allFrameNum);
    printf("display media start\n");

    while(1)
    {
        ret=LCD_DispMedia_OneFrame(&vedioInfo);
        if(ret) 
        {
            printf("disp media one frame fail:%d\n",ret);
            break;
        }
        vedioInfo.readSize+=vedioInfo.oneFrameDataSize;
        oneSecondReadSize+=vedioInfo.oneFrameDataSize;
        vedioInfo.frameIndex++;

        //printf("play one frame over\n");
        //delay_ms(1000);
        /*一帧结束，开始响应LVGL的UI事件，这里会调用回调函数*/
        if(frameFunc!=NULL) 
        {
            ret=frameFunc(&vedioInfo);
            if(ret==1)break;
        }


        //delay_ms(1000);

        //计算帧率，每秒算一次。计算公式=读取的数据/一帧画面需要的数据大小/读取的时间。
        if((HAL_GetTick()-secendStartTime)>=1000)
        {
            /*不要用浮点数，不然会出问题，大概是数据太大了*/
            Printf("frame:%f,%f%%\n",(float)oneSecondReadSize/vedioInfo.oneFrameDataSize/((HAL_GetTick()-secendStartTime)/1000.0f),(float)vedioInfo.readSize/showDataSize*100);
            oneSecondReadSize=0;
            secendStartTime=HAL_GetTick();
        }


        if((vedioInfo.readSize+4)==vedioInfo.allSize)break;  //要加上四个字节的w h 数据
    }
    
    Printf("display media suc,use time:%fs\n",(HAL_GetTick()-startTime)/1000.0f);
   
    LCD_WaitDMA_Over(); /*等待SPI传输完成并拉高CS*/
    FS_API_Close(&vedioInfo.fp); //关闭该文件

    return 0;
}


//画图，是直接在当前图层绘图。不会清屏
uint8_t LCD_DrawImg(uint16_t x1, uint16_t y1,const uint8_t* imgPath) 
{
    uint8_t ret=0;
    ret=LCD_DispMedia(x1,y1,imgPath,ALIGN_NORMAL,NULL);
    return ret;
}



/// @brief 显示文件。这个是用于全屏展示文件的，类似相册，会清屏，居中
/// @param path 盘符。"0:""
/// @param imgPath 文件路径。"0:test.bin"
/// @return 0成功
uint8_t LCD_DispImg(const uint8_t* imgPath)
{
    //LCD_Clear_All(WHITE); //先清空数据

    uint8_t ret=0;
    ret=LCD_DispMedia(0,0,imgPath,ALIGN_CENTER_UP_30,NULL);
    if(ret)
    {
        printf("draw img fail:%d\n",ret);
        return ret;
    }
    
    return 0;
}



/// @brief 居中播放视频。
/// 使用阻塞方式，缓存区2048*3 帧率大概8.93帧；使用DMA方式，大概9.74帧;缓存区 2048 使用DMA+双缓冲，帧率大概14.8帧
///  所以推荐使用DMA+双缓冲区方式。SD卡读取数据和spi发送数据各一个缓存区。
/// @param imgPath 文件名。"0:test.bin"
/// @return 0成功
uint8_t LCD_PlayVedio(const char* vedioPath,FrameFunc frameFunc)
{
    uint8_t ret=0;
    ret=LCD_DispMedia(0,0,vedioPath,ALIGN_CENTER,frameFunc);
    if(ret)
    {
        printf("play vedio fail:%d\n",ret);
    }

    return ret;
}


