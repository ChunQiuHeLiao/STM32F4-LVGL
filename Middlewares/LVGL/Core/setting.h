
#ifndef __SETTING_H
#define __SETTING_H

#ifdef __cplusplus
extern "C" {
#endif


#include"main_lib.h"


#define FIRMWARE_NAME "S:MyFirmware.bin"



typedef struct 
{
    /*WIFI 部分*/
    uint8_t isOpenWifi:1;
    uint8_t isConnectSuc:1; /*连接wifi是否 成功*/

    uint8_t isExitWifi:1;
    uint8_t isReflush:1; /*点了刷新按钮 或者自动刷新时间到了*/
    uint8_t isExitPwd:1; /*退出WIFI 输入密码 界面*/

    uint8_t isOverScanAp; /*是否扫描成功WIFI*/
    uint8_t isStartConnWifi; /*开始连接wifi*/
    uint8_t isOverConnWifi;

    uint16_t wifiResponseSize;
    char wifiResponse[2048]; /*获取wifi的内容指针。比如通过此获取wifi响应内容*/
   

    char* pendConnWifiName;
    char* pendConnWifiPwd;
    char wifiName[32]; /*已连接的wifi名*/
    uint8_t isOpenBlueTooth:1;

}Setting_Handle;



/*系统更新部分*/
typedef struct 
{
    uint8_t isExistPackage:1; /*是否已经下载了安装包，但是还没更新*/

    uint8_t isStartCheck:1; /*开始检查是否需要更新*/
    uint8_t isOverCheck:1; /*开始检查是否需要更新*/
    uint16_t checkTimeout; /*检查更新超时的时间的*/
    uint8_t checkErrFlag; /*1:连接服务器错误 2：连接服务器超时*/

    uint8_t isStartUpdate:1; /*开始更新*/
    uint8_t isUpdating:1; /*正在更新中，也就是在下载安装包中*/
    uint8_t isOverUpdate:1; /*更新结束*/
    uint32_t transByteNum; /*更新过程中已传输的字节数*/

    uint8_t isStartInstall:1; /*代表要开始安装系统安装包了*/

    uint8_t updateFlag; /*1:需要更新，0无需*/
    uint8_t updateErrFlag; /*更新过程中产生的错误。1:链接如果网络没了,通讯异常等*/

    uint32_t pakageSize; /*安装包大小*/
}SysUpdateHandle;

extern Setting_Handle setHandle;
extern SysUpdateHandle sysUpdateHandle;



void Setting_Init();
void Setting_Handler();

#ifdef __cplusplus
}
#endif
#endif
