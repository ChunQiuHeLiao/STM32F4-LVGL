#ifndef __MUSIC_H
#define __MUSIC_H

#ifdef __cplusplus
extern "C" {
#endif
#include"lvgl.h"
typedef enum
{
    CONN_STATUS_DISCONN = 0, /*未连接*/
    CONN_STATUS_CONNECTING,
    CONN_STATUS_SUC,
    CONN_STATUS_CONN_TIMEOUT,
    CONN_STATUS_CONN_EEROR,

}Conn_Status; /*网络/蓝牙等连接状态*/

typedef enum
{
    MUSIC_STATUS_NO_START = 0,
    MUSIC_STATUS_ERR=1,
    MUSIC_STATUS_READY,
    MUSIC_STATUS_PLAYING,
    MUSIC_STATUS_PAUSE,
    MUSIC_STATUS_OVER,
    MUSIC_STATUS_OVER_TO_READY  /*结束后立刻开始播放，一般用于切换音乐，即关闭当前播放的，然后开始播放新的*/
}Music_Status;

typedef struct
{
    uint8_t isInit : 2; /* 初始化时调用，仅调用一次，初始化完后置为2 代表开始*/
    uint8_t isDeInit : 1; /* 该APP结束运行后调用，仅调用一次，初始化完后置为0 代表结束*/

    uint8_t isPrev : 1;
    uint8_t isPlay : 1;
    uint8_t isNext : 1;
    uint8_t isClickedPlay : 1; /*点了了播放按钮*/
    uint8_t isClickedPlayMode : 1; /*点击了该按钮*/
    Music_Status playStatus:4; /*播放状态*/
    char songPath[48]; /*播放的歌曲的路径*/
    char playName[32]; /*播放的歌曲的名字*/

    uint8_t playMode : 2; /*0:整体循环，1:随机播放，2:单曲循环*/

    uint16_t num; /*音乐总数*/
    uint8_t index; /*音乐索引*/
    //uint32_t sampleRate; /*采样率*/
    //uint16_t duration; /*歌曲总时长*/
    //uint16_t playedDuration; /*已播放的歌曲总时长*/
    
    uint8_t vol1;
    uint8_t vol2;
    bool isModifyVol; /*是否修改音量*/
    bool isDelete; /*是否删除歌曲*/
    char deleteName[32]; /*删除歌曲的名字*/
    uint8_t isAccessFile : 1; /*是否在访问文件，目的是 访问文件和删除文件分开*/

    /*用于wifi下载歌曲部分*/
    bool isClickedConn;   /*是否点击连接按钮*/
    uint8_t isConnServer;  /*是否连接服务器*/
    Conn_Status connStatus; /*服务器连接状态*/
    uint8_t downloadProcess; /*下载进度0~100*/

}Music_Handle;

extern Music_Handle musicHandle;


#ifdef __cplusplus
}
#endif
#endif
