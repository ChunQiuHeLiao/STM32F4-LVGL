#ifndef __FS_API_H
#define __FS_API_H

#include"main.h"
#include"ff.h"
#include"sdio.h"



uint8_t FS_API_Init(const uint8_t* path);
uint8_t FS_API_FormatFileSystem(const uint8_t* path);
uint8_t FS_API_Read(const uint8_t* filePath,uint8_t* data,uint16_t* size,uint32_t startReadPos);
uint8_t FS_API_Write(const uint8_t* filePath,uint8_t* data,uint16_t size);


uint8_t FS_API_Get_FP(const uint8_t* filePath,FIL* fp,uint8_t mode);
uint8_t FS_API_Write_By_FP(FIL* fp,const uint8_t* writeData,uint16_t size);
uint8_t FS_API_Read_By_FP(FIL* fp,uint8_t* readData,uint16_t size,uint32_t* readSize);


uint8_t FS_API_ScanDir(const uint8_t* path);
uint32_t FS_API_GetFileSize(const uint8_t* filePath);
uint8_t FS_API_GetImgSize(const uint8_t* imgPath,uint16_t* width,uint16_t* height);

uint8_t FS_API_Close(FIL* fp);

void FS_API_FilePathToPath(uint8_t* path,const uint8_t* filePath);
#endif