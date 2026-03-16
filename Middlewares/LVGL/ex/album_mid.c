#include"ex/album_mid.h"
#include"lcd.h"

//void Album_MID_Init();
static uint8_t Album_Mid_Name_Show();


/*************自己实现的接口***************8/

/*显示图片接口。需要自己实现调用*/
static uint8_t Album_Show(const char* imgPath)
{
    LCD_DispImg(imgPath);
    // printf("%s 图片显示完毕\n",imgPath);
}

/*获取图片宽高*/
static uint8_t Album_GetResolution(const char* filePath,uint16_t* w, uint16_t* h)
{
    return LCD_GetMediaSize(filePath,w,h);
}

/*获取图片大小(B)。*/
static uint8_t Album_GetSize(const char *filePath, uint16_t *size)
{
    uint32_t s=0;
    uint8_t ret=LCD_GetFileSize(filePath,&s);
    {
        return ret;
    }
    *size=s/1024;
    return 0;
}

/*******************end*******************/




void Album_MID_Handler()
{
    if (albumHandle.isInit)
    {
        albumHandle.isInit = 0;
        Album_Mid_Name_Show();
    }
    else if (albumHandle.isDispImg) /*显示图片*/
    {
        albumHandle.isDispImg = 0;

        /*通过索引获取图片名*/
        char imgName[32] = { 0 };
        ML_GetFileName_By_Index(albumHandle.index, IMG_DIR, imgName, sizeof(imgName));
        lv_label_set_text(ui_ScrAlbumShow_LabelTitle,imgName); /*修改图片标题*/
        printf("img name:%s\n", imgName);

        char imgPath[48] = { 0 };
        sprintf(imgPath, "%s/%s",IMG_DIR,imgName);
        printf("img path:%s\n", imgPath);


        /*1.更新图片信息面板*/
        /*1.1 获取图片宽高*/
        uint16_t width = 0;
        uint16_t height = 0;
        Album_GetResolution(imgPath,&width, &height);

        /*1.2 获取图片占用空间大小*/
        uint16_t imgSize = 0;
        Album_GetSize(imgPath,&imgSize);

        /*1.3 修改图片信息面板的信息*/
        lv_label_set_text_fmt(ui_ScrAlbumShow_LabelShowInfo, "file name: %s\nfile resolution : %d X %d\nfile size : %d KB",
            imgName, width, height, imgSize);

        /*2. 显示图片*/
        Album_Show(imgPath);
    }
    else if (albumHandle.isSwipeLeft || albumHandle.isSwipeRight)
    {
        if (albumHandle.isSwipeLeft)
        {
            albumHandle.isSwipeLeft = 0;
            printf(" swipe left\n");
            if ((albumHandle.index + 1) == albumHandle.num) return;
            albumHandle.index++;
        }
        else if (albumHandle.isSwipeRight)
        {
            albumHandle.isSwipeRight = 0;
            printf(" swipe right\n");
            if (albumHandle.index==0) return;
            albumHandle.index--;
        }


        /*关闭显示图片信息面板*/
        if (albumHandle.isShowInfo)
        {
            albumHandle.isShowInfo = 0;
            lv_label_set_text(ui_ScrAlbumShow_LabelInfo, LV_SYMBOL_EYE_OPEN); /*将显示图标改为开*/
            lv_obj_add_flag(ui_ScrAlbumShow_ContShowInfo,LV_OBJ_FLAG_HIDDEN); /*隐藏显示面板*/
        }
        albumHandle.isDispImg = 1;
    }
}


/*在列表里显示当前图片文件夹有哪些视频*/
static uint8_t Album_Mid_Name_Show()
{
    lv_fs_dir_t dir;
    lv_fs_res_t res = 0;
    res = lv_fs_dir_open(&dir, IMG_DIR);
    if (res)
    {
        printf("open dir fail:%d\n", res);
        lv_fs_dir_close(&dir);
        return res;
    }

    /*读取目录下的文件*/
    char fileName[32] = { 0 };
    uint16_t img_num = 0;
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
        lv_table_set_cell_value_fmt(ui_ScrAlbum_Table,img_num, 0, LV_SYMBOL_IMAGE"   %s", fileName);
        img_num++;
    }
    albumHandle.num = img_num;

    printf("img_num:%d\n", img_num);

    lv_fs_dir_close(&dir);
    return res;
}
