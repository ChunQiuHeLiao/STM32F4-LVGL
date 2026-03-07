#include"vedio.h"
#include"lvgl.h"
#include"main_screen.h"
#include"touch_lcd.h"
Vedio_Handle vedioHandle = { 0 };




/*
* 获取播放视频的标志
* 得到的值，低三位从低到高分别代表 播放上一个 播放/暂停 播放下一个
*/
uint8_t Vedio_GetPlayFlag(){return vedioHandle.isPlay;}
uint8_t Vedio_Get_PlayPre_Flag()
{
    if (vedioHandle.isPlayPrev == 1) vedioHandle.isPlayPrev = 0;
    else return 0;

    return 1;
}
uint8_t Vedio_Get_PlayNext_Flag()
{
    if (vedioHandle.isPlayNext == 1) vedioHandle.isPlayNext = 0;
    else return 0;
    return 1;
}

void Vedio_Set_Play_Flag(uint8_t flag) { vedioHandle.isPlay = flag;}
void Vedio_Set_PlayPre_Flag(uint8_t flag) { vedioHandle.isPlayPrev = flag; }
void Vedio_Set_PlayNext_Flag(uint8_t flag) { vedioHandle.isPlayNext=flag; }

/*
* 设置当前播放视频的名字。
*
*/
void Vedio_SetPlayVedioName(const char* name)
{
    lv_label_set_text(vedioHandle.show_vedio_label, name);
}




void Vedio_Init(Vedio_Handle* vh)
{
    vedioHandle.down_widget = vh->down_widget;
    vedioHandle.up_widget = vh->up_widget;
    vedioHandle.play_vedio_label = vh->play_vedio_label;
    vedioHandle.show_vedio_label = vh->show_vedio_label;
    
    vedioHandle.isPlay=1;
    vedioHandle.sliderMaxValue=1000;
    //Vedio();
}





void Vedio()
{
    //Screen_SetShowDir(1);

    printf("start\n");
    #if LV_USE_GUIDER_SIMULATOR==0
    uint8_t ret=0;
    Screen_SetShowDir(1);
    ret=Vedio_Disp("test.bin",NULL);
    if(ret)
    {
        printf("play vedio fail:%d\n",ret);
    }
    #endif
}


/*视频应用处理函数*/
void Vedio_Handler()
{
    if(vedioHandle.isDispVedio || vedioHandle.isDispVedioFromFe)
    {
        /*初始化一些标志位*/
        
        vedioHandle.isPlay=1;
        vedioHandle.isExitVedio=0;

        #if LV_USE_GUIDER_SIMULATOR==0
        lv_timer_handler(); /*一定先要刷新下屏幕，让使用LVGL绘制的各种组件先显示*/

        /*开始播放视频。先获取该视频的文件路径，这样才好播放*/
        char filePath[48]={0};
        if(vedioHandle.isDispVedio)
        {
            vedioHandle.isDispVedio=0;
            vedioHandle.openMethod=1;
            sprintf(filePath,"%s/%s",VEDIO_DIR,vedioHandle.name);
        }
        else if(vedioHandle.isDispVedioFromFe)
        {
            vedioHandle.isDispVedioFromFe=0;
            vedioHandle.openMethod=2;
            sprintf(filePath,"%s/%s",vedioHandle.path,vedioHandle.name);
        }
        
        printf("vedio path:%s\n",filePath);

        uint8_t ret=0;
        
        ret=Vedio_Disp(filePath,NULL);
        if(ret)
        {
            printf("play vedio fail:%d\n",ret);
        }
        #endif
    }
}


void Vedio_Clear_ScrAct_Event()
{
    if (lv_obj_get_event_count(lv_screen_active()) == 0) return;

    lv_obj_remove_event_cb(lv_screen_active(), Vedio_EventHandler);
}






#if LV_USE_GUIDER_SIMULATOR==0

void Vedio_SetShowDir(uint8_t dir)
{
    LCD_WaitDMA_Over();
    lv_delay_ms(100);
    LCD_SetShowDir(dir);
    lv_delay_ms(100);
    TP_SetShowDir(dir);
    lv_display_t* disp = lv_display_get_default();
    lv_display_set_resolution(disp, LCD_GetWidth(), LCD_GetHeight());    
}

/// @brief 媒体播放核心代码适配LVGL。主要是从SD卡读取数据，再显示到屏幕上，至于屏幕背景颜色，自己设置
/// @param imgPath 文件路径
/// @return 0成功
uint8_t Vedio_Disp(const char* filePath,FrameFunc frameFunc)
{
    printf("disp vedio\n");
    
    Vedio_SetShowDir(1);
    
    // LCD_Clear_All(BLACK); //先清空数据
    
    LCD_PlayVedio(filePath,frameFunc);

    
}
#endif
