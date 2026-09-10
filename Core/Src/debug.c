#include"debug.h"
#include"usart.h"
#include"app_task.h"
#include"app_update.h"
#include"ota_ab.h"
#include"fs_api.h"
#include"iwdg.h"

static char debugRxBuf[128]={0};

TaskHandle_t debugHandle=NULL;


// cmd:value
typedef struct
{
    const char* cmd; /*通过串口输入的指令*/
    void (*Cmd_CallBack)(char* value); /*回调函数，value为指令后面的值*/
}Debug_Cmd_t;


/*-----------------命令回调函数-----------------*/
void PrintTask_CallBack(char* value);
void UpdateStart_CallBack(char* value);
void OtaConfirm_CallBack(char* value);
void OtaRollback_CallBack(char* value);
void OtaApply_CallBack(char* value);
void ScanDir_CallBack(char* value);
void Reboot_CallBack(char* value);
void OtaInfo_CallBack(char* value);


Debug_Cmd_t debugTable[]=
{
    { "print_task_info",PrintTask_CallBack }, /*打印所有任务栈剩余*/
    { "update_start",   UpdateStart_CallBack }, /*手动进入升级接收状态*/
    { "ota_confirm",    OtaConfirm_CallBack },  /*确认本次升级*/
    { "ota_rollback",   OtaRollback_CallBack }, /*回滚到A区固件*/
    { "ota_apply",      OtaApply_CallBack },    /*写片上备份区+置更新标志, 交给Boot*/
    { "ota_info",       OtaInfo_CallBack },     /*打印升级信息*/
    { "scan_dir",       ScanDir_CallBack },     /*扫描SD卡目录*/
    { "reboot",         Reboot_CallBack },      /*软复位*/
};


static void Debug_Task(void* args);


/// @brief 调试任务初始化
void Debug_Init()
{
    BaseType_t ret=xTaskCreate(Debug_Task,"debug",(192),NULL,2,&debugHandle);
    if(ret==pdFALSE)
    {
        LOG_E(ret,"xTaskCreate");
    }
}


static void Debug_Task(void* args)
{
    char cmd[24]={0};
    char value[8]={0};

    Task_PrintStart();

    while(1)
    {
        /* 调试命令走 printf 那个串口 */
        if(UART_GetRxData(debugRxBuf,sizeof(debugRxBuf),DEBUG_UART_ID)==0)
        {
            uint16_t rxSize=UART_GetRxDataSize(DEBUG_UART_ID);
            if(rxSize>0 && rxSize<sizeof(debugRxBuf)) debugRxBuf[rxSize]='\0';

            /*去掉换行*/
            char* p=debugRxBuf;
            while(*p)
            {
                if(*p=='\r'||*p=='\n') { *p='\0'; break; }
                p++;
            }

            if(strchr(debugRxBuf,':')==NULL)
            {
                strncpy(cmd,debugRxBuf,sizeof(cmd)-1);
            }
            else
            {
                char* token=strtok(debugRxBuf,":");
                if(token) strncpy(cmd,token,sizeof(cmd)-1);
                token=strtok(NULL,":");
                if(token) strncpy(value,token,sizeof(value)-1);
            }

            for(uint16_t i=0;i<sizeof(debugTable)/sizeof(Debug_Cmd_t);i++)
            {
                if(strcmp(cmd,debugTable[i].cmd)==0)
                {
                    debugTable[i].Cmd_CallBack(value);
                    break;
                }
            }

            memset(cmd,0,sizeof(cmd));
            memset(value,0,sizeof(value));
            memset(debugRxBuf,0,sizeof(debugRxBuf));
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


/*-----------------调试命令实现-----------------*/

void PrintTask_CallBack(char* value)
{
    Print_MinHeapSize();
    Task_PrintMinStackSize2(uartRxTaskHandle);
    Task_PrintMinStackSize2(frameParseTaskHandle);
    Task_PrintMinStackSize2(otaTaskHandle);
    Task_PrintMinStackSize2(lvglTaskHandle);
    Task_PrintMinStackSize2(debugHandle);
    Task_PrintMinStackSize2(iwdgTaskHandle);
    Task_PrintMinStackSize2(NULL);
}


void UpdateStart_CallBack(char* value)
{
    updateInfo.isUpdating = 1;
    LOG_I("manual set updating flag\r\n");
}


void OtaConfirm_CallBack(char* value)
{
    if(OTA_Confirm()==RET_OK) LOG_I("ota confirm ok\r\n");
    else LOG_E(-1,"ota confirm fail\r\n");
}


void OtaRollback_CallBack(char* value)
{
    LOG_I("rollback to %s\r\n",OTA_FILE_A);
    OTA_Rollback();
}


void OtaInfo_CallBack(char* value)
{
    OtaSysConf_t sc;

    LOG_I("A:%s\r\n",OTA_FILE_A);
    LOG_I("B:%s\r\n",OTA_FILE_B);
    LOG_I("app base:0x%08x,app size:%lu\r\n",(unsigned)OTA_APP_ADDR,OTA_GetRunningImageSize());
    LOG_I("rx:%lu/%lu,%lu%%,updating:%d\r\n",
        OTA_GetRxSize(),OTA_GetTotalSize(),OTA_GetProgress(),OTA_IsUpdating());

    /* Boot 侧共享配置 */
    if(OTA_SysConf_Get(&sc)==RET_OK)
    {
        LOG_I("boot sysconf: isUpdate:%d,factoryFlag:%d\r\n",sc.isUpdate,sc.factoryFlag);
        LOG_I("              factory:%lu/%08lx\r\n",sc.factoryFirmware.size,sc.factoryFirmware.crc32);
        LOG_I("              update :%lu/%08lx\r\n",sc.updateFireware.size,sc.updateFireware.crc32);
    }
}


void OtaApply_CallBack(char* value)
{
    LOG_I("write save slot & set update flag, then let Boot take over\r\n");
    if(OTA_ApplyFromB()==RET_OK)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        Sys_SoftReset();
    }
    else
    {
        LOG_E(-1,"ota apply fail\r\n");
    }
}


void ScanDir_CallBack(char* value)
{
    if(g_sdMutex!=NULL) xSemaphoreTake(g_sdMutex,portMAX_DELAY);
    FS_API_ScanDir((const uint8_t*)OTA_DIR);
    if(g_sdMutex!=NULL) xSemaphoreGive(g_sdMutex);
}


void Reboot_CallBack(char* value)
{
    LOG_I("reboot\r\n");
    vTaskDelay(pdMS_TO_TICKS(200));
    Sys_SoftReset();
}
