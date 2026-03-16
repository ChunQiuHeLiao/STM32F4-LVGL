#include"util_mid.h"


/*刷新一下表格内容。使表格显示指定目录所有文件名，需要用到读写SD卡函数*/
uint8_t UtilMid_RefleshFileTable(lv_obj_t* table, const char* fileDir,uint16_t* fileNum)
{
    lv_table_set_row_count(table, 0);
    lv_fs_dir_t dir;
    lv_fs_res_t res = 0;
    res = lv_fs_dir_open(&dir, fileDir);
    if (res)
    {
        printf("open dir fail:%d\n", res);
        return res;
    }

    /*读取目录下的文件*/
    char fileName[32] = { 0 };

    uint16_t num = 0; /*初始化歌曲数为0*/
    while (1)
    {
        res = lv_fs_dir_read(&dir, fileName, sizeof(fileName));
        if (res)
        {
            printf("read dir fail:%d\n", res);
            break;
        }

        if (fileName[0] == 0) break; /*代表该目录文件读取完毕,获取判断该数组长度为0*/


        /*把文件内容加载到表格里面*/
        lv_table_set_cell_value_fmt(table, num, 0, "%s", fileName);
        lv_table_set_cell_value_fmt(table, num, 1, LV_SYMBOL_CLOSE);

        num++;
    }

    if (res == LV_FS_RES_OK)
    {
        lv_fs_dir_close(&dir);
    }

    printf("file num:%d\n",num);

    *fileNum = num;
    return res;
}



/*刷新一下表格形成的列表内容。列表显示文字图标和文字，row代表显示在第几行*/
void UtilMid_RefleshList(lv_obj_t* table, const char* symbol, const char* str, uint16_t row)
{
    /*把文件内容加载到表格里面*/
    lv_table_set_cell_value_fmt(table, row, 0,"%s  %s",symbol,str);
    //lv_table_set_cell_value_fmt(table, row, 1, str);
}
