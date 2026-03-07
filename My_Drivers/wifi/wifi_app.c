#include"wifi_app.h"
#include"stm_flash.h"
#include"fs_api.h"

uint8_t* wifi_buf=NULL; /*WIFI的指针*/



uint8_t WIFI_APP_Init(uint32_t baudrate)
{
    ESP8266_Init(baudrate);
}



/*WIFI信号处理函数*/
void WIFI_APP_Handler()
{
    // static uint32_t t1=HAL_GetTick();
    // static uint32_t t2=0;


    /*处理事件*/
    char rxData[512]={0};
    uint8_t ret=WIFI_APP_ScanAP(rxData,sizeof(rxData));
    if(ret) printf("scan ap fail:%d\n",ret);

    printf("%s\n",rxData);

    char wifiName[32]={0};
    char* p=rxData;
    while((p=WIFI_APP_GetWifiName(p,wifiName,sizeof(wifiName)))!=NULL)
    {
        printf("wifi name:%s\n",wifiName);
    }

    ret=WIFI_APP_ConnectAP("vivo Y53t","12345678");
    if(ret==1) printf("pwd error\n");
    else if(ret==2) printf("connect timeout\n");


    ret=WIFI_APP_GetConnWifiName(wifiName);
    if(ret) printf("select ap fail\n");
    printf("connect wifi name:%s\n",wifiName);



    /*周期延迟*/
    // t2=HAL_GetTick();
    // int8_t delayTime=t2-t1; 
    // if(delayTime<0) return ;
    // delay_ms(delayTime);
}









/// @brief 向服务器发送 0x10 cu 0x20 数据来检查。服务器返回 0x20 u 0/1 0x10  第三个字节是判断 是否需要更新
/// @param updateFlag 用于获取是否需要更新的信息
/// @return 0 成功，
uint8_t WIFI_APP_CheckUpdateFlag_FromServer(uint8_t* updateFlag)
{
    uint8_t ret=0;
    uint8_t sendData[4]={0x10,'c','u',0x20}; //check update
    WIFI_SendData(sendData,sizeof(sendData));

    uint8_t rxData[16]={0};
    uint16_t timeout=1000; /*延迟10s*/
    while (WIFI_RevData(rxData,sizeof(rxData))==1)
    {
        #if WIFI_USE_LVGL==1 
        lv_delay_ms(10);
        #else
        delay_ms(10);
        #endif

        timeout--;
        if(timeout==0) 
        {
            printf("server response timeout\n");
            return 2;
        }
    }

    uint8_t size=WIFI_RevDataSize(); /*获取服务器发来的数据*/
    //printf("%s\n",rxData);

    if(rxData[size-4]==0x20&&rxData[size-3]=='u'&&rxData[size-1]==0x10)
    {
        printf("rev ok,update flag=%d",rxData[size-2]);
        if(rxData[size-2]==1) 
        {
            *updateFlag=1;
            STM_FLASH_Set_UpdateFlag(1);
        }
        else *updateFlag=0;
        return 0;
    }

    printf("check update flag fail\n");
    
    return 1;
}


/// @brief 获取更新安装包的大小
/// @param allByte 总字节数
/// @return 0：获取成功 2:响应超时
uint8_t WIFI_APP_GetPackageSize(uint32_t* allByte)
{
    uint8_t sendData[4]={0x10,'u','s',0x20}; //update start  开始更新的指令
    WIFI_SendData(sendData,4); //发送开始更新系统安装包指令

    uint8_t rxData[8]={0};
    uint16_t timeout=1000; //10s
    while (WIFI_RevData(rxData,6)==1)
    {
        #if WIFI_USE_LVGL==1
        lv_delay_ms(10);
        #else
        delay_ms(10);
        #endif

        timeout--;
        if(timeout==0) return 2;
    }

    // uint8_t size=WIFI_RevDataSize();
    *allByte=0; //本次安装包的总字节大小
    if(rxData[0]==0x20&&rxData[5]==0x10)
    {
        *allByte=((uint32_t)rxData[4]<<24)+((uint32_t)rxData[3]<<16)+((uint32_t)rxData[2]<<8)+rxData[1];
        printf("allbyte=%dB\n",*allByte);
    }

    return 0;
}


/// @brief 从服务器下载应用(分为系统安装包和应用安装包)。下载到SD卡里面。
/// @filePath 下载的SD卡路径。需要有这个目录。
/// @return 0:下载成功，2 接收数据超时,1获取文件指针失败,3写入SD卡错误
uint8_t WIFI_APP_Download_Pack(const char *filePath,uint32_t allByte,uint32_t* revAllByte)
{
    uint8_t sendData[5]={0};
    //can download
    sendData[0]=0x10;
    sendData[1]='c'; 
    sendData[2]='d';
    sendData[3]=0x20;
    WIFI_SendData(sendData,4); //发送可以把安装包数据发来了指令


    //获取文件指针并下载
    FIL fp;
    uint8_t ret=0;
    ret=FS_API_Get_FP(filePath,&fp,1);
    if(ret!=0) 
    {
        printf("get fp fail=%d\n",ret);
        return 1;
    }


   
    //开始下载
    //continuous 
    sendData[1]='c';
    sendData[2]='t';
    uint8_t rxData[512]={0}; /*每次接收的数据量*/
    // uint32_t revAllByte=0;

    WIFI_SendData(sendData,4); /*给服务器 发送开始发送数据的指令*/

    uint16_t timeout=10000;
    uint32_t startTime=HAL_GetTick();
    while(1)
    {
        timeout=1000; 
        while (WIFI_RevData(rxData,sizeof(rxData))==1)
        {
            #if WIFI_USE_LVGL==1 
            lv_delay_ms(10); 
            #else
            delay_ms(10);
            #endif

            timeout--;
            if(timeout==0) 
            {
                FS_API_Close(&fp);
                printf("byte=%dB\n",*revAllByte);
                return 2;
            }
            else if(timeout%200==0) WIFI_SendData(sendData,4); //2s发送一次。发送给服务器 继续发送指令
        }
        //printf("%s",rxData);
        

        //写入SD卡 4ms
        ret=FS_API_Write_By_FP(&fp,rxData,WIFI_RevDataSize());
        if(ret!=0) 
        {
            printf("sd write fail=%d\n",ret);
            return 3;
        }


        *revAllByte+=WIFI_RevDataSize();
        
        Printf("rev byte:%fKB,%f%%\n",(*revAllByte)/1000.0f,(*revAllByte)/(float)(allByte)*100);
        if(allByte==(*revAllByte))break; //接收数据完毕

        //1ms
        #if WIFI_USE_LVGL==1 
        lv_delay_ms(10); 
        #else
        delay_ms(10);
        #endif

        WIFI_SendData(sendData,4); //发送给服务器 继续发送指令
        memset(rxData,0,WIFI_RevDataSize());
    }

    printf("rev over, %dB\n",*revAllByte);
    FS_API_Close(&fp);

    Printf("download all time:%fs\n",(HAL_GetTick()-startTime)/1000.0f);
    return 0;
}





/**********这部分可以用对应的接口***********/
/*一些常用的指令，如果没有，自行使用 WIFI_SendCmd会 WIFI_SendCmd_Detail 来自行编写**/

/*一般为1*/
uint8_t WIFI_APP_Set_CIPMODE(uint8_t mode)
{
    return ESP8266_Cmd_Set_CIPMODE(mode);
}


/*************WIFI 部分*/

uint8_t WIFI_APP_ScanAP(char* response,uint16_t size)
{
    return ESP8266_Cmd_ScanAP(response,size);
}


char* WIFI_APP_GetWifiName(char* response,char* wifiName,uint8_t size)
{
   return ESP8266_GetWifiName(response,wifiName,size);
}
uint8_t WIFI_APP_ConnectAP(const char* wifiName,const char* wifiPwd)
{
    return ESP8266_Cmd_ConnectAP(wifiName,wifiPwd);
}

/*获取当前连接的WIFI名*/
uint8_t WIFI_APP_GetConnWifiName(char* conWifiName)
{
    return ESP8266_SelectConnAP(conWifiName);
}


uint8_t WIFI_APP_ConnectServer(const char* protol,const char* ip,const uint16_t port)
{
    return ESP8266_Cmd_ConnectServer(protol,ip,port);
}

uint8_t WIFI_APP_DiconnectServer()
{
    lv_delay_ms(500);
    return ESP8266_Cmd_DisConnectServer();
}


uint8_t WIFI_APP_EN_SendData()
{
    return ESP8266_Cmd_EN_SendData();
}

uint8_t WIFI_APP_EXIT_SendData()
{
    lv_delay_ms(500);
    uint8_t ret=ESP8266_Cmd_EXIT_SendData();
    return ret;
}


void WIFI_APP_Delay()
{
    ESP8266_Delay();
}




/*http get*/

/// @brief 获取http请求响应
/// @param host 主机
/// @param path 主机路径
/// @param response 响应字符串
/// @param size 大小
/// @return 0：成功，1失败
uint8_t WIFI_APP_GetAPIResponse(const char* host, const char* path, char** response)
{
    uint8_t ret=WIFI_APP_ConnectServer("TCP",host,80);
    if(ret) 
    {
        printf("connect %s fail:%d\n",host,ret);
        return 1;
    }
    printf("connect %s suc\n",host);

    WIFI_APP_EN_SendData(); /*使能发送数据*/

    char sendData[256]={0};
    sprintf(sendData,"GET %s HTTP/1.1\r\n"
                    "Host: %s\r\n"
                    "Connection: close\r\n"
                    "\r\n",path,host);  // 头部结束必须是单独的\r\n

    // printf("get: %s/%s\n",host,path);
    ret=WIFI_SendData(sendData,strlen(sendData)); /*发送http 请求*/
    if (ret)
    {
        printf("api: %s/%s get fail:%d\n",host,path,ret);
        return ret;
    }

    printf("api: %s/%s get suc\n",host,path);

    uint8_t timeout=20;
    while(WIFI_RevDataPointer(response)==1)
    {
        lv_delay_ms(1000);
        printf("wait %s send data...\n",host);
        timeout--;
        if(timeout==0) 
        {
            return 2;
        }
    }

    return 0;
}