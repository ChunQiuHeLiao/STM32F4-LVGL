#include"audio_rec_mid.h"
#include"SquareLine/screens/ui_ScrRec.h"
#include"util_mid.h"
#include"Core/main_lib.h"

#include"audio_player.h"
#include"audio_recording.h"

static void Audio_Rec_Status_Handler();
static uint8_t AR_Mid_Show_Name();

static void ar_event_cb(lv_event_t* e);

static lv_obj_t* table = NULL; /*显示音乐名和删除按钮的表格*/



/*****自定义和需要实现的接口部分***********/
// #include"stdio.h"
// FILE* fp = NULL;

static uint8_t Audio_Init()
{
    Audio_Player_Init(I2S_AUDIOFREQ_16K);
}
/*音频录制初始化*/
static uint8_t AudioRec_Init(const char* path)
{
   return  Audio_Recording_Start(path);
    // fp=fopen(path, "w");
    // if (fp == NULL)printf("fopen err\n");
    // return 0;
}

/*本首音乐播放立刻结束*/
static void AudioRec_Over()
{
    Audio_Recording_Stop();
    // fclose(fp);
}

/*音乐录制处理函数。*/
static uint8_t AudioRec_Handler()
{
    return Audio_Recording_Handler();
    // return 0;
}

static uint16_t AudioRec_GetDuration()
{
    return Audio_Recording_GetDuration();
    // return 123;
}



/*********录音播放处理部分  函数*************/
/*播放音乐前的初始化*/
static uint8_t Audio_Play_Init(const char* path)
{
    return Audio_Player_Play_Init(path);
}

/*本首音乐播放立刻结束*/
static void Audio_Play_Over()
{
    Audio_Player_PlayOver();
}

/*音乐播放中的处理函数。*/
static uint8_t Audio_Play_Handler()
{
    return Audio_Player_Handler();
}

static uint16_t Audio_Play_GetDuration()
{
    return Audio_Player_GetDuration();
    // return 123;
}

static int a = 1;
static uint16_t Audio_Play_GetPlayedDuration()
{
    return Audio_Player_GetPlayedDuration();
    // return a;
}

static void Audio_Play_SetVol(uint16_t volume)
{

    Audio_Player_SetVolume((volume*5));
}


/******************end***********************/






static void Init()
{
    table = lv_table_create(ui_ScrRec_ContBody);
    lv_obj_set_size(table, lv_pct(100), lv_pct(100));
    lv_obj_center(table);
    lv_obj_set_scroll_dir(table, LV_DIR_VER); /*垂直滚动*/
    lv_obj_set_scrollbar_mode(table, LV_SCROLLBAR_MODE_OFF);
    lv_obj_update_layout(table); /*更新布局*/

    uint16_t w = lv_obj_get_width(table);
    uint16_t h = lv_obj_get_height(table);
    lv_table_set_row_count(table, 0);
    lv_table_set_column_count(table, 2);
    lv_table_set_column_width(table, 0, w * 0.85);
    lv_table_set_column_width(table, 1, w * 0.15);

    lv_obj_add_event_cb(table, ar_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /*初始化音量 ToDo */
    lv_slider_set_value(ui_ScrRec_SliderVolume, arHandle.volume, LV_ANIM_OFF);
    lv_label_set_text_fmt(ui_ScrRec_LabelVolume,"音量:%d",arHandle.volume);
    Audio_Play_SetVol(arHandle.volume);

    /*逻辑代码*/
    UtilMid_RefleshFileTable(table, AUDIO_RECORDING_DIR,&arHandle.num);
    arHandle.status = AUDIO_STATUS_NO_START;
    arHandle.isAccessFile = false;

    Audio_Init();
}

static void DeInit()
{
    /*ToDo 关闭文件*/
    AudioRec_Over();
}

void Audio_Rec_Mid_Handler()
{
    if (arHandle.isInit == 0) return;
    if (arHandle.isInit==1)
    {
        arHandle.isInit = 2;
        Init();
    }
    if (arHandle.isDeInit)
    {
        arHandle.isDeInit = 0;
        arHandle.isInit = 0;
        DeInit();

        return;
    }


    /*正片开始*/
    if (arHandle.isClickedRec)
    {
        arHandle.isClickedRec = false;
        /*开始录音*/
        if (arHandle.status == AUDIO_STATUS_NO_START)
        {
            arHandle.status = AUDIO_STATUS_RECORD_READY;
        }
        else if (arHandle.status == AUDIO_STATUS_RECORDING) /*暂停录音*/
        {
            lv_label_set_text(ui_ScrRec_LabelRecord, "继续");
            arHandle.status = AUDIO_STATUS_RECORD_PAUSE;
        }
        else if (arHandle.status == AUDIO_STATUS_RECORD_PAUSE) /*继续播放*/
        {
            lv_label_set_text(ui_ScrRec_LabelRecord, "暂停");
            arHandle.status = AUDIO_STATUS_RECORDING;
        }
    }
    else if (arHandle.isClickedPlay) /*点击了播放按钮*/
    {
        arHandle.isClickedPlay = false;
        /*开始播放录音*/
        if (arHandle.status == AUDIO_STATUS_NO_START)
        {
            arHandle.status = AUDIO_STATUS_PLAY_READY;
        }
        else if (arHandle.status == AUDIO_STATUS_PLAYING) /*暂停播放录音*/
        {
            lv_label_set_text(ui_ScrRec_LabelPlay, "继续");
            arHandle.status = AUDIO_STATUS_PLAY_PAUSE;
        }
        else if (arHandle.status == AUDIO_STATUS_PLAY_PAUSE) /*继续播放*/
        {
            lv_label_set_text(ui_ScrRec_LabelPlay, "暂停");
            arHandle.status = AUDIO_STATUS_PLAYING;
        }
    } 
    else if (arHandle.isDelete) /*开始删除文件*/
    {
        arHandle.isDelete = false;
        if (arHandle.isAccessFile) /*如果在访问文件则关闭*/
        {
            arHandle.isAccessFile = 0;
            /*ToDo 用文件指针关闭播放的文件*/
            AudioRec_Over();
            printf("close file:%s\n", arHandle.filePath);
        }
        /*ToDo 删除指定文件并刷新下文件列表*/
        char filePath[48] = { 0 };
        sprintf(filePath, "%s/%s", AUDIO_RECORDING_DIR, arHandle.deleteName);
        lv_fs_delete(filePath);
        printf("delete file:%s\n", filePath);


        /*如果删除的文件与播放的文件相同，则会结束播放*/
        if (strcmp(arHandle.playName, arHandle.deleteName) == 0)
        {
            arHandle.status = AUDIO_STATUS_OVER;
        }

        UtilMid_RefleshFileTable(table, AUDIO_RECORDING_DIR, &arHandle.num);
    }
    else if (arHandle.isModifyVol)
    {
        arHandle.isModifyVol = 0;
        Audio_Play_SetVol(arHandle.volume);
    }


    Audio_Rec_Status_Handler();   
}




static void Audio_Rec_Status_Handler()
{
    switch (arHandle.status)
    {
    case AUDIO_STATUS_RECORD_READY:
        lv_label_set_text(ui_ScrRec_LabelRecord, "暂停");
        lv_obj_set_state(ui_ScrRec_Btn_Play, LV_STATE_DISABLED, true); /*让播放按钮按不了*/

        memset(arHandle.recordName, 0, sizeof(arHandle.recordName) / sizeof(char));
        sprintf(arHandle.recordName, "%s%d", "audio", arHandle.num); /*得到录音的文件名*/
        sprintf(arHandle.filePath, "%s/%s", AUDIO_RECORDING_DIR, arHandle.recordName); /*得到文件路径*/

        /*显示你录音的文件名*/
        lv_label_set_text(ui_ScrRec_ProcessLabel, arHandle.recordName);

        /*ToDo 完成录音初始化代码，和得到SD卡里有"录音几"的文件，以便对新的录音文件命名*/
        AudioRec_Init(arHandle.filePath);

        arHandle.isAccessFile = true; /*正在访问文件*/
        arHandle.recordedDuration = 0;
        arHandle.status = AUDIO_STATUS_RECORDING;
        break;
    case AUDIO_STATUS_RECORDING: /*正在录音，循环调用*/
        /*ToDo 获取录音时长的代码*/
        static uint8_t ret=0;
        ret=Audio_Recording_Handler();
        if(ret==1) 
        {
            PrintErr(ret);
            arHandle.status = AUDIO_STATUS_OVER;
        }
        arHandle.recordedDuration=AudioRec_GetDuration();
        lv_label_set_text_fmt(ui_ScrRec_PrecessRecTime,"%02d:%02d", arHandle.recordedDuration / 60, arHandle.recordedDuration % 60);
        //lv_delay_ms(100);

        break;


    case AUDIO_STATUS_PLAY_READY:
        lv_label_set_text(ui_ScrRec_LabelPlay, "暂停");
        lv_obj_set_state(ui_ScrRec_Btn_Record, LV_STATE_DISABLED, true); /*让录音按钮按不了*/
        lv_obj_set_state(ui_ScrRec_Btn_Play, LV_STATE_DISABLED, false); /*让播放按钮可以按*/

        lv_label_set_text(ui_ScrRec_ProcessLabel, arHandle.playName); /*显示你点击的录音，即要播放的*/

        /*ToDo 完成播放录音初始化代码*/
        Audio_Play_Init(arHandle.filePath);
        arHandle.duration =Audio_Play_GetDuration();
        arHandle.playedDuration = 0;
        lv_bar_set_range(ui_ScrRec_Process, 0, arHandle.duration);

        arHandle.isAccessFile = true;
        arHandle.status = AUDIO_STATUS_PLAYING;
        break;
    case AUDIO_STATUS_PLAYING: /*正在播放录音，循环调用*/
        /*ToDo 获取录音时长的代码*/
        arHandle.playedDuration = Audio_Play_GetPlayedDuration();

        lv_label_set_text_fmt(ui_ScrRec_Process_Time, "%02d:%02d/%02d:%02d", arHandle.playedDuration / 60, arHandle.playedDuration % 60, \
            arHandle.duration / 60, arHandle.duration % 60);
        lv_bar_set_value(ui_ScrRec_Process, arHandle.playedDuration, LV_ANIM_OFF);  /*设置播放的进度条*/

        /*音乐播放处理函数*/
        if (Audio_Play_Handler() == 1)
        {
            arHandle.status = AUDIO_STATUS_OVER;
        }

        /*播放录音结束*/
        if (arHandle.playedDuration == arHandle.duration)
        {
            arHandle.status = AUDIO_STATUS_OVER;
        }
        lv_delay_ms(100);
        break;
    default:
        break;
    }

    if (arHandle.status == AUDIO_STATUS_OVER || arHandle.status == AUDIO_STATUS_OVER_TO_READY)  /*录音或播放录音结束*/
    {
        lv_label_set_text(ui_ScrRec_LabelRecord, "录音");
        lv_label_set_text(ui_ScrRec_LabelPlay, "播放");
        lv_label_set_text(ui_ScrRec_ProcessLabel, "");

        lv_obj_set_state(ui_ScrRec_Btn_Record, LV_STATE_DISABLED, false);
        lv_obj_set_state(ui_ScrRec_Btn_Play, LV_STATE_DISABLED, true);
        lv_bar_set_value(ui_ScrRec_Process, 0, LV_ANIM_OFF);  /*设置播放的进度条为0*/
        lv_label_set_text_fmt(ui_ScrRec_Process_Time, "00:00/00:00");

        UtilMid_RefleshFileTable(table, AUDIO_RECORDING_DIR, &arHandle.num);
        /*ToDo 关闭文件代码*/
        AudioRec_Over();
        Audio_Play_Over();

        arHandle.isAccessFile = false;
        if (arHandle.status == AUDIO_STATUS_OVER)
        {
            arHandle.status = AUDIO_STATUS_NO_START;
        }
        else if (arHandle.status == AUDIO_STATUS_OVER_TO_READY)
        {
            arHandle.status = AUDIO_STATUS_PLAY_READY;
        }
    }
}



/*音乐添加列表*/
static void AR_Mid_AddList(const char* songName, uint16_t index)
{
    lv_table_set_cell_value_fmt(table, index, 0, "%s", songName);
    lv_table_set_cell_value_fmt(table, index, 1, LV_SYMBOL_CLOSE);
}


/*在列表里显示当前录音件夹有哪些录音*/
static uint8_t AR_Mid_Show_Name()
{
    lv_fs_dir_t dir;
    lv_fs_res_t res = 0;
    res = lv_fs_dir_open(&dir, AUDIO_RECORDING_DIR);
    if (res)
    {
        printf("open dir fail:%d\n", res);
        return res;
    }

    /*读取目录下的文件*/
    char fileName[32] = { 0 };

    arHandle.num = 0; /*初始化录音个数为0*/
    while (1)
    {
        res = lv_fs_dir_read(&dir, fileName, sizeof(fileName));
        if (res)
        {
            printf("read dir fail:%d\n", res);
            break;
        }

        if (fileName[0] == 0) break; /*代表该目录文件读取完毕,获取判断该数组长度为0*/

        /*把文件内容加载到里面*/
        AR_Mid_AddList(fileName, arHandle.num);
        arHandle.num++;
    }

    if (res == LV_RESULT_OK)
    {
        lv_fs_dir_close(&dir);
    }

    printf("audio record num:%d\n", arHandle.num);

    return res;
}



/*********-----------------------回调函数*/


/*录音表格点击的回调函数*/
static void ar_event_cb(lv_event_t* e)
{
    uint32_t row = 0, col = 0;
    lv_obj_t* table = lv_event_get_target_obj(e);
    lv_table_get_selected_cell(table, &row, &col); /*获取点击的那个表格的单元格的行列*/
    const char* val = lv_table_get_cell_value(table, row, col); /*获取里面的值*/

    /*代表要删除该录音*/
    if (col == 1)
    {
        /*显示删除对话框*/
        lv_obj_remove_flag(ui_ScrRec_ContDialog, LV_OBJ_FLAG_HIDDEN);
        val = lv_table_get_cell_value(table, row, col - 1); /*获取该录音的名字*/
        lv_label_set_text_fmt(ui_ScrRec_DialogLabelMain, "是否删除该录音 %s\n注意:录音时不要删除文件,否则录音将会结束", val);

        memset(arHandle.deleteName, 0, sizeof(arHandle.deleteName) / sizeof(uint8_t));
        strcpy(arHandle.deleteName, val);

        return;
    }

    arHandle.index = row; /*录音的索引*/
    /*开始播放录音*/

    /*如果一开始没有播放录音，则开始播放，如果 正在播放或暂停了，则要关闭当前录音，播放新的*/
    if (arHandle.status == AUDIO_STATUS_NO_START)
    {
        arHandle.isClickedPlay = true;
    }
    else if (arHandle.status == AUDIO_STATUS_PLAYING || arHandle.status == AUDIO_STATUS_PLAY_PAUSE)
    {
        printf("ready close file:%s\n", arHandle.filePath);
        arHandle.status = AUDIO_STATUS_OVER_TO_READY;
    }
    else return; /*其它也就是处于录音阶段，点了不会打断录音*/

    strcpy(arHandle.playName, val);
    memset(arHandle.filePath, 0, sizeof(arHandle.filePath) / sizeof(char));
    sprintf(arHandle.filePath, "%s/%s", AUDIO_RECORDING_DIR, arHandle.playName);
    //if (strcmp(val + strlen(val) - 11, "[connected]") == 0) printf("already con\n");
    //printf("clicked (%d,%d),val=%s\n", row, col, val); /*加7是因为 LV_SYMBOL_XXX 占3个字节，后面还有四个空格，过了7字节后才是wifi名*/
}
