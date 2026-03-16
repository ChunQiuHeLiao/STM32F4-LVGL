#include"main_lib.h"


DispHandle dispHandle = { 0 };
AppFlag appFlag = { 0 };


int screenColor = 0xffffff;

char switchTaskName[64] = ""; /*主要用于页面切换*/

uint16_t scr_w=SCR_WIDTH;
uint16_t scr_h=SCR_HEIGHT;


void StackInit(DispHandle* dispHandle)
{
    dispHandle->capacity = 0;
    //dispHandle->dispFunc = (DispFunc*)malloc(sizeof(DispFunc*));
}

void Push(DispHandle* dispHandle, DispFunc func)
{
    //DispFunc* tmp=(DispFunc*)realloc(dispHandle->dispFunc,sizeof(DispFunc*)*(dispHandle->capacity+1));
    //dispHandle->dispFunc=tmp;
    dispHandle->dispFunc[dispHandle->capacity] = func;
    dispHandle->capacity++;
}


void Pop(DispHandle* dispHandle)
{
    dispHandle->capacity--;
}


bool Util_IsEndWith(const char* str1, const char* str2)
{
    if (str1 == NULL || str2 == NULL)
        return false;

    uint16_t len1 = lv_strlen(str1);
    uint16_t len2 = lv_strlen(str2);
    if ((len1 < len2) || (len1 == 0 || len2 == 0))
        return false;

    while (len2 >= 1) {
        if (str2[len2 - 1] != str1[len1 - 1])
            return false;

        len2--;
        len1--;
    }

    return true;
}


#if LV_USE_GUIDER_SIMULATOR==0

/// @brief 设置显示是横屏还是竖屏
/// @param dir 0:竖屏 1:横屏
void Screen_SetShowDir(uint8_t dir)
{
    LCD_SetShowDir(dir);
    appFlag.isSwitchShowDir=dir;
    lv_display_t* disp=lv_display_get_default();
    if(dir==0) {scr_w=SCR_WIDTH;scr_h=SCR_HEIGHT;}
    else if(dir==1) {scr_w=SCR_HEIGHT;scr_h=SCR_WIDTH;}
    lv_display_set_resolution(disp,scr_w,scr_h);
}

#endif



/*通过索引来获取文件名。适合同一类型文件*/
/*fileName:得到的文件名*/
uint8_t ML_GetFileName_By_Index(uint16_t index,const char* filePath,char* fileName,uint16_t size)
{
    lv_fs_dir_t dir;
    lv_fs_res_t res = 0;
    res = lv_fs_dir_open(&dir, filePath);
    if (res)
    {
        printf("open dir fail:%d\n", res);
        lv_fs_dir_close(&dir);
        return res;
    }

    /*读取目录下的文件*/
    lv_obj_t* btn = NULL;
    uint16_t img_index = 0;
    while (1)
    {
        res = lv_fs_dir_read(&dir, fileName, size);
        if (res)
        {
            printf("read dir fail:%d\n", res);
            break;
        }

        if (fileName[0] == 0) break; /*代表该目录文件读取完毕,获取判断该数组长度为0*/
        img_index++;
        if ((img_index - 1) == index) break;
    }

    lv_fs_dir_close(&dir);
    return res;
}




/// @brief 通过文件索引获取指定目录下该文件的路径。一般用于 音乐，视频，图片目录用
/// @param index 文件索引，也就是文件的默认排序方式
/// @param filePath 输入输出参数。输入该文件所在目录，输出该文件路径
/// @return 0 成功
uint8_t ML_GetFilePath_By_Index(uint16_t index, const char* filePath)
{
    /*通过索引获取文件名*/
    char imgName[32] = { 0 };
    uint8_t ret=0;
    ret=ML_GetFileName_By_Index(index, filePath,imgName,sizeof(imgName));
    if(ret) 
    {
        printf("get file path fail:%d\n",ret);
        return ret;
    }

    char imgDir[32] = { 0 };
    strcpy(imgDir,filePath); 

    sprintf(filePath,"%s/%s",imgDir,imgName);
    
    return 0;
}



/// @brief 会主动分配字符串内存，并完成复制字符串操作。
/// @param str 字符指针变量地址
/// @param size 复制的字符串的大小(函数内部会补一个\0 ,也就是大小+1)
/// @param value 待复制的字符串
/// @return 成功返回为字符指针分配的地址，失败返回NULL
char* MyStrCpy(char** str, uint8_t size, char* value)
{
    if (*str != NULL) lv_free(*str);

    *str = (char*)lv_malloc(sizeof(char) * (size + 1)); /*加1是为了/0*/
    if (*str == NULL)
    {
        printf("malloc fail\n");
        return NULL;
    }
    strcpy(*str, value);
    //printf("cp=%s,cp1=%s\n", *str,weatherHandle.realTimeInfo.text);
    memset(value, 0, size);

    return *str;
}



/*刷新一遍UI*/
void ML_RefreshUI()
{
    while (1)
    {
        if (lv_timer_handler() == LV_DEF_REFR_PERIOD)break;
        lv_delay_ms(10);
    }
}

// 打印 LVGL 内存占用信息
void print_lvgl_mem_usage(void)
{
    lv_mem_monitor_t mon;  // 内存监控结构体
    lv_mem_monitor(&mon);  // 获取内存数据
    
    // 打印核心参数（单位：字节，可换算为KB/MB）
    printf("===== LVGL 内存占用 =====\n");
    printf("已使用内存：%d B\n", mon.total_size-mon.free_size);
    printf("分配最大内存：%d B\n", mon.total_size);
    printf("=========================\n");
}


uint8_t lv_fs_delete(const char* filePath)
{
    int ret=remove(filePath);
    if (ret) perror("REMOVE");
    return 0;
}