#include"audio_player.h"
#include"audio_output_api.h"
#include"fs_api.h"
#include"audio_conf.h"

enum Audio_Fmt
{
    AUDIO_FMT_ERR=1,
    AUDIO_FMT_WAV=0xF0,
    AUDIO_FMT_PCM=0xF1,
}; 
#define DEFAULT_VOLUME 2 /*默认音量*/


typedef enum __Audio_Player_Status
{
    AUDIO_STATUS_PLAY=0, /*正在播放*/
    AUDIO_STATUS_PLAY_ERR, /*播放失败*/
    AUDIO_STATUS_PAUSE, /*暂停了*/
    AUDIO_STATUS_NO_START /*都没开始播*/
}Audio_Status;



typedef struct 
{
    uint16_t* buf1; /*播放音频的缓存区*/
    uint16_t* buf2;
    uint16_t* txBuf; /*存放要从文件读取的数据缓存区*/
    uint16_t* rxBuf; /*存放要发送的音频数据缓存区*/
    uint16_t bufSize; /*存放音频数据的缓存区大小*/

    uint8_t isStart:1; /*音乐开始播放，该标志位用于初始化一些操作*/
    uint8_t isOver:1; /*判断音乐是否播放完毕*/

    uint8_t playStatus; /*音乐处于的播放状态*/

    uint32_t dataSize; /*单指音频数据的大小，不是文件总大小*/
    uint32_t playedSize; /*已经播放的音频数据大小*/
    uint32_t sampleRate; /*采样率*/

    uint8_t isPlay:1; /*播放与停止*/

    uint16_t volume; /*音量*/
    uint8_t audioFmt; /*播放的音乐是什么格式，值对应 Audio_Fmt。为0代表没音乐播放*/

    uint16_t duration; /*时长 单位s*/
    uint16_t playedDuration; /*音乐已经播放的时长 单位s*/

}Audio_Info;
static Audio_Info audioInfo={0};


static FIL fp={0}; /*文件指针*/
static uint8_t swBuf=0; /*两个缓存区切换标志位*/ 
static uint32_t fileReadSize=0; /*每次读取音频文件真正读到的数据大小*/
static int16_t* tmp_16=NULL;







/// @brief 初始化音频播放器
/// @param sampleRate 音频放大模块的采样率 //I2S_AUDIOFREQ_xxx 如：I2S_AUDIOFREQ_48K

void Audio_Player_Init(uint32_t sampleRate)
{
    audioInfo.sampleRate=sampleRate;
    audioInfo.buf1=audio_buf1;
    audioInfo.buf2=audio_buf2;
    audioInfo.bufSize=AUDIO_BUF_SIZE;
   
    audioInfo.isStart=1;
    audioInfo.playStatus=AUDIO_STATUS_NO_START;
    audioInfo.volume=DEFAULT_VOLUME; /*默认音量*/
    Audio_Output_Init(sampleRate);
}


/// @brief 音频播放器初始化。初始化成功会自动把播放标志位置于1
/// @param filePath 音频文件绝对路径
/// @return 返回0成功。  0xF0~0xFF代表各种格式的音频文件,其它是失败标志。0xF0是.wav文件，
uint8_t Audio_Player_Play_Init(const char* filePath)
{
    /*读取音频文件*/
    uint8_t ret=0;

    uint8_t pathNum=strlen(filePath); /*路径的总长度*/
    char* musicFmt=filePath+pathNum-4; /*音乐文件格式，如.mp3*/
    printf("%s\n",musicFmt);
    if(strcmp(musicFmt,".wav")==0)
    {
        printf("is .wav\n");
        audioInfo.audioFmt=AUDIO_FMT_WAV;
        audioInfo.isPlay=1;
        audioInfo.playStatus=AUDIO_STATUS_PLAY;
    }
    else if(strcmp(musicFmt,".pcm")==0)
    {
        printf("is .pcm\n");
        audioInfo.audioFmt=AUDIO_FMT_PCM;
        audioInfo.isPlay=1;
        audioInfo.playStatus=AUDIO_STATUS_PLAY;
    }
    else
    {
        printf("fmt err\n");
        audioInfo.isPlay=0;
        return 1;
    } 

    ret=FS_API_Get_FP(filePath,&fp,0);
    if(ret) 
    {
        printf("read file fail:%d\n",ret);
        return ret;
    }

    audioInfo.dataSize=FS_API_GetFileSize(filePath); /*获取文件大小*/
    if(audioInfo.audioFmt==AUDIO_FMT_WAV)
    {   
        audioInfo.duration=audioInfo.dataSize/(audioInfo.sampleRate*4); /*简单计算下，这里的头帧忽略了*/
    }
    else if(audioInfo.audioFmt==AUDIO_FMT_PCM)
    {   
        audioInfo.duration=audioInfo.dataSize/(audioInfo.sampleRate*4); /*简单计算下，这里的头帧忽略了*/
    }
    printf("audio size=%dKB,duration=%d\n",audioInfo.dataSize/1024.0f,audioInfo.duration);
    
    uint16_t sampleData=100;
    ret=Audio_Output_Transmit(&sampleData,1);
    if(ret) 
    {
        PrintErr(ret);
        return AUDIO_STATUS_PLAY_ERR;
    }
    uint8_t t=0;
    while(1)
    {
        if(Audio_Output_WaitTxCplt()==0) break;
        delay_ms(1);
        t++;
        if(t==240) 
        {
            PrintErr(ret);
            return AUDIO_STATUS_PLAY_ERR;
        }
    }
    return 0;
}


/// @brief 音频放大器发送一次数据给喇叭。 先发送音频数据，再从SD卡读取音频数据，所以第一次需要先读好数据。
/// @param fp 文件指针
/// @return 0：成功
static uint8_t Audio_Player_PlayOneTimes(FIL* fp)
{
    uint8_t ret=0;
    if(audioInfo.isStart)
    {
        audioInfo.isStart=0;
        audioInfo.rxBuf=audioInfo.buf1;
        audioInfo.txBuf=audioInfo.buf1;

        if(audioInfo.audioFmt==AUDIO_FMT_WAV)
        {
            f_lseek(fp,0x2C);
            audioInfo.playedSize=0x2C;
        }
        
        ret=FS_API_Read_By_FP(fp,(uint8_t*)audioInfo.rxBuf,audioInfo.bufSize*2,&fileReadSize);
        if(ret)
        {
            printf("read audio file fail:%d\n",ret);
            return ret;
        }
        
        swBuf=1;
    }
    else
    {
        /*等待音频数据传输完成，如果返回0代表传输好了，1代表没有，没有则结束本次*/
        if(Audio_Output_WaitTxCplt())
        {
            return 0; 
        }
    }

    /*爽缓存区切换*/
    if(swBuf==1) 
    {
        audioInfo.rxBuf=audioInfo.buf2;
        audioInfo.txBuf=audioInfo.buf1;
    }
    else
    {
        audioInfo.rxBuf=audioInfo.buf1;
        audioInfo.txBuf=audioInfo.buf2;
    }

    /*传输音频数据*/
    ret=Audio_Output_Transmit((uint16_t*)audioInfo.txBuf,fileReadSize/2);
    if(ret)
    {
        printf("max98357 play fail:%d\n",ret);
        return ret;
    }

    /*播放的音频数据大小增加*/
    audioInfo.playedSize+=fileReadSize;

    /*获取已播放的音频时长*/
    audioInfo.playedDuration=(uint16_t)((float)audioInfo.duration*audioInfo.playedSize/audioInfo.dataSize);

    /*读取音频数据*/
    ret=FS_API_Read_By_FP(fp,(uint8_t*)audioInfo.rxBuf,audioInfo.bufSize*2,&fileReadSize);
    if(ret)
    {
        printf("read audio file fail:%d\n",ret);
        return ret;
    }

    /*软件方式设置音量*/
    if(audioInfo.volume!=100)
    {
        tmp_16 =(int16_t*)audioInfo.rxBuf;
        int32_t tmp=0;
        for(uint16_t j=0;j<audioInfo.bufSize;j++) 
        {
            tmp=(int32_t)tmp_16[j];
            tmp=(tmp/(100.0f/audioInfo.volume)); /*音频放大*/

            /*限幅*/
            if(tmp>32767)
            {
                tmp=32767;
            }
            else if(tmp<-32768)
            {
                tmp=-32768;
            }
            tmp_16[j]=(int16_t)tmp;
        }
    }
    
    swBuf=(swBuf==1)?0:1;

    /*播放完毕*/
    if(audioInfo.playedSize==audioInfo.dataSize)
    {
        audioInfo.isOver=1;
    }

    return 0;
}

/// @brief 获取音频播放的总时长
/// @return 
uint16_t Audio_Player_GetDuration()
{
    return audioInfo.duration;
}

/// @brief 获取音频已经播放的时长
/// @return 
uint16_t Audio_Player_GetPlayedDuration()
{
    return audioInfo.playedDuration;
}

/// @brief 音频播放器处理函数。需要需要些循环调用来实现播放音频
/// @return 返回播放状态 Audio_Status。 0:播放成功(正在播放),1播放了，但失败了，2播放暂停，3没有开始播放
uint8_t Audio_Player_Handler()
{
    //printf("%d\n",volume);
    if(audioInfo.isPlay)
    {
        if(Audio_Player_PlayOneTimes(&fp)) 
        {
            audioInfo.isOver=1;
            audioInfo.isPlay=0;
            return AUDIO_STATUS_PLAY_ERR;
        }
    }

    if(audioInfo.isOver) /*播放结束，关闭文件，清除各种标志位*/
    {
        FS_API_Close(&fp);
        audioInfo.isOver=0;
        audioInfo.isPlay=0;
        audioInfo.isStart=1;
        audioInfo.playStatus=AUDIO_STATUS_NO_START;
        printf("end\n");
    }

    return audioInfo.playStatus;
}


void Audio_Player_Play()
{
    audioInfo.isPlay=1;
}

void Audio_Player_Pause()
{
    audioInfo.isPlay=0;
}

void Audio_Player_SetVolume(uint16_t volume)
{
    audioInfo.volume=volume;
}

/*播放音乐结束，一般用于换一首歌播放*/
void Audio_Player_PlayOver()
{
    if(audioInfo.playStatus==AUDIO_STATUS_NO_START) return; /*代表内部自动结束了播放，不是手动的*/
    audioInfo.isOver=1;
    Audio_Player_Handler();
}



