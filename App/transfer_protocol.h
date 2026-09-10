#ifndef __TRANSFER_PROTOCOL_H
#define __TRANSFER_PROTOCOL_H
#include"main.h"



/*--------------用于无CRC校验的通信协议(UART，I2C)这是新的应用层协议-----------------*/

#define DATA_FRAME_MAGIC 0x61696763 //魔术头 "aigc"
#define DATA_PACKET_MAX_SIZE 512 //每包的数据最大大小


/// @brief 帧错误码
typedef enum
{
    ERR_CODE_OK=0,
    ERR_CODE_SYNC_ERR,
    ERR_CODE_HEADER_CRC_ERR,
    ERR_CODE_BODY_CRC_ERR,
    ERR_CODE_TIMEOUT,

    ERR_CODE_WRITE_FLASH_ERR, //把数据写入flash失败
    ERR_CODE_FIRMWARE_CRC_ERR, //固件校验失败，说明写入FLASH时有问题
    ERR_CODE_FIRMWARE_SIZE_NO_MATCH, //最终接收的数据大小和固件实际大小不同，一般是少了
    ERR_CODE_UNKONW, //主要是其它的错误，什么内存申请失败呀

    ERR_CODE_MAX
}Frame_ErrCode;


/// @brief 功能码，主要用于判断本次数据用于干什么，升级的，还是传输温度，红外等数据
typedef enum
{
    //升级功能码
    FUNC_CODE_RESPONSE=0, //响应
    FUNC_CODE_UPDATE_START, //主要传送些本次传输的固件大小
    FUNC_CODE_UPDATING, //传输数据
    FUNC_CODE_UPDAT_FINISHED, //传输整个固件的CRC32校验值

    FUNC_CODE_MAX
}FuncCode;


/// @brief 帧头 16字节
typedef struct
{
    uint32_t magic; //魔术头 "aigc" 
    uint16_t sync; //同步序列号
    uint16_t len; //本次传输的数据大小。指 FrameBody_t::data
    uint8_t errCode; //错误码。一般都是接收端上报给发送端的 看 @ref Frame_ErrCode 错误码不是OK，则帧体没有
    uint8_t reverse1; //保留位1 默认0x00
    uint8_t reverse2; //保留位2 默认0x00
    uint8_t reverse3; //保留位3 默认0x00
    uint16_t bodyCrc; //帧体数据CRC16
    uint16_t headCrc; //帧头数据crc16
}FrameHead_t;


/// @brief 帧体
typedef struct
{
    uint8_t funcCode; //功能码 @ref FuncCode
    uint8_t data[DATA_PACKET_MAX_SIZE]; //实际的数据大小
}FrameBody_t;


/// @brief 每包的数据帧
typedef struct
{
    FrameHead_t head;
    FrameBody_t body;
}DataFrame_t;



//功能码和对应回调函数结构体
typedef struct
{
    uint8_t funcCode;
    void (*data_frame_cb)(DataFrame_t* df);
}FuncCodeHandler_t;


int DataFrame_Handler(RingBuffer_t* rb,DataFrame_t* df);
int DataFrame_CallBack(DataFrame_t* df,FuncCodeHandler_t* mapTable,uint16_t size);
int DataFrame_Response(uint8_t errCode);
#endif