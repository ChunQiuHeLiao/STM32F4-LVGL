#include"fs_api.h"


//!!!一定要是全局变量呀，它相当于就是整个文件系统的句柄呀。
FATFS fs; // FatFs文件系统的工作区对象，它是FatFs模块的核心数据结构。

/// @brief 挂载盘符，也就打开哪个盘符，看看该盘符在不在
/// @param path 盘符。一般 "0:"
/// @return 0成功
uint8_t FS_API_Init(const uint8_t* path)
{
  //挂载盘符，也就是打开哪个盘符
  
  FRESULT res=0;
  res=f_mount(&fs,path,1);
  if(res)
  {
    printf("f_mount fail=%d\n",res);
    return res;
  }
  return 0;
}



uint8_t FS_API_FormatFileSystem(const uint8_t* path)
{
    //1.卸载当前盘符
    FRESULT ret=0; 
    ret=f_mount(NULL,path,0);
    if(ret)
    {
        printf("uninstall fail:%d\n",ret);
        return 1;
    }

    //2.开始格式化
    printf("start format,please wait 5s more than\n");

    MKFS_PARM opt={0};
    opt.fmt=FM_FAT32;
    uint8_t work[512]={0};
    ret=f_mkfs(path,&opt,work,sizeof(work));
    if(ret)
    {
        printf("format fail:%d\n",ret);
        return 2;
    }
    printf("format suc\n");

    return 0;
}




/// @brief 读取文件的数据
/// @param filePath 文件路径
/// @param data 读取的数据
/// @param size 读取的实际文件数据大小
/// @param startReadPos 起始读取文件的位置，也就是从文件哪里开始读
/// @return 0成功
uint8_t FS_API_Read(const uint8_t* filePath,uint8_t* data,uint16_t* size,uint32_t startReadPos)
{
    FRESULT res=0; 

    //2.打开文件
    FIL fp;
    res=f_open(&fp,filePath,FA_READ);
    if(res)
    {
        printf("f_open fail=%d\n",res);
        return res;
    }

    //读取指针变更
    res=f_lseek(&fp,startReadPos);
    if(res)
    {
        printf("f_lseek fail=%d\n",res);
        return res;
    }


    //3.读取数据到文件
    UINT readNum=0;
    res=f_read(&fp,data,*size,&readNum);
    if(res)
    {
        printf("f_read fail=%d,readNum size=%d\n",res,readNum);
        return res;
    }
    *size=readNum;

    f_close(&fp);

    return 0;
}


/// @brief 将数据写入文件末尾
/// @param filePath 文件路径
/// @param data 写入的数据
/// @param size 数据大小
/// @return 0成功
uint8_t FS_API_Write(const uint8_t* filePath,uint8_t* data,uint16_t size)
{
    FRESULT res=0; 

    //2.打开文件
    FIL fp;
    res=f_open(&fp,filePath,FA_WRITE|FA_OPEN_APPEND);
    if(res)
    {
        printf("f_open fail=%d\n",res);
        return res;
    }

    //3.写入数据到文件
    UINT writeNum=0;
    res=f_write(&fp,data,size,&writeNum);
    if(res)
    {
        printf("f_write fail=%d,write size=%d\n",res,writeNum);
        return res;
    }
    
    // 关键：定期刷新缓冲区
    // res=f_sync(&fp); 
    // if(res)
    // {
    //     printf("f_sync fail=%d,write size=%d\n",res,writeNum);
    //     return res;
    // }

    f_close(&fp);

    return 0;
}




/// @brief 通过文件路径来获取该文件的 读写 文件指针
/// @param filePath 文件路径
/// @param fp 获取的文件指针
/// @param mode 获取的是读还是写文件指针。0读的，1写的
/// @return 0成功
uint8_t FS_API_Get_FP(const uint8_t* filePath,FIL* fp,uint8_t mode)
{ 
    FRESULT res=0; 
    BYTE open_mode=0;
    if(mode==1) open_mode=FA_WRITE|FA_OPEN_APPEND;
    else open_mode=FA_READ;
    
    //打开文件    
    res=f_open(fp,filePath,open_mode);
    if(res)
    {
        return res;
    }

    return 0;
}


/// @brief (FP=File Point)通过文件指针来把数据写入知道文件。writeData如果填入NULL，则会关闭该文件
/// @param fp 文件指针
/// @return 0写入成功
uint8_t FS_API_Write_By_FP(FIL* fp,const uint8_t* writeData,uint16_t size)
{
    FRESULT res=0; 
    
    //写入数据到文件
    UINT writeNum=0;
    res=f_write(fp,writeData,size,&writeNum);
    if(res)
    {
        printf("f_write fail=%d,write size=%d\n",res,writeNum);
        return res;
    }

    // 关键：定期刷新缓冲区。频繁刷新，太占CPU了
    // res=f_sync(fp); 
    // if(res)
    // {
    //     printf("f_sync fail=%d,write size=%d\n",res,writeNum);
    //     return res;
    // }

    return 0;
}


/// @brief (FP=File Point)通过文件指针来把数据写入知道文件。writeData如果填入NULL，则会关闭该文件
/// @param fp 文件指针
/// @return 0写入成功
uint8_t FS_API_Read_By_FP(FIL* fp,uint8_t* readData,uint16_t size,uint32_t* readSize)
{
    if(size==0) 
    {
        *readSize=0;
        return 0;
    }
    FRESULT res=0; 
    //读取数据从文件
    //UINT readNum=0;
    res=f_read(fp,readData,size,(UINT*)readSize);
    if(res)
    {
        printf("f_read fail=%d,read size=%d\n",res,*readSize);
        return res;
    }

    return 0;
}



uint8_t FS_API_Close(FIL* fp)
{
    FRESULT res=0;
    res=f_close(fp);
    if(res) printf("file close fail:%d\n",res);
    return res;
}


/// @brief 删除文件
/// @param filePath 文件路径 
/// @return 0成功
uint8_t FS_API_Delete(const char* filePath)
{
    uint8_t ret=f_unlink(filePath);
    if(ret)
    {
        PrintErr(ret);
        return ret;
    }

    return 0;
}


/// @brief 扫描指定目录下的文件
/// @param path 扫描的目录
/// @return 0成功，其它失败
uint8_t FS_API_ScanDir(const uint8_t* path)
{
    FRESULT res=0;  

    //2.打开指定盘符的指定目录
    DIR dir={0};
    res=f_opendir(&dir,(const TCHAR*)path);
    if(res)
    {
        printf("f_opendir fail=%d\n",res);
        return res;
    }

    //3.获取目录下的所有文件()
    FILINFO fileInfo; //存储当前读取的文件或文件夹信息
    while (1)
    {
        res=f_readdir(&dir,&fileInfo);
        if(res!=0) return res; //读取目录异常
        if(fileInfo.fname[0]==0)  //读到了文件末尾
        {
            printf("read dir over\n");
            break; 
        }

        if(fileInfo.fattrib==AM_DIR) //判断是不是目录
        {
            printf("%s %dB dir\n",fileInfo.fname,fileInfo.fsize);
        }
        else
        {
            printf("%s  %dB  file\n",fileInfo.fname,fileInfo.fsize);
        }
    }
    
    //4.关闭目录
    f_closedir(&dir);   

    return 0;
}



/// @brief 从文件路径中得到盘符
/// @param path 盘符
/// @param filePath 文件路径
void FS_API_FilePathToPath(uint8_t* path,const uint8_t* filePath)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        if(filePath[i]==':') break;
    }

    for(uint8_t j=0;j<=i;j++) path[j]=filePath[j];
}


/// @brief 获取文件大小
/// @param filePath 文件路径
/// @return 负数为获取失败
long FS_API_GetFileSize(const uint8_t* filePath)
{
    FRESULT res=0; 
    FILINFO fileInfo;

    res=f_stat(filePath,&fileInfo);
    if(res) return (-res);

    return fileInfo.fsize;
}



uint8_t FS_API_GetImgSize(const uint8_t* imgPath,uint16_t* width,uint16_t* height)
{
    FIL fp;
    uint8_t ret=0;
    ret=FS_API_Get_FP(imgPath,&fp,0);
    if(ret) return ret;

    uint32_t readSize=0;
    uint8_t readData[4]={0}; //先是宽，再是高，都是小端的，每个2字节
    ret=FS_API_Read_By_FP(&fp,readData,4,&readSize);
    if(ret) 
    {
        return ret;
    }

    *width=((uint16_t)readData[1]<<8)+readData[0];
    *height=((uint16_t)readData[3]<<8)+readData[2];

    ret=FS_API_Close(&fp);

    return ret;
}
