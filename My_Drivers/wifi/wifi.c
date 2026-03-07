#include"wifi.h"




/*****************下面四个个函数替换成自己的接口*/

//wifi 发送数据。默认发送前延迟500ms
uint8_t WIFI_SendData(char* data,uint16_t size)
{
    lv_delay_ms(500);
    return UART_Transmit_String(data,size,WIFI_USE_UART_ID);
}

/// @brief 接收WIFI模块发来的数据
/// @return 0：有数据了，1无
uint8_t WIFI_RevData(char* rxData,uint16_t size)
{
    return UART_GetRxData(rxData,size,WIFI_USE_UART_ID);
}

/// @brief 接收WIFI模块发来的数据的指针
/// @return 0：有数据了，1无
uint8_t WIFI_RevDataPointer(char** rxData)
{
    return UART_GetRxDataPointer(rxData,WIFI_USE_UART_ID);
}


/// @brief 获取本次接收数据的大小
uint16_t WIFI_RevDataSize()
{
    return UART_GetRxDataSize(WIFI_USE_UART_ID);
}





/// @brief 发送命令(参数简略版)。判断是否响应，也可以得到响应的数据
/// @param cmd 命令
/// @param resPart 响应字符串的部分，主要用于判断是否响应成功，如 响应部分有 OK 则成功， ERROR 则失败  
/// @param timeout 超时时长
/// @return 0成功,1错误，2超时
uint8_t WIFI_SendCmd(uint8_t* cmd,uint8_t* resPart,uint16_t timeout)
{
   return WIFI_SendCmd_Detail(cmd,resPart,NULL,0,timeout);
}



/// @brief 发送命令(参数详细版)。判断是否响应，也可以得到响应的数据
/// @param cmd 命令
/// @param resPart 响应字符串的部分(响应子串)，主要用于判断是否响应成功，如 响应部分有 OK 则成功， ERROR 则失败  
/// @param response 响应字符串。如果不需要可以填NULL 
/// @param size 响应字符串大小
/// @param timeout 超时时长
/// @return 0成功,1：响应字符串中无响应子串，2超时。一般只有2超时或0
uint8_t WIFI_SendCmd_Detail(uint8_t* cmd,uint8_t* resPart,char* response,uint16_t size,uint16_t timeout)
{
    printf("timeout:%d\n",timeout);
    timeout=timeout/10;
    uint8_t cmdSize=0;
    while (1)
    {
        if(cmd[cmdSize++]=='\n')break;
    }

    // printf("121\n");
    WIFI_SendData(cmd,cmdSize);
    //printf("122\n");

    char rxDataTmp[128]={0};
    char* rxData=NULL; /*接收数据指针*/
    if(response==NULL) 
    {
        rxData=rxDataTmp; /*申请128字节数据*/
        size=128;
    }
    else rxData=response; /*则需要响应字符串*/

    // printf("123\n");
    while (WIFI_RevData(rxData,size-1)==1)
    {
        #if WIFI_USE_LVGL==1
        lv_delay_ms(10);
        #else
        delay_ms(10); 
        #endif
        // printf("124\n");
        timeout--;
        if(timeout==0) 
        {
            printf("wifi cmd:%s send timeout\n",cmd);
            return 2;
        }
    }
    // printf("125\n");
    //判断响应字符串是否在接收的数据当中。这两个字符串一定要有'\0'
    char* ret=strstr(rxData,resPart);
    if(ret==NULL)
    {
        printf("wifi cmd:%s response fail:%s",cmd,rxData);
        return 1;
    }
    
    printf("cmd:%s ok\n",cmd);
    
    //if(response!=NULL) printf("response:%s\n",response);

    // printf("126\n");
    return 0;
}










