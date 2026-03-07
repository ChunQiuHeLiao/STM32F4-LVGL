#include"mid/album_mid.h"


//void Album_MID_Init();
static uint8_t Album_Mid_Name_Show();


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

        FS_API_GetImgSize(imgPath,&width,&height);
        
        printf("w1\n");
        /*1.2 获取图片占用空间大小*/
        uint32_t imgSize = 0;
        LCD_GetFileSize(imgPath,&imgSize);
        printf("w2\n");
        

        /*1.3 修改图片信息面板的信息*/
        lv_label_set_text_fmt(ui_ScrAlbumShow_LabelShowInfo, "file name: %s\nfile resolution : %d X %d\nfile size : %dKB",
            imgName, width, height, imgSize/1024);

        /*2. 显示图片*/
        printf("start img disp\n");
        LCD_DispImg(imgPath);



        /*获取图片信息*/
        // strcpy(albumHanle.name,fileName);

        //uint32_t size = 0;
        //LCD_GetFileSize(filePath, &size);
        //albumHanle.size = size / 1024.0f;

        //LCD_GetMediaSize(filePath, &albumHanle.w, &albumHanle.h); /*获取图片宽高*/

        ///*先刷新*/
        //uint8_t waitTime = lv_timer_handler();
        //delay_ms(waitTime);

        //LCD_DispImg(filePath);


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


/*在图片显示列表添加图片*/
//static void Album_Mid_AddList(const char* fileName)
//{
//    ui_ScrAlbum_ContImg = lv_obj_create(ui_ScrAlbum_PanelBody);
//    lv_obj_remove_style_all(ui_ScrAlbum_ContImg);
//    lv_obj_set_width(ui_ScrAlbum_ContImg, lv_pct(90));
//    lv_obj_set_height(ui_ScrAlbum_ContImg, lv_pct(10));
//    lv_obj_set_align(ui_ScrAlbum_ContImg, LV_ALIGN_CENTER);
//    lv_obj_remove_flag(ui_ScrAlbum_ContImg, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
//    lv_obj_set_style_radius(ui_ScrAlbum_ContImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
//    lv_obj_set_style_border_color(ui_ScrAlbum_ContImg, lv_color_hex(0xF6F6F6), LV_PART_MAIN | LV_STATE_DEFAULT);
//    lv_obj_set_style_border_opa(ui_ScrAlbum_ContImg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
//    lv_obj_set_style_border_width(ui_ScrAlbum_ContImg, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
//    lv_obj_set_style_border_side(ui_ScrAlbum_ContImg, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    ui_ScrAlbum_BtnImg = lv_button_create(ui_ScrAlbum_ContImg);
//    lv_obj_set_width(ui_ScrAlbum_BtnImg, lv_pct(100));
//    lv_obj_set_height(ui_ScrAlbum_BtnImg, lv_pct(100));
//    lv_obj_set_align(ui_ScrAlbum_BtnImg, LV_ALIGN_LEFT_MID);
//    lv_obj_set_flex_flow(ui_ScrAlbum_BtnImg, LV_FLEX_FLOW_ROW);
//    lv_obj_set_flex_align(ui_ScrAlbum_BtnImg, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//    lv_obj_add_flag(ui_ScrAlbum_BtnImg, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
//    lv_obj_remove_flag(ui_ScrAlbum_BtnImg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
//    lv_obj_set_style_radius(ui_ScrAlbum_BtnImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
//    lv_obj_set_style_bg_color(ui_ScrAlbum_BtnImg, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
//    lv_obj_set_style_bg_opa(ui_ScrAlbum_BtnImg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    ui_ScrAlbum_LabelImgSymbol = lv_label_create(ui_ScrAlbum_BtnImg);
//    lv_obj_set_width(ui_ScrAlbum_LabelImgSymbol, LV_SIZE_CONTENT);   /// 1
//    lv_obj_set_height(ui_ScrAlbum_LabelImgSymbol, LV_SIZE_CONTENT);    /// 1
//    lv_obj_set_align(ui_ScrAlbum_LabelImgSymbol, LV_ALIGN_LEFT_MID);
//    lv_label_set_text(ui_ScrAlbum_LabelImgSymbol,LV_SYMBOL_IMAGE);
//    lv_obj_set_style_text_color(ui_ScrAlbum_LabelImgSymbol, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
//    lv_obj_set_style_text_opa(ui_ScrAlbum_LabelImgSymbol, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    ui_ScrAlbum_LabelImgName = lv_label_create(ui_ScrAlbum_BtnImg);
//    lv_obj_set_width(ui_ScrAlbum_LabelImgName, LV_SIZE_CONTENT);   /// 1
//    lv_obj_set_height(ui_ScrAlbum_LabelImgName, LV_SIZE_CONTENT);    /// 1
//    lv_obj_set_align(ui_ScrAlbum_LabelImgName, LV_ALIGN_RIGHT_MID);
//    lv_label_set_text(ui_ScrAlbum_LabelImgName, fileName);
//    lv_obj_set_style_text_color(ui_ScrAlbum_LabelImgName, lv_color_hex(0x020202), LV_PART_MAIN | LV_STATE_DEFAULT);
//    lv_obj_set_style_text_opa(ui_ScrAlbum_LabelImgName, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
//    lv_obj_set_style_text_font(ui_ScrAlbum_LabelImgName, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    lv_obj_add_event_cb(ui_ScrAlbum_BtnImg, ui_event_ScrAlbum_BtnImg, LV_EVENT_ALL, NULL); /*添加点击按钮事件*/
//}



/*在列表里显示当前图片文件夹有哪些视频*/
static uint8_t Album_Mid_Name_Show()
{
    lv_fs_dir_t dir;
    lv_fs_res_t res = 0;
    res = lv_fs_dir_open(&dir, IMG_DIR);
    if (res)
    {
        printf("open dir fail:%d\n", res);
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

        print_lvgl_mem_usage();
        /*把图片名加载到相册列表中*/
        lv_table_set_cell_value_fmt(ui_ScrAlbum_Table,img_num, 0, LV_SYMBOL_IMAGE"   %s", fileName);
        img_num++;
    }
    albumHandle.num = img_num;

    printf("img_num:%d\n", img_num);

    lv_fs_dir_close(&dir);
    return res;
}
