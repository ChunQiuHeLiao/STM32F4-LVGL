#include"transfer_protocol.h"
#include"usart.h"




/*--------------用于无CRC校验的通信协议(UART，I2C)这是新的应用层协议-----------------*/

/// @brief 发送数据的 基函数，这个要自己改成自己传输的
/// @param data 
/// @param size 
/// @return 
static int DataFrame_Send_Base(uint8_t* data,uint16_t size)
{   
    return UART_Transmit_String(data,size,1);
}



/// @brief 处理缓存区数据，并进行函数 一般都是和缓存缓存区搭配的
/// @param rb 环形缓存区
/// @param df 数据帧
/// @return 0处理成功，其它失败
int DataFrame_Handler(RingBuffer_t* rb,DataFrame_t* df)
{
    //缓存区数据大小需要大于头帧结构体大小
    if(rb->cnt<sizeof(FrameHead_t))  
    {
        //DEBUG_E2(-1,"ring buffer len less head frame size\r\n");
        return RET_ERR;
    }


    //1.先1字节数据，判断魔术头 "aigc" 低位开始
    uint8_t rxByte=0;

    //这样解决粘包问题
    while(1)
    {
        if(rb->cnt < sizeof(FrameHead_t)) 
        {
            // DEBUG_E2(-1,"magic err\r\n");
            return RET_ERR;
        }

        RingBuffer_Read(rb,&rxByte,1);
        if(rxByte!='c') continue;

        RingBuffer_Read(rb,&rxByte,1);
        if(rxByte!='g') continue;

        RingBuffer_Read(rb,&rxByte,1);
        if(rxByte!='i') continue;

        RingBuffer_Read(rb,&rxByte,1);
        if(rxByte!='a') continue;

        break; //魔术头正确
    }


    //取剩余的头帧数据
    FrameHead_t* head=&df->head;
    head->magic=DATA_FRAME_MAGIC;

    //注意点W：四字节移动 (uint8_t*)(head+4) 这样错的，你这也是这个结构体移动四个，很明显还越界了，正确是下面的，转为u8* 后+4 
    RingBuffer_Read(rb,((uint8_t*)(head)+4),sizeof(FrameHead_t)-4);


    //2.校验头帧数据CRC
    uint16_t crc16=CRC16_Calculate((uint8_t*)head,sizeof(FrameHead_t)-2);
    if(crc16!=head->headCrc)
    {
        DEBUG_E2(-1,"head crc err,rx:%04x,cal:%04x\r\n",head->headCrc,crc16);
        DataFrame_Response(ERR_CODE_HEADER_CRC_ERR);
        return RET_ERR;
    }

    //printf("data len:%d\r\n",df->head.len);

    //4.读取帧体数据
    uint16_t bodyFrameSize=df->head.len+sizeof(df->body.funcCode); //帧体大小
    uint16_t readSize=0; //已经读取的帧体的实际数据
    uint8_t timeout=6; //最多等3s
    while(1)
    {
        /*注意: 必须读到已读数据之后的位置, 否则分包到达时后面的数据会覆盖前面的*/
        readSize+=(RingBuffer_Read(rb,((uint8_t*)&(df->body))+readSize,bodyFrameSize-readSize));
        if(readSize==bodyFrameSize)
        {
            break;
        }

        vTaskDelay(500);
        timeout--;
        if(timeout==0)
        {
            DEBUG_E2(-1,"body data read err,rxSize:%d,len:%d\r\n",readSize,bodyFrameSize);
            DataFrame_Response(ERR_CODE_TIMEOUT);
            return RET_ERR;
        }
    }

    //5.校验帧体
    crc16=CRC16_Calculate((uint8_t*)&df->body,bodyFrameSize);
    if(crc16!=df->head.bodyCrc)
    {
        DEBUG_E2(-1,"body crc err,rx:%04x,cal:%04x\r\n",df->head.bodyCrc,crc16);
        DataFrame_Response(ERR_CODE_BODY_CRC_ERR);
        return RET_ERR;
    }


    //6.通过功能码执行对应的函数

    // DEBUG_I("data frame deal OK\r\n");
    //DEBUG_I("sync:%d,errcode:%d,func:%d\r\n",df->head.sync,df->head.errCode,df->body.funcCode);
    return RET_OK;
}


/// @brief 数据帧的回调函数，也可以用自己的方式，如switch case这种
/// @param df 
/// @param mapTable 
/// @param size 
/// @return 
int DataFrame_CallBack(DataFrame_t* df,FuncCodeHandler_t* mapTable,uint16_t size)
{
    for(uint16_t i=0;i<size;i++)
    {
        //DEBUG_I("sync:%d,errcode:%d,func:%d\r\n",df->head.sync,df->head.errCode,df->body.funcCode);
        if(df->body.funcCode==mapTable[i].funcCode)
        {
            mapTable[i].data_frame_cb(df);
            return 0;
        }
    }

    return RET_ERR;
}



/// @brief 发送数据给发送方
/// @param df 
/// @param errCode 
/// @return 
int DataFrame_Response(uint8_t errCode)
{
    DataFrame_t df={0};
    df.head.magic=DATA_FRAME_MAGIC;
    df.head.errCode=errCode;
    df.head.len=0;
    df.head.bodyCrc=0xe1f0;
    df.head.headCrc=CRC16_Calculate((uint8_t*)&df.head,sizeof(FrameHead_t)-2);
    df.body.funcCode=FUNC_CODE_RESPONSE;

    int ret=DataFrame_Send_Base((uint8_t*)&df,sizeof(FrameHead_t)+1);
    return ret;
}   