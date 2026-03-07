#include"audio_recording.h"
#include"audio_input_api.h"
#include"fs_api.h"
#include"audio_conf.h"

typedef struct 
{
    uint16_t* buf1; /*播放音频的缓存区*/
    uint16_t* buf2;
    uint16_t* txBuf; /*存放得到的音频数据缓存区，待会会要对该缓存区数据进行处理*/
    uint16_t* rxBuf;  /*存放从模块得到的音频数据的缓存区*/
    uint16_t* writeBuf; /*真正把数据写入文件时用的缓存区*/ 
    uint16_t bufSize; /*存放音频数据的缓存区大小 (单位 半字)*/

    uint32_t sampleRate; /*采样率*/
    uint8_t recordStatus; /*录音处于的播放状态*/
    uint32_t recordedSize; /*已经录制的音频数据大小(字节)*/
    uint16_t recordedDuration; /*已录音的时长 单位s*/
}AudioRecording_Info;
static AudioRecording_Info aRInfo={0};


/*低频滤波参数 IIR滤波 差分方程 y[n] = x[n] - x[n-1] + α·y[n-1]*/
/*y[n]为滤波后的音频数据，x[n]为输入音频数据,x[n-1]为上一个音频数据*/
int16_t x_prev=0; /*x[n-1]*/
float y_prev=0; /*y[n-1]*/
float alpha=0.9998f; /*α*/


static FIL fp={0}; /*文件指针*/
static uint8_t swBuf=0; /*两个缓存区切换标志位*/ 
// static uint32_t fileReadSize=0; /*每次读取音频文件真正读到的数据大小*/
static uint16_t timeout=1000; /*(ms)读取麦克风的数据超时时间*/
static uint32_t t1=0; /*记录时间，这里和timeout 一起使用，实现超时判断*/

void Audio_Recording_Init(uint32_t sampleRate)
{
    aRInfo.sampleRate=sampleRate;
    aRInfo.buf1=audio_buf1;
    aRInfo.buf2=audio_buf2;

    aRInfo.bufSize=AUDIO_BUF_SIZE;

    Audio_Input_Init(sampleRate);
}

/// @brief 录音开始，进行各变量初始化
/// @param filePath 写入的文件路径
/// @return 0成功
uint8_t Audio_Recording_Start(const char* filePath)
{
    uint8_t ret=FS_API_Get_FP(filePath,&fp,1);
    if(ret)
    {
        printf("get fp fail:%d\n",ret);
        return ret;
    }

    /*初始化各数据*/
    aRInfo.recordedDuration=0;
    aRInfo.recordedSize=0;
    aRInfo.recordStatus=AR_STATUS_START; /*正在录音*/
    aRInfo.rxBuf=aRInfo.buf1;
    aRInfo.txBuf=aRInfo.buf1;

    x_prev=0;
    y_prev=0;

    swBuf=0;

    ret=Audio_Input_Receive(aRInfo.rxBuf,aRInfo.bufSize/2);
    if(ret)
    {
        printf("audio recording fail:%d\n",ret);
        return AR_STATUS_ERR;
    }
    delay_ms(1000);
    Audio_Input_WaitRxCplt();

    return 0;
}


/// @brief 录音一次,并且只适合 从麦克风读取24/32位数据读取,保存为16位数据。也就是调用一次录音函数(使用了DMA)和写入数据和记录录音时长
/// @param fp 文件指针
/// @return 返回录音状态，需要自己处理状态。
static AR_Status Audio_Recording_OneTimes_To16(FIL* fp)
{
    uint8_t ret=0;
    if(aRInfo.recordStatus==AR_STATUS_START)
    {
        aRInfo.recordStatus=AR_STATUS_RECORDING;
        ret=Audio_Input_Receive(aRInfo.rxBuf,aRInfo.bufSize/2);
        if(ret)
        {
            printf("audio recording fail:%d\n",ret);
            return AR_STATUS_ERR;
        }
        t1=HAL_GetTick();
    }

    /*等待音频数据读取完毕*/
    while(Audio_Input_WaitRxCplt()) 
    {
        if((HAL_GetTick()-t1)>timeout)
        {
            printf("wait recording timeout\n");
            return AR_STATUS_ERR;
        }
        return AR_STATUS_RECORDING;
    }
    t1=HAL_GetTick();
    printf("------\n");

    /*缓存区切换*/
    if(swBuf==0)
    {
      aRInfo.rxBuf=aRInfo.buf2;
      aRInfo.txBuf=aRInfo.buf1;
    }
    else
    {
      aRInfo.rxBuf=aRInfo.buf1;
      aRInfo.txBuf=aRInfo.buf2;
    }

    /*开始从麦克风读取音频数据*/
    ret=Audio_Input_Receive(aRInfo.rxBuf,aRInfo.bufSize/2);
    if(ret)
    {
        printf("audio recording fail:%d\n",ret);
        return AR_STATUS_ERR;
    }

    /*下面都是处理得到的麦克风数据，因为我的这个模块得到的数据开始的数据太乱了，有左声道32位中低16位开头，也有右声道低16位开头，还有右声道32位开头
    而我想要的是左声道32位开头，所以这里都是判断前面三种情况，然后变成左声道32位开头，再取高16位*/
    uint8_t start=0;
    uint8_t end=0;
    uint16_t size=0;
    if(aRInfo.txBuf[0]==0 &&aRInfo.txBuf[1]>0)
    {
      start=1;
      end=1;
    }
    else if(aRInfo.txBuf[0]==0 &&aRInfo.txBuf[1]==0)
    {
      start=2;
      end=0;
    }
    else if(aRInfo.txBuf[0]>0 &&aRInfo.txBuf[1]==0)
    {
      start=3;
      end=1;
    }
    size=(aRInfo.bufSize-end-start)/4; /*录音得到的左声道有效数据大小*/

    int32_t tmp=0;
    int16_t sample_16=0; 

    /*提取出左声道的数据*/
    for(uint16_t i=0;i<size;i++)
    {
      //printf("0x%04x ",rxBuf[i]); /*打印得到的数据*/
      //buf[i]=((rxBuf[i*2+start]<<16)+rxBuf[i*2+1+start])>>16;
        sample_16=(int16_t)aRInfo.txBuf[i*4+start];

        /*滤除低频信号 用了IIR滤波，具体参数功能，去上面看，每个变量有说明作用*/
        y_prev=sample_16-x_prev+alpha*y_prev;
        x_prev=sample_16;

        /*限幅在int16*/
        tmp=((int32_t)y_prev);
        if(tmp>32767)
        {
            aRInfo.txBuf[i*2]=32767;
        }
        else if(tmp<-32768)
        {
            aRInfo.txBuf[i*2]=-32768;
        }
        else aRInfo.txBuf[i*2]=(uint16_t)tmp;

        /*右声道的数据和左声道一样*/
        aRInfo.txBuf[i*2+1]=aRInfo.txBuf[i*2];
        //printf("%d ",buf[i]);
    }

    ret=FS_API_Write_By_FP(fp,(uint8_t*)aRInfo.txBuf,size*4);
    if(ret)
    {
        printf("write fail:%d\n",ret);
        return AR_STATUS_ERR;
    }

    /*录音文件数据增加*/
    aRInfo.recordedSize+=(size*4);

    swBuf=(swBuf==0)?1:0;
    return AR_STATUS_RECORDING;
}


/*录音处理函数，直接调用即可。会根据状态自动结束录音和自动启动录音*/
AR_Status Audio_Recording_Handler()
{
    if(aRInfo.recordStatus==AR_STATUS_RECORDING || aRInfo.recordStatus==AR_STATUS_START)
    {
        aRInfo.recordStatus=Audio_Recording_OneTimes_To16(&fp);
    }
    
    if(aRInfo.recordStatus==AR_STATUS_STOP || aRInfo.recordStatus==AR_STATUS_ERR)
    {
        AR_Status s=aRInfo.recordStatus;
        FS_API_Close(&fp);
        aRInfo.recordStatus=AR_STATUS_NO_START;
        return s;
    }

    return aRInfo.recordStatus;
}


/*暂停录音*/
void Audio_Recording_PAUSE()
{
    aRInfo.recordStatus=AR_STATUS_PAUSE;
}

/*停止录音/结束录音*/
void Audio_Recording_Stop()
{
    aRInfo.recordStatus=AR_STATUS_STOP;
}

/*继续录音*/
void Audio_Recording_Continue()
{
    aRInfo.recordStatus=AR_STATUS_RECORDING;
}


/*获取录音的实时时长*/
uint16_t Audio_Recording_GetDuration()
{
    return aRInfo.recordedSize/(aRInfo.sampleRate*4);
}






void Audio_Recording_Example()
{
    
// #define RX_BUF_SIZE 6656
// uint16_t buf1[RX_BUF_SIZE]={0};
// uint16_t buf2[RX_BUF_SIZE]={0};
// uint16_t buf[RX_BUF_SIZE/2]={0};
// uint16_t* rxBuf=NULL;
// uint16_t* txBuf=NULL;


// #define AUDIO_MODE 3 /*0:录音后立刻播放，1只录音，2只播放*/
// int main(void)
// {
//   //SYS_Set_Vector_Table(FLASH_BASE,STM_FLASH_APP_OFSET_ADDR);
//   HAL_Init();
//   SystemClock_Config();
//   TIM_Base_Init(72-1,1000-1); //作为HAL库的时钟源

  
//   delay_init(72);
//   HAL_Delay(100);

//   UART_Init(115200);
//   //WIFI_APP_Init(115200);

//   FS_API_Init((const uint8_t*)DRIVER_DIR); //SD卡和文件系统FATFS初始化
//   FS_API_ScanDir("S:");

//   I2S_Init(I2S_AUDIOFREQ_48K);
//   Audio_Player_Init(I2S_AUDIOFREQ_48K,buf1,buf2,RX_BUF_SIZE);


//   uint8_t ret=0;


//   #if AUDIO_MODE==2
//   ret=Audio_Player_Play_Init("S:audio.pcm");
//   printf("audio init fail:%d\n",ret);
//   #endif

//   uint16_t b=(0x7000);
//   uint16_t b1=0x8000;
//   int16_t b2=b;
//   printf("n1=%d,0x%04x\n",b2,b2);
//   int32_t tmm=(((int32_t)0x8700)|0xffff0000)*3;
//   printf("n2=%d,0x%x\n",tmm,tmm);
//   int32_t b3=b;
//   printf("n3=%d,0x%08x\n",b3,b3);
//   b3=(int32_t)b1;
//   printf("n4=%d,0x%08x\n",b3,b3);
//   b3=b3*4;
//   printf("n4=%d,0x%08x\n",b3,b3);
//   printf("n4=%d,0x%08x\n",(int32_t)(b-32767),b3);

//   // uint8_t rxData[64]={0};
//   // uint8_t rxData1[2048]={0};
//   // delay_ms(6000);
//   // Weather_UpdateInfo_RealTime("HangZhou");
//   // UART_ClearRxFlag(3);
//   // Weather_UpdateInfo("HangZhou");
//   // while(1)
//   // {
//   //   if(UART_GetRxData(rxData,sizeof(rxData),1)==0)
//   //   {
//   //     printf("rx:%s\n",rxData);
//   //     UART_Transmit_String(rxData,sizeof(rxData),3);
//   //   }

//   //   if(UART_GetRxData(rxData1,sizeof(rxData1),3)==0)
//   //   {
//   //     printf("esp rx:%s\n",rxData1);
//   //   }
//   // }







//   #if AUDIO_MODE==1
//   /*打开文件*/
//   ret=f_unlink("S:audio.pcm");
//   {
//     printf("delete fail:%d\n",ret);
//   }
//   FIL fp;
//   ret=FS_API_Get_FP("S:audio.pcm",&fp,1);
//   if(ret)
//   {
//     printf("get fp file:%d\n",ret);
//   }

//   #endif

//   uint8_t ret2=0;
//   uint8_t num1=0;
//   rxBuf=buf1;
//   txBuf=buf1;
//   delay_ms(1000);
//   ret2=I2S_Receive_DMA((uint16_t*)rxBuf,RX_BUF_SIZE/2,I2S1_ID);
//   if(ret2)
//   {
//     printf("i2s rx fail:%d\n",ret2);
//   }
//   delay_ms(500);

//   ret2=I2S_Receive_DMA((uint16_t*)rxBuf,RX_BUF_SIZE/2,I2S1_ID);
//   if(ret2)
//   {
//     printf("i2s rx fail:%d\n",ret2);
//   }

//   delay_ms(500);
//   uint16_t timeout=0;

//   uint8_t sw=0;
//   uint32_t t1=0;
//   uint32_t t2=HAL_GetTick();
//   printf("tt2=%d\n",t2);

//   int16_t x_prev=0;
//   float y_prev=0; /*FIR滤波器*/
//   float alpha=0.9998f;

//   while(1)
//   {
//     printf("t1=%d\n",HAL_GetTick()-t1);
//     timeout=3000;
//     while(I2S_WaitRxCplt(I2S1_ID)) /*读取完毕*/
//     {
//       delay_us(100);
//       timeout--;
//       //printf("running...%d\n",timeout);
//       if(timeout==0)
//       {
//         printf("timeout-e\n");
//         continue;
//       }
//     }
//     printf("t2=%d\n",HAL_GetTick()-t1);

//     /*缓存区切换*/
//     if(sw==0)
//     {
//       rxBuf=buf2;
//       txBuf=buf1;
//     }
//     else
//     {
//       rxBuf=buf1;
//       txBuf=buf2;
//     }

//     t1=HAL_GetTick();
//     ret2=I2S_Receive_DMA((uint16_t*)rxBuf,RX_BUF_SIZE/2,I2S1_ID);
//     if(ret2)
//     {
//       printf("i2s rx fail:%d\n",ret2);
//     }

//     // printf("rx----\n");

//     // t1=HAL_GetTick();
//     // ret2=I2S_Receive((uint16_t*)rxBuf,RX_BUF_SIZE/2,I2S1_ID);
//     // if(ret2)
//     // {
//     //   printf("i2s rx fail:%d\n",ret2);
//     //   continue;
//     // }
//     // printf("t=%d\n",HAL_GetTick()-t1);


//     // ret2=I2S_Transmit_DMA((uint16_t*)buf,RX_BUF_SIZE,I2S2_ID);
//     // if(ret2)
//     // {
//     //   printf("i2s tx fail:%d\n",ret2);
//     //   continue;
//     // }


//     printf("-------------------------999-\n");
//     uint8_t start=0;
//     uint8_t end=0;
//     uint16_t size=0;
//     if(txBuf[0]==0 &&txBuf[1]>0)
//     {
//       start=1;
//       end=1;
//     }
//     else if(txBuf[0]==0 &&txBuf[1]==0)
//     {
//       start=2;
//       end=0;
//     }
//     else if(txBuf[0]>0 &&txBuf[1]==0)
//     {
//       start=3;
//       end=1;
//     }

//     size=(RX_BUF_SIZE-end-start)/4; /*录音得到的左声道有效数据大小*/
//     //size=RX_BUF_SIZE; /*录音得到的左声道有效数据大小*/
//     int32_t tmp=0;
    
//     int16_t sample_16=0;

//     for(uint16_t i=0;i<size;i++)
//     {
//       //printf("0x%04x ",rxBuf[i]);
//       //continue;
//       //buf[i]=((rxBuf[i*2+start]<<16)+rxBuf[i*2+1+start])>>16;
//       sample_16=(int16_t)txBuf[i*4+start];

//       y_prev=sample_16-x_prev+alpha*y_prev;
//       x_prev=sample_16;


      
//       // if(sample_16>30000 || sample_16) buf[i*2]=0;
//       //printf("%d ",buf[i*2]);
      
//       /*滤除低频信号*/



//       // if((sample_16&0x8000)==0x8000) /*负数*/
//       // {
//       //   tmp=(((int32_t)buf[i*2])|0xffff0000);
//       // }
//       // else tmp=(int32_t)(buf[i*2]);
//       tmp=((int32_t)y_prev);
//       //tmp=sample_16*30;
//       if(tmp>32767)
//       {
//         buf[i*2]=32767;
//       }
//       else if(tmp<-32768)
//       {
//         buf[i*2]=-32768;
//       }
//       else buf[i*2]=tmp;

//       buf[i*2+1]=buf[i*2];
//       //printf("%d ",buf[i]);
//     }

   

//     #if AUDIO_MODE==0
//     ret=I2S_Transmit((uint16_t*)buf,size*2,I2S2_ID);
//     if(ret)
//     {
//       printf("i2s trans fail:%d\n",ret);                                                                                                                                                                                                                                                                                
//     }
//     #endif

//     //I2S_Transmit_DMA((uint16_t*)rxBuf,RX_BUF_SIZE*2,I2S2_ID);

//     #if AUDIO_MODE==1
//     ret=FS_API_Write_By_FP(&fp,(uint8_t*)buf,size*4);
//     if(ret)
//     {
//       printf("write fail:%d\n",ret);
//     }

//     if(HAL_GetTick()-t2>1000)
//     {
//       FS_API_Close(&fp);
//       break;
//     }
//     #endif
//     break;
//     sw=(sw==0)?1:0;
//   }

//   printf("tt2=%d\n",HAL_GetTick());


//   #if AUDIO_MODE==3

//   // ret=Audio_Player_Play_Init("S:audio.pcm");
//   // printf("audio init fail:%d\n",ret);
//   ret=Audio_Player_Play_Init("S:img/jingwei.wav");
  
//     /*播放器*/
//   uint16_t j=0;
//   uint16_t volume=5;
//   Audio_Player_SetVolume(volume);
//   while(1)
//   {
//     ret=Audio_Player_Handler();
//     delay_ms(10);
//     if(j%80==0) 
//     {
//       j=0;
//       printf("1s,ret=%d,vol=%d, %d/%d\n",ret,volume,Audio_Player_GetPlayedDuration(),Audio_Player_GetDuration());
//     }
//     j++;
//   }
//   #endif
}