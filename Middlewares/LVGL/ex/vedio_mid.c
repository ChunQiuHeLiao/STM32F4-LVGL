#include"ex/vedio_mid.h"
#include"Core/vedio.h"

/*思路：
    1.
*/

static uint8_t Vedio_Mid_Name_Show();


/*************自己实现的接口***************/

// typedef lv_fs_file_t FIL;
// #define f_lseek(fp,pos) lv_fs_seek(fp,pos,1)

// typedef struct _Vedio_Info
// {
//     uint32_t readSize; //当前读取文件的字节数
//     uint32_t allSize; //视频 显示数据总大小
//     uint32_t oneFrameDataSize; /*刷新一帧图像的需要数据大小*/

//     uint16_t allFrameNum; //总帧数
//     uint16_t frameIndex; //当前播放的帧索引

//     uint8_t isBufHaveData : 1; //缓存区是否有数据
//     uint8_t bufNum : 2; //如果缓存区有数据，哪个有，1还是2，如无，则0

//     /*视频在屏幕的位置*/
//     uint16_t x;
//     uint16_t y;
//     uint16_t w;
//     uint16_t h;

//     FIL fp;
// }Vedio_Info;

// typedef uint8_t(*FrameFunc)(Vedio_Info* vedioInfo);


/// @brief 视频播放一帧后会调用此函数
/// @param vedioInfo 视频信息的结构体
/// @return 0表示无事件产生，1代表退出该视频,2表示有事件产生
uint8_t Vedio_FrameFunc(Vedio_Info* vedioInfo)
{
    uint8_t ret = 0;
    uint8_t t = 0;

    while (vedioHandle.isPlay == 0)
    {
        t = lv_timer_handler();
        delay_ms(t);

        if (vedioHandle.isExitVedio)
        {
            vedioHandle.isExitVedio = 0;
            vedioHandle.isPlay = 1;
            return 1;  //点了退出按钮 或 为了播放上下视频而退出
        }
        //if(vedioHandle.isPlayNext||vedioHandle.isPlayPrev) return 1;
    }

    t = lv_timer_handler(); /*每一帧进行LVGL事件处理和图像处理*/


    if (vedioHandle.isExitVedio == 1)
    {
        printf("exit flag:%d\n", vedioHandle.isExitVedio);
        vedioHandle.isExitVedio = 0;
        vedioHandle.isPlay = 1;
        return 1;  //点了退出按钮 或 为了播放上下视频而退出
    }

    /*如果滑动条拖了并放下，则修改文件指针，让视频可以随意播放*/
    if (vedioHandle.isSliderReleased == 1)
    {
        vedioHandle.isSliderReleased = 0;
        lv_obj_t* slider = ui_ScrVedioShow_Slider;
        vedioInfo->frameIndex = (float)lv_slider_get_value(slider) / lv_slider_get_max_value(slider) * vedioInfo->allFrameNum;
        vedioInfo->readSize = vedioInfo->frameIndex * vedioInfo->w * vedioInfo->h * 2;
        // printf("rd size:%d\n",vedioInfo->readSize);
        f_lseek(&vedioInfo->fp, (vedioInfo->readSize + 4)); /*文件指针修改。加的4字节不要忘，因为有四字节头信息，即宽高数据*/
    }


    /*显示播放进度条*/
    lv_obj_t* slider = ui_ScrVedioShow_Slider;
    uint16_t maxVal = lv_slider_get_max_value(slider);
    lv_slider_set_value(slider, (float)(vedioInfo->frameIndex) / (vedioInfo->allFrameNum) * maxVal, LV_ANIM_OFF);


    ret = LCD_WaitDMA_Over();
    if (ret) printf("dma timeout\n");

    if (t != LV_DEF_REFR_PERIOD)
    {
       LCD_SetShowArea(vedioInfo->x, vedioInfo->y, vedioInfo->x + vedioInfo->w - 1, vedioInfo->y + vedioInfo->h - 1);
    }

    return 0;
}


/*显示图片接口。需要自己实现调用*/
static uint8_t Vedio_Show(const char* imgPath)
{
   return Vedio_Disp(imgPath, Vedio_FrameFunc);
    //printf("%s 视频显示完毕\n", imgPath);
}

// /*获取图片宽高*/
// static uint8_t Vedio_GetResolution(const char* filePath,uint16_t* w, uint16_t* h)
// {
//     return LCD_GetMediaSize(filePath,w,h);
// }





/*******************end*******************/



static uint8_t Vedio_Mid_Name_Show();
uint8_t Vedio_FrameFunc(Vedio_Info* vedioInfo);

void Vedio_Mid_Handler()
{
    if (vedioHandle.isInit)
    {
        vedioHandle.isInit = 0;
        vedioHandle.isPlay = 1;
        Vedio_Mid_Name_Show();
    }
    else if (vedioHandle.isDispVedio)
    {
        vedioHandle.isDispVedio = 0;
        vedioHandle.isExitVedio = 0; /*初始化为0，以防播放就秒退*/
        vedioHandle.isDispOver = 0;

        /*1. 根据索引值获取文件名*/
        char fileName[32] = { 0 };
        ML_GetFileName_By_Index(vedioHandle.index, VEDIO_DIR, fileName, sizeof(fileName) - 1);

        /*2. 更新视频标题*/
        lv_label_set_text(ui_ScrVedioShow_LabelTitle, fileName);
        printf("vedio name:%s\n", fileName);

        /*3. 获取视频路径*/
        char filePath[48] = { 0 };
        sprintf(filePath, "%s/%s", VEDIO_DIR, fileName);

        /*4. 开始显示视频*/
        Vedio_Show(filePath);

        /*5.视频播放完毕,要把播放标签改为 播放符号，代表暂停了*/
        vedioHandle.isPlay++;
        lv_label_set_text(ui_ScrVedioShow_LabelPlay, LV_SYMBOL_PLAY);
        //vedioHandle.isDispOver = 1;
    }
    else if (vedioHandle.isPlayPrev)
    {
        vedioHandle.isPlayPrev = 0;
        if (vedioHandle.index == 0) return;
        vedioHandle.index--;

        vedioHandle.isDispVedio = 1;
    }
    else if (vedioHandle.isPlayNext)
    {
        vedioHandle.isPlayNext = 0;
        if ((vedioHandle.index + 1) == vedioHandle.num) return;
        vedioHandle.index++;

        vedioHandle.isDispVedio = 1;
    }
}





/*在列表里显示当前图片文件夹有哪些视频*/
static uint8_t Vedio_Mid_Name_Show()
{
    lv_fs_dir_t dir;
    lv_fs_res_t res = 0;
    res = lv_fs_dir_open(&dir, VEDIO_DIR);
    if (res)
    {
        printf("open dir fail:%d\n", res);
        return res;
    }

    /*读取目录下的文件*/
    char fileName[32] = { 0 };
    uint16_t vedio_num = 0;;
    while (1)
    {
        res = lv_fs_dir_read(&dir, fileName, sizeof(fileName));
        if (res)
        {
            printf("read dir fail:%d\n", res);
            break;
        }

        if (fileName[0] == 0) break; /*代表该目录文件读取完毕,获取判断该数组长度为0*/

        //print_lvgl_mem_usage();
        /*把图片名加载到相册列表中*/
        lv_table_set_cell_value_fmt(ui_ScrVedio_Table, vedio_num, 0, LV_SYMBOL_IMAGE"   %s", fileName);
        vedio_num++;
    }
    vedioHandle.num = vedio_num;

    printf("vedio_num:%d\n", vedio_num);

    lv_fs_dir_close(&dir);
    return res;
}



