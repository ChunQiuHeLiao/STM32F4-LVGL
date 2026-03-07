#include"Core/weather.h"

Weather_Handle weatherHandle = { 0 };



static char* Weather_GetSingleInfo(char* str, char* key, char* value);

/*获取天气句柄指针*/
Weather_Handle* Weather_GetHandlePointer()
{
    return &weatherHandle;
}


uint8_t Weather_UpdateInfo_RealTime(const char* city)
{
    uint8_t ret=0;
    char host[18] = "api.seniverse.com";
    char realTimePath[128] = { 0 };

    sprintf(realTimePath, "/v3/weather/now.json?key=SPLb5Js-pixOr2KUE&location=%s&language=en&unit=c", city);

    char* response=NULL;
    ret=WIFI_APP_GetAPIResponse(host, realTimePath, &response);
    if(ret) 
    {
        WIFI_APP_DiconnectServer();
        WIFI_APP_EXIT_SendData(); /*关闭服务器并发送+++ 退出透传模式*/
        return ret;
    }

    printf("size=%d\n%s\n",WIFI_RevDataSize(),response); /*打印响应内容*/
    // rxData[]={"GET /v3/weather/now.json?key=SPLb5Js-pixOr2KUE&location=HangZhou&language=en&unit=c HTTP/1.1\r\n
    //           Host: api.seniverse.com\r\n
    //           Connection: close\r\n"}
   // AT+CIPSTART="TCP","api.seniverse.com",80

    /*1.先判断是否有返回结果*/
    char* p = NULL; /*临时指针*/
    p = strstr(response, "results");
    if (p == NULL)
    {
        printf("get weather info fail\n");
        WIFI_APP_DiconnectServer();
        WIFI_APP_EXIT_SendData(); /*关闭服务器并发送+++ 退出透传模式*/
        return 1;
    }
    //printf("%s\n", p); 



    char value[16] = { 0 };
    Weather_Info_RealTime* realTimeInfo = &weatherHandle.realTimeInfo;

    p = Weather_GetSingleInfo(p, "text", value);
    MyStrCpy(&realTimeInfo->text, strlen(value), value);

    p = Weather_GetSingleInfo(p, "temperature", value);
    MyStrCpy(&realTimeInfo->temp, strlen(value), value);

    // printf("%s\n", p);
    printf("text=%s,temp=%s\n", weatherHandle.realTimeInfo.text, weatherHandle.realTimeInfo.temp);

    WIFI_APP_DiconnectServer();
    WIFI_APP_EXIT_SendData(); /*关闭服务器并发送+++ 退出透传模式*/
    return 0;
}



/*更新3日的*/
uint8_t Weather_UpdateInfo(const char* city)
{
    char host[18] = "api.seniverse.com";
    char realTimePath[128] = { 0 };

    sprintf(realTimePath, "/v3/weather/daily.json?key=SPLb5Js-pixOr2KUE&location=%s&language=en&unit=c&start=0&days=3", city);

    char* response=NULL;
    uint8_t ret=WIFI_APP_GetAPIResponse(host, realTimePath, &response);
    if(ret) 
    {
        WIFI_APP_DiconnectServer();
        WIFI_APP_EXIT_SendData(); /*关闭服务器并发送+++ 退出透传模式*/
        return ret;
    }
    printf("size=%d\n%s\n",WIFI_RevDataSize(),response); /*打印响应内容*/


    /*1.先判断是否有返回结果*/
    char* p = NULL; /*临时指针*/
    p = strstr(response, "results");
    if (p == NULL)
    {
        printf("get weather info fail\n");
        WIFI_APP_DiconnectServer();
        WIFI_APP_EXIT_SendData(); /*关闭服务器并发送+++ 退出透传模式*/
        return 1;
    }
    /*printf("%s\n", p); */


    char value[16] = { 0 };
    Weather_Info_Day* info = NULL;
    for (uint8_t i = 0; i < sizeof(weatherHandle.daysInfo) / (sizeof(Weather_Info_Day)); i++)
    {
        info = &weatherHandle.daysInfo[i];

        p = Weather_GetSingleInfo(p, "text_day", value);
        MyStrCpy(&info->text, strlen(value), value);

        p = Weather_GetSingleInfo(p, "high", value);
        MyStrCpy(&info->highTemp, strlen(value), value);

        p = Weather_GetSingleInfo(p, "low", value);
        MyStrCpy(&info->lowTemp, strlen(value), value);

        p = Weather_GetSingleInfo(p, "rainfall", value);
        MyStrCpy(&info->rainfall, strlen(value), value);

        p = Weather_GetSingleInfo(p, "precip", value);
        MyStrCpy(&info->precip, strlen(value), value);

        p = Weather_GetSingleInfo(p, "wind_direction", value);
        MyStrCpy(&info->windDir, strlen(value), value);

        p = Weather_GetSingleInfo(p, "wind_speed", value);
        MyStrCpy(&info->windSpeed, strlen(value), value);

        p = Weather_GetSingleInfo(p, "wind_scale", value);
        MyStrCpy(&info->windScale, strlen(value), value);

        p = Weather_GetSingleInfo(p, "humidity", value);
        MyStrCpy(&info->humidity, strlen(value), value);

        /*printf("%s\n", p);*/
    }

    Weather_PrintInfo();

    WIFI_APP_DiconnectServer();
    WIFI_APP_EXIT_SendData(); /*关闭服务器并发送+++ 退出透传模式*/
    return 0;
}



void Weather_PrintInfo()
{
    Weather_Info_Day* info = NULL;
    for (uint8_t i = 0; i < sizeof(weatherHandle.daysInfo) / sizeof(Weather_Info_Day); i++)
    {
        info = &weatherHandle.daysInfo[i];
        printf("state:%s\n", info->text);
        printf("lowTemp:%s\n", info->lowTemp);
        printf("highTemp:%s\n", info->highTemp);
        printf("rainfall:%s\n", info->rainfall);
        printf("precip:%s\n", info->precip);
        printf("windDir:%s\n", info->windDir);
        printf("windSpeed:%s\n", info->windSpeed);
        printf("windScale:%s\n", info->windScale);
        printf("humidity:%s\n", info->humidity);
        printf("---------------------------------------\n");
    }
}



/*获取返回json的一个键值对中的一个值*/
static char* Weather_GetSingleInfo(char* str, char* key, char* value)
{
    char* p = NULL;
    p = strstr(str, key);
    if (p == NULL)
    {
        printf("get %s fail\n", key);
        return str;
    }

    p = strstr(p, ":");
    if (p == NULL)
    {
        printf("get %s fail\n", key);
        return str;
    }

    //printf("---> %s\n", p);
    sscanf(p, ":\"%15[^\"]\"", value);
    printf("value=%s\n", value);

    return p;
}
