#include"weather_mid.h"
#include<stdlib.h>

static void Weather_Mid_ShowResponseUI(uint8_t flag);
static uint8_t Weather_Mid_Update(void);


void Weather_Mid_Init()
{
    
}


void Weather_Mid_Handler()
{
    if (weatherHandle.isReflush)
    {
        weatherHandle.isReflush = 0;
        lv_label_set_text(ui_ScrWeather_LabelResponseRes, "获取中...");
        lv_obj_remove_flag(ui_ScrWeather_Mask, LV_OBJ_FLAG_HIDDEN); /*显示MASK*/
        Weather_Mid_Update();
    }
}


/*更新天气信息*/
static uint8_t Weather_Mid_Update(void)
{
    uint8_t ret = 0;
    Weather_Handle* weatherHandle=Weather_GetHandlePointer();
    Weather_Info_RealTime* info_realtime = &weatherHandle->realTimeInfo;
    Weather_Info_Day* info = &weatherHandle->daysInfo[0];

    char* city=lv_label_get_text(ui_ScrWeather_LabelTitle);
    ret=Weather_UpdateInfo_RealTime(city);
    UART_ClearRxFlag(3);
    if (ret)
    {
        printf("update realtime fail:%d\n", ret);
        Weather_Mid_ShowResponseUI(1);
        return 1;
    }
    else
    {
        //lv_label_set_text(ui_LabelTodayWeather, info_realtime->text);
        lv_label_set_text(ui_LabelCurTemp, info_realtime->temp);
    }

    ret= Weather_UpdateInfo(city);
    UART_ClearRxFlag(3);
    if (ret)
    {
        printf("update info fail:%d\n", ret);
        Weather_Mid_ShowResponseUI(1);
        return 1;
    }
    else
    {
        uint8_t precip = 0; /*降雨概率*/

        /*这里后续可以写 根据天气现象更改天气图片的代码*/
        lv_label_set_text_fmt(ui_LabelTodayWeather, "%s %s~%s", info_realtime->text, info->lowTemp, info->highTemp);

        precip = atof(info->precip) * 100;
        if(precip!=0) lv_label_set_text_fmt(ui_LabelPrecip1, "%d%%",precip);
        else lv_label_set_text(ui_LabelPrecip1, "");
        lv_label_set_text_fmt(ui_LabelTodayTemp1, "%s~%s", info->lowTemp, info->highTemp);

        /*当日详细天气信息*/
        lv_label_set_text_fmt(ui_LabelRainfall,"rainfall\n%smm",info->rainfall);
        lv_label_set_text_fmt(ui_LabelHumidity,"humidity\n%s%%",info->humidity);

        /*显示风向*/
        if (strcmp(info->windDir, "NE") == 0) lv_label_set_text_fmt(ui_LabelWind, "Northeast wind\n %s scale", info->windScale);
        else if (strcmp(info->windDir, "SE") == 0) lv_label_set_text_fmt(ui_LabelWind, "Southeast wind\n%s scale", info->windScale);
        else if (strcmp(info->windDir, "NW") == 0) lv_label_set_text_fmt(ui_LabelWind, "Northwest wind\n%s scale", info->windScale);
        else if (strcmp(info->windDir, "SW") == 0) lv_label_set_text_fmt(ui_LabelWind, "Southwest wind\n%s scale", info->windScale);
        else if (strcmp(info->windDir, "E") == 0) lv_label_set_text_fmt(ui_LabelWind, "East wind\n%s scale", info->windScale);
        else if (strcmp(info->windDir, "S") == 0) lv_label_set_text_fmt(ui_LabelWind, "South wind\n%s scale", info->windScale);
        else if (strcmp(info->windDir, "W") == 0) lv_label_set_text_fmt(ui_LabelWind, "West wind\n%s scale", info->windScale);
        else if (strcmp(info->windDir, "N") == 0) lv_label_set_text_fmt(ui_LabelWind, "North wind\n%s scale", info->windScale);
        lv_label_set_text_fmt(ui_LabelWindSpeed,"wind speed\n%skm/h",info->windSpeed);
        

        /****第二三天的信息****/

        info =&weatherHandle->daysInfo[1];
        precip = atof(info->precip) * 100;
        if (precip != 0) lv_label_set_text_fmt(ui_LabelPrecip3, "%d%%", precip);
        else lv_label_set_text(ui_LabelPrecip3,"");
        lv_label_set_text_fmt(ui_LabelTodayTemp3, "%s~%s", info->lowTemp, info->highTemp);


        info =&weatherHandle->daysInfo[2];
        precip = atof(info->precip) * 100;
        if (precip != 0) lv_label_set_text_fmt(ui_LabelPrecip4, "%d%%", precip);
        else lv_label_set_text(ui_LabelPrecip4,""); /*清空*/
        lv_label_set_text_fmt(ui_LabelTodayTemp4, "%s~%s", info->lowTemp, info->highTemp);
    }

    Weather_Mid_ShowResponseUI(0);
    return 0;
}


/*根据天气API响应结果，显示UI，成功和失败*/
/*0:成功，1失败*/
static void Weather_Mid_ShowResponseUI(uint8_t flag)
{
    lv_obj_add_flag(ui_ScrWeather_Spinner, LV_OBJ_FLAG_HIDDEN); /*隐藏spinner*/

    /*改变显示响应的字体大小*/
    if (flag == 0) lv_label_set_text(ui_ScrWeather_LabelResponseRes, "获取成功");
    else lv_label_set_text(ui_ScrWeather_LabelResponseRes, "获取失败，请检查网络或再次尝试");

    lv_delay_ms(2000);
    lv_obj_remove_flag(ui_ScrWeather_Spinner, LV_OBJ_FLAG_HIDDEN); /*spinner*/
    lv_obj_add_flag(ui_ScrWeather_Mask, LV_OBJ_FLAG_HIDDEN);
}



