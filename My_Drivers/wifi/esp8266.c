#include"esp8266.h"
#include"string.h"

/*ESP8266C初始化，默认发送数据就设置为透传模式*/
void ESP8266_Init(uint32_t baudRate)
{
    ESP8266_GPIO_EN_CLK_ENABLE();
    ESP8266_GPIO_RST_CLK_ENABLE();

    GPIO_InitTypeDef init={0};
    init.Mode=GPIO_MODE_OUTPUT_PP;
    init.Pull=GPIO_PULLUP;
    init.Speed=GPIO_SPEED_FREQ_HIGH;
    init.Pin=ESP8266_GPIO_EN_PIN;
    HAL_GPIO_Init(ESP8266_GPIO_EN_PORT,&init);

    init.Pin=ESP8266_GPIO_RST_PIN;
    HAL_GPIO_Init(ESP8266_GPIO_RST_PORT,&init);

    //软件复位
    HAL_GPIO_WritePin(ESP8266_GPIO_RST_PORT,ESP8266_GPIO_RST_PIN,0);

    delay_ms(100);
    HAL_GPIO_WritePin(ESP8266_GPIO_RST_PORT,ESP8266_GPIO_RST_PIN,1);
    HAL_GPIO_WritePin(ESP8266_GPIO_EN_PORT,ESP8266_GPIO_EN_PIN,1);


    UART2_Init(baudRate); /*串口初始化*/
    delay_ms(1000);

    /*先接收esp8266发来的初始化数据，必须接*/
    uint8_t rxData[16]={0};
    uint16_t timeout=2000;
    while (WIFI_RevData(rxData,sizeof(rxData))==1) 
    {
        delay_ms(1);
        timeout--;
        if(timeout==0) break;
    }

    memset(rxData,0,sizeof(rxData));
    //没有关回显时这里会发送两个\r\r
    delay_ms(100);


    WIFI_SendCmd("ATE0\r\n","OK",4000);
    ESP8266_Cmd_Set_CIPMODE(1);
}




/// @brief 设置传输模式
/// @param mode 1为透传模式，0 普通模式(每次发送需要说下有多少数据)
/// @return 0:成功，1失败
uint8_t ESP8266_Cmd_Set_CIPMODE(uint8_t mode)
{
    uint8_t ret=0;
    if(mode==1) ret=WIFI_SendCmd("AT+CIPMODE=1\r\n","OK",ESP8266_BASE_TIMEOUT_TIME);
    else if(mode==0) ret=WIFI_SendCmd("AT+CIPMODE=0\r\n","OK",ESP8266_BASE_TIMEOUT_TIME);
    return ret;
}

/// @brief 连接服务器命令
/// @param protol 协议
/// @return 0成功
uint8_t ESP8266_Cmd_ConnectServer(const uint8_t* protol,const uint8_t* ip,const uint16_t port)
{
    uint8_t WIFI_sendCmd[64]={0};
    //1.发送连接服务器命令
    sprintf(WIFI_sendCmd,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",protol,ip,port);
    return WIFI_SendCmd(WIFI_sendCmd,"OK",ESP8266_BASE_TIMEOUT_TIME);
}


/// @brief 断开服务器命令
/// @return 0成功
uint8_t ESP8266_Cmd_DisConnectServer()
{
    uint8_t WIFI_sendCmd[64]={0};
    
    //1.发送连接服务器命令
    sprintf(WIFI_sendCmd,"AT+CIPCLOSE\r\n");
    return WIFI_SendCmd(WIFI_sendCmd,"OK",ESP8266_BASE_TIMEOUT_TIME);
}



//使能发送数据。需要在连接服务器后才可以使能
uint8_t ESP8266_Cmd_EN_SendData()
{
    return WIFI_SendCmd("AT+CIPSEND\r\n","OK",ESP8266_BASE_TIMEOUT_TIME);
}

//退出发送数据。需要在连接服务器后才可以退出
uint8_t ESP8266_Cmd_EXIT_SendData()
{
    return WIFI_SendData("+++",3); /*发送数据，\0也不要有*/
}  


/*****扫描WIFI 和连接WIFI****/

/*扫描周围WIFI,得到所以。这里得到的数组大点，推荐 320字节*/
uint8_t ESP8266_Cmd_ScanAP(uint8_t* response,uint16_t size)
{
    uint8_t ret=WIFI_SendCmd_Detail("AT+CWLAP\r\n","OK",response,size,ESP8266_BASE_TIMEOUT_TIME);
    if(ret) return ret;
}


/// @brief 获取wifi名，每次返回一个wifi名
/// @param response 需要把ESP8266_Cmd_ScanAP 得到的所有wifi信息填入。后面每次都经返回得到的 
/// @param wifiName 得到的wifi名。最大31字符，中文2字符
/// @return 返回数组指针，可直接将返回值赋值给response 参数。 返回NULL代表无WIFI名了。
char* ESP8266_GetWifiName(char* response,char* wifiName,uint8_t size)
{
    memset(wifiName, 0, size);
    response = strstr(response, "+CWLAP"); /*得到出现子串的指针*/
    response = strstr(response, "\""); /*得到wifi名称的指针*/
    sscanf(response, "\"%31[^\"]\"", wifiName);

    return response;
}

/// @brief 连接WIFI
/// @return 0 连接成功，1连接失败(密码错误) 2:连接超时(连接错误)
uint8_t ESP8266_Cmd_ConnectAP(uint8_t* wifiName,uint8_t* wifiPwd)
{
    uint8_t buf[64]={0};
    sprintf(buf,"AT+CWJAP=\"%s\",\"%s\"\r\n",wifiName,wifiPwd);
    uint8_t ret=WIFI_SendCmd(buf,"OK",(ESP8266_BASE_TIMEOUT_TIME*2));
    if(ret) return ret;

    return 0;
}


/// @brief 查询连接的WIFI
/// @param conWifiName 连接的wifi名,如果为NULL则代表为无连接wifi。
/// @return 0查询成功,1:无连接的WIFI，2:超时 
uint8_t ESP8266_SelectConnAP(char* conWifiName)
{  
    char response[64]={0}; /*这是常量指针，指针不可修改*/
    char* p=response;
    uint8_t ret=WIFI_SendCmd_Detail("AT+CWJAP?\r\n","OK",response,sizeof(response),ESP8266_BASE_TIMEOUT_TIME);
    if(ret)
    {
        printf("select AP fail:%d\n",ret);
        return 2;
    }

    p=strstr(response,"+CWJAP"); 
    if(p==NULL) return 1;
    p=strstr(p,"\"");

    sscanf(p,"\"%31[^\"]\"",conWifiName);
    
    return 0;
}





/// @brief 判定ESP8266是否处于忙状态
/// @return 0：空闲，1忙
void ESP8266_Delay()
{
    lv_delay_ms(500);
}