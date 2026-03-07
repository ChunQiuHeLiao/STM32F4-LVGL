#include"music_mid.h"


static uint8_t Music_Mid_Show_Name();
static void music_event_cb(lv_event_t* e);


static lv_obj_t* table = NULL; /*显示音乐名和删除按钮的表格*/
uint16_t playedDuration = 0;

void Music_Mid_Init()
{
    table = lv_table_create(ui_ScrMusic_TabPageSongList);
    lv_obj_set_size(table, lv_pct(100), lv_pct(100));
    lv_obj_center(table);
    lv_obj_set_scroll_dir(table, LV_DIR_VER); /*垂直滚动*/
    lv_obj_set_scrollbar_mode(table, LV_SCROLLBAR_MODE_OFF);
    lv_obj_update_layout(table); /*更新布局*/

    uint16_t w = lv_obj_get_width(table);
    uint16_t h = lv_obj_get_height(table);
    lv_table_set_row_count(table, 0);
    lv_table_set_column_count(table, 2);
    lv_table_set_column_width(table, 0, w*0.85);
    lv_table_set_column_width(table, 1, w*0.15);

   /* for (uint8_t i = 0; i < 10; i++)
    {
        lv_table_set_cell_value_fmt(table, i, 0, "精卫-飞流这肯定", i);
        lv_table_set_cell_value_fmt(table, i, 1, LV_SYMBOL_CLOSE);
    }*/

    lv_obj_add_event_cb(table, music_event_cb, LV_EVENT_VALUE_CHANGED, NULL);


    /*初始化音量 ToDo*/
    lv_slider_set_value(ui_ScrMusic_SliderVolume1, musicHandle.vol1, LV_ANIM_OFF);
    lv_slider_set_value(ui_ScrMusic_SliderVolume2, musicHandle.vol2, LV_ANIM_OFF);
    lv_label_set_text_fmt(ui_ScrMusic_LabelAllVolNum, "总音量：%d\n音量1：%d，音量2：%d\n总音量=音量1 乘以 音量2\n音量100播放效果最佳,但声大\n5也还行,音量一般，音质还行", \
        musicHandle.vol1 * musicHandle.vol2, musicHandle.vol1, musicHandle.vol2);


    /*逻辑代码*/
    musicHandle.vol1 = 5;
    musicHandle.vol2 = 1;
    musicHandle.isPlay = 0;

    Music_Mid_Show_Name();
}


void Music_Mid_Handler()
{
    
    if (musicHandle.isInit == 0) return;

    if (musicHandle.isInit==1) /*进入软件执行的操作*/
    {
        musicHandle.isInit = 2;
        Music_Mid_Init();
    }
    if (musicHandle.isDeInit) /*退出软件时执行的操作*/
    {
        musicHandle.isDeInit = 0;
        musicHandle.isInit = 0;
        musicHandle.playStatus = MUSIC_STATUS_NO_START;

        /*ToDo 关闭文件*/

        /*ToDo-- 后续有时间再写个*/
        //musicHandle.playStatus = MUSIC_STATUS_PAUSE; /*暂停音乐，以便下次来的时候继续播放音乐*/
        return;
    }


    if (musicHandle.isClickedPlayMode)
    {
        musicHandle.isClickedPlayMode = 0;

        musicHandle.playMode++;
        if (musicHandle.playMode == 3)  musicHandle.playMode = 0;
        if (musicHandle.playMode == 0) lv_label_set_text(ui_ScrMusic_LabelPlayMode, LV_SYMBOL_LOOP);
        else if (musicHandle.playMode == 1) lv_label_set_text(ui_ScrMusic_LabelPlayMode, LV_SYMBOL_SHUFFLE);
        else if (musicHandle.playMode == 2) lv_label_set_text(ui_ScrMusic_LabelPlayMode, LV_SYMBOL_REFRESH);
    }
    else if (musicHandle.isPrev || musicHandle.isNext)
    {
        if (musicHandle.isPrev)
        {
            musicHandle.isPrev = 0;
            /*限制条件*/
            if (musicHandle.index == 0) return;           
            musicHandle.index--;
        }
        else if (musicHandle.isNext)
        {
            musicHandle.isNext = 0;
            if (musicHandle.index == (musicHandle.num - 1)) return;
            musicHandle.index++;
        }
        
    
        char fileName[48] = { 0 };
        uint8_t ret=ML_GetFileName_By_Index(musicHandle.index, MUSIC_DIR, fileName, sizeof(fileName));
        if (ret)
        {
            printf("get file name by index fail:%d\n", ret);
            return;
        }

        strcpy(musicHandle.playName, fileName);
        memset(musicHandle.songPath, 0, sizeof(musicHandle.songPath) / sizeof(char));
        sprintf(musicHandle.songPath, "%s/%s", MUSIC_DIR, musicHandle.playName);

        musicHandle.playStatus = MUSIC_STATUS_OVER_TO_READY; /*准备播放音乐*/
    }
    else if (musicHandle.isClickedPlay) /*点击了播放按钮，用于播放和暂停 按钮显示*/
    {
        musicHandle.isClickedPlay = 0;

        /*判断是否是连歌曲都没有点击，直接点击播放*/
        if (strlen(musicHandle.playName) == 0) return; 
        musicHandle.isPlay = (musicHandle.isPlay == 0) ? 1 : 0;

        /*开始或暂停播放*/
        if (musicHandle.isPlay) /*开始播放*/
        {
            lv_label_set_text(ui_ScrMusic_LabelPlay, LV_SYMBOL_PAUSE);
            if (musicHandle.playStatus == MUSIC_STATUS_PAUSE)
            {
                musicHandle.playStatus = MUSIC_STATUS_PLAYING;
            }
        }
        else /*暂停*/
        {
            if (musicHandle.playStatus == MUSIC_STATUS_PLAYING)
            {
                musicHandle.playStatus = MUSIC_STATUS_PAUSE;
            }
            lv_label_set_text(ui_ScrMusic_LabelPlay, LV_SYMBOL_PLAY);
        }

    }
    else if (musicHandle.isModifyVol) /*修改音量*/
    {
        musicHandle.isModifyVol = 0;
        lv_label_set_text_fmt(ui_ScrMusic_LabelAllVolNum, "总音量：%d\n音量1：%d，音量2：%d\n总音量=音量1 乘以 音量2\n音量100播放效果最佳,但声大\n5也还行,音量一般，音质还行", \
            musicHandle.vol1 * musicHandle.vol2, musicHandle.vol1, musicHandle.vol2);
    }
    else if (musicHandle.isDelete) /*删除歌曲*/
    {
        musicHandle.isDelete = false;
        if (musicHandle.isAccessFile)
        {
            /*ToDo 关闭正在播放的音乐*/
            printf("close file:%s\n", musicHandle.songPath);
        }
        /*ToDoCode---删除文件代码--*/
        char filePath[48] = { 0 };
        sprintf(filePath, "%s/%s", MUSIC_DIR, musicHandle.deleteName);
        printf("delete file:%s\n", filePath);

        /*如果删除的文件与播放的文件相同，则会结束播放*/
        if (strcmp(musicHandle.playName, musicHandle.deleteName) == 0)
        {
            musicHandle.playStatus = MUSIC_STATUS_OVER;
        }

    }
    else if (musicHandle.playStatus == MUSIC_STATUS_READY)
    {
        musicHandle.playStatus = MUSIC_STATUS_PLAYING;
        musicHandle.isClickedPlay = true; /*代表点击了播放按钮，会开始播放音乐，这样按钮的图片就会变换*/
        musicHandle.isPlay = 0;

        lv_obj_set_state(ui_ScrMusic_BtnPlay, LV_STATE_DISABLED, false); /*可以点击播放按钮了*/
        lv_label_set_text(ui_ScrMusic_LabelSongName, musicHandle.playName); /*显示要播放的歌曲名*/

        /*ToDoCode--播放音乐初始化代码和获取播放总时长代码*/


        musicHandle.isAccessFile = true;
        uint16_t duration = 140;
        playedDuration = 0;
        lv_slider_set_range(ui_ScrMusic_Slider, 0, duration);
    }
    else if (musicHandle.playStatus == MUSIC_STATUS_PLAYING) /*播放音乐，需要循环调用*/
    {
        /*ToDoCode---播放音乐代码和获取已播放时长代码*/
        uint16_t duration = 140;
        playedDuration++;
        lv_delay_ms(100);


        lv_label_set_text_fmt(ui_ScrMusic_LabelSongDuration, "%02d:%02d/%02d:%02d",playedDuration/60,playedDuration%60\
            , duration / 60, duration % 60);
        lv_slider_set_value(ui_ScrMusic_Slider, playedDuration, LV_ANIM_OFF);
        if (playedDuration == duration)
        {
            musicHandle.playStatus = MUSIC_STATUS_OVER;
        }
    }
    else if (musicHandle.playStatus == MUSIC_STATUS_OVER || musicHandle.playStatus== MUSIC_STATUS_OVER_TO_READY)
    {
        playedDuration = 0;
        musicHandle.isClickedPlay = true; /*再点击播放按钮，代表暂停音乐*/;
        lv_obj_set_state(ui_ScrMusic_BtnPlay, LV_STATE_DISABLED, true);
        lv_label_set_text(ui_ScrMusic_LabelSongName, "");
        lv_label_set_text(ui_ScrMusic_LabelSongDuration, "00:00/00:00");
        lv_slider_set_value(ui_ScrMusic_Slider, 0, LV_ANIM_OFF);

        /*ToDo 关闭文件*/


        musicHandle.isAccessFile = false;
        if (musicHandle.playStatus == MUSIC_STATUS_OVER)
        {
            musicHandle.playStatus = MUSIC_STATUS_NO_START;
        }
        else if (musicHandle.playStatus == MUSIC_STATUS_OVER_TO_READY)
        {
            musicHandle.playStatus = MUSIC_STATUS_READY; /*开始准备了*/
        }
    }

    /*连接服务器，下载歌曲部分*/
    if (musicHandle.isClickedConn) 
    {
        musicHandle.isClickedConn = false;

        lv_label_set_text(ui_ScrMusic_LabelConn, "正在尝试与手机应用建立通讯,请稍等...");
        lv_delay_ms(3000);
        musicHandle.connStatus = CONN_STATUS_CONN_TIMEOUT;
        lv_label_set_text(ui_ScrMusic_LabelConn, "通讯失败，手机应用未打开或手机与该设备未连接在同一网络");
    }
    else if (musicHandle.connStatus == CONN_STATUS_SUC) /*连接成功*/
    {
        /*ToDo 开始传数据*/

    }

}






/*音乐添加列表*/
static void Music_Mid_AddList(const char* songName,uint16_t index)
{
    lv_table_set_cell_value_fmt(table, index, 0, "%s", songName);
    lv_table_set_cell_value_fmt(table, index, 1, LV_SYMBOL_CLOSE);
}


/*在列表里显示当前音乐文件夹有哪些音乐*/
static uint8_t Music_Mid_Show_Name()
{
    lv_fs_dir_t dir;
    lv_fs_res_t res = 0;
    res = lv_fs_dir_open(&dir, MUSIC_DIR);
    if (res)
    {
        printf("open dir fail:%d\n", res);
        return res;
    }

    /*读取目录下的文件*/
    char fileName[32] = { 0 };

    musicHandle.num = 0; /*初始化歌曲数为0*/
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
        Music_Mid_AddList(fileName, musicHandle.num);
        musicHandle.num++;
    }

    if (res == LV_RESULT_OK)
    {
        lv_fs_dir_close(&dir);
    }

    printf("music num:%d\n", musicHandle.num);

    return res;
}



/*********-----------------------回调函数*/


/*歌曲表格点击的回调函数*/
static void music_event_cb(lv_event_t* e)
{
    uint32_t row = 0, col = 0;
    lv_obj_t* table = lv_event_get_target_obj(e);
    lv_table_get_selected_cell(table, &row, &col); /*获取点击的那个表格的单元格的行列*/
    const char* val = lv_table_get_cell_value(table, row, col); /*获取里面的值*/

    /*代表要删除该歌曲*/
    if (col == 1)
    {
        /*显示删除对话框*/
        lv_obj_remove_flag(ui_ScrMusic_ContDialog, LV_OBJ_FLAG_HIDDEN);
        val= lv_table_get_cell_value(table, row, col-1); /*获取这首歌的名字*/
        lv_label_set_text_fmt(ui_ScrMusic_DialogLabelMain, "是否删除这首歌 %s\n", val);

        memset(musicHandle.deleteName,0,sizeof(musicHandle.deleteName)/sizeof(uint8_t));
        strcpy(musicHandle.deleteName, val);
  
        return;
    }

    /*如果已在播放歌曲，则要关闭该歌曲*/
    if (musicHandle.isAccessFile)
    {
        musicHandle.playStatus= MUSIC_STATUS_OVER_TO_READY;
        printf("ready close file:%s\n", musicHandle.songPath);
    }
    else
    {
        musicHandle.playStatus = MUSIC_STATUS_READY;
        musicHandle.isClickedPlay = true;
    }

    musicHandle.index = row; /*歌曲索引*/
    /*开始播放音乐*/
    lv_tabview_set_active(ui_ScrMusic_TabView, 1, LV_ANIM_OFF);

    strcpy(musicHandle.playName, val);
    memset(musicHandle.songPath, 0, sizeof(musicHandle.songPath) / sizeof(char));
    sprintf(musicHandle.songPath, "%s/%s", MUSIC_DIR, musicHandle.playName);
    //if (strcmp(val + strlen(val) - 11, "[connected]") == 0) printf("already con\n");
    //printf("clicked (%d,%d),val=%s\n", row, col, val); /*加7是因为 LV_SYMBOL_XXX 占3个字节，后面还有四个空格，过了7字节后才是wifi名*/
}
