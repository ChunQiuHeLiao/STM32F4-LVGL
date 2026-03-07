#include"setting.h"
#include"wifi_app.h"
#include"stm_flash.h"
#include<stdlib.h>

Setting_Handle setHandle={0};
SysUpdateHandle sysUpdateHandle={0};

static void Setting_EventHandler(lv_event_t* e);
static void SysUpdate_EventHandler(lv_event_t* e);

void Setting_SysUpdate();

void SysUpdate_Handler();
void Wifi_Handler();



void SysUpdate_Handler()
{
    if(sysUpdateHandle.isStartCheck)
    {
        sysUpdateHandle.isStartCheck=0;
        /*先判断是否已经下载了更新安装包*/
        if(STM_FLASH_Get_UpdateFlag()==1&&sysUpdateHandle.isExistPackage==0)
        {
            sysUpdateHandle.isExistPackage=1; /*另一边显示UI*/
            return;
        }

        /*1.连接服务器*/
        
        uint8_t ret=WIFI_APP_ConnectServer("TCP",ESP8266_SYS_IP,ESP8266_SYS_PORT);
        if(ret) 
        {
            printf("connect server fail:%d\n",ret);
            sysUpdateHandle.checkErrFlag=ret; /*连接服务器错误*/
            sysUpdateHandle.isOverCheck=1;
            return;
        }

        /*2.使能发送数据。也就是可以和服务器互相发送接收数据了*/
        ret=ESP8266_Cmd_EN_SendData(); /*使能透传发送数据*/
        if(ret) 
        {
            printf("en send data fail:%d\n",ret);
            return;
        }

        /*3.检查系统更新*/
        ret=WIFI_APP_CheckUpdateFlag_FromServer(&sysUpdateHandle.updateFlag);
        if(ret) 
        {
            printf("check update flag fail:%d\n",ret);
            sysUpdateHandle.checkErrFlag=ret; /*连接服务器错误*/
            sysUpdateHandle.isOverCheck=1;
            return;
        }

        /*4.获取安装包数据*/
        ret=WIFI_APP_GetPackageSize(&sysUpdateHandle.pakageSize);
        if(ret) 
        {
            printf("get package size timeout\n");
            return;
        }

        sysUpdateHandle.isOverCheck=1;
    }
    else if(sysUpdateHandle.isStartUpdate) /*开始更新*/
    {
        /*开始从服务器下载安装包*/
        sysUpdateHandle.isStartUpdate=0;
        sysUpdateHandle.isUpdating=1;

        printf("package size:%d\n",sysUpdateHandle.pakageSize);
        uint8_t ret=WIFI_APP_Download_Pack(FIRMWARE_NAME,sysUpdateHandle.pakageSize,&sysUpdateHandle.transByteNum);
        if(ret)
        {
            printf("download fail:%d\n",ret);
            sysUpdateHandle.updateErrFlag=ret;
        }

        WIFI_APP_EXIT_SendData();
        WIFI_APP_DiconnectServer();

        sysUpdateHandle.isUpdating=0; /*更新结束*/
        sysUpdateHandle.isOverUpdate=1;
    }
    else if(sysUpdateHandle.isStartInstall) 
    {
        /*开始安装系统了，这里跳转到bootloader程序里*/
        sysUpdateHandle.isStartInstall=0;
        printf("install over\n");
        FS_API_ScanDir(DRIVER_DIR);
        uint8_t ret=0;
        if(ret==0) ret=SYS_JumpApp(FLASH_BASE); //跳转到bootloader程序
        printf("jump fail=%d\n",ret);
    }
}


void Setting_Handler()
{
    SysUpdate_Handler();
    Wifi_Handler();
}




void Wifi_Handler()
{
    if(setHandle.isOpenWifi==0) return;

    if(setHandle.isReflush==1)
    {
        WIFI_APP_Delay();
        printf("----------------scan wifi ---------------------- \n");
        /*扫描WIFI*/
        setHandle.wifiResponseSize=2048;
        char* rxData=setHandle.wifiResponse;
        uint8_t ret=WIFI_APP_ScanAP(rxData,setHandle.wifiResponseSize);
        if(ret) 
        {
            printf("scan ap fail:%d\n",ret);
            setHandle.isReflush=0;
            setHandle.isOverScanAp=0; /*未获取了wifi响应*/
            return;
        }
        
        printf("rx=%s\n",rxData);

        /*获取连接的wifi 名*/
        ret=WIFI_APP_GetConnWifiName(setHandle.wifiName);
        if(ret==1) Printf("no connect wifi\n");
        else if(ret==2) 
        {
            printf("get connect wifi name timeout:%d\n",ret);
        }

        setHandle.isOverScanAp=1; /*获取了wifi响应*/
        setHandle.isReflush=0;

        printf("scan suc... :%d,%d\n",ret,setHandle.isOverScanAp);
    }
    else if(setHandle.isStartConnWifi)
    {
        setHandle.isStartConnWifi=0;
        WIFI_APP_Delay();

        uint8_t ret=WIFI_APP_ConnectAP(setHandle.pendConnWifiName,setHandle.pendConnWifiPwd); /*连接wifi*/
        if(ret) 
        {
            setHandle.isConnectSuc=0;
            printf("connect ap fail:%d\n",ret); 
        }
        else setHandle.isConnectSuc=1;
        setHandle.isOverConnWifi=1;

        printf("connect wifi:%d\n",ret);
    }
}




void Setting_Init()
{
    sysUpdateHandle.checkTimeout=10000;
}



/*设置APP的*/
void Setting()
{
    lv_obj_t* list = lv_list_create(lv_screen_active());
    lv_obj_set_size(list, scr_w, scr_h);
    lv_obj_set_pos(list, 0, 0);
    lv_list_add_text(list, "setting");
    lv_obj_t* child_label = lv_obj_get_child(list, 0);

    App_SetPageTitle(child_label);
    App_CreateExitBtn(NULL);


    lv_obj_t* btn1 = lv_list_add_button(list, NULL, "WLAN");
    lv_obj_t* btn2 = lv_list_add_button(list, NULL, "BlueTooth");
    lv_obj_t* btn3 = lv_list_add_button(list, NULL, "SD Card");
    lv_obj_t* btn4 = lv_list_add_button(list, NULL, "System Update");
    lv_obj_t* btn5 = lv_list_add_button(list, NULL, "System Information");

    lv_obj_add_event_cb(btn1, Setting_EventHandler, LV_EVENT_CLICKED, (void*)"WLAN");
    lv_obj_add_event_cb(btn2, Setting_EventHandler, LV_EVENT_CLICKED, (void*)"BlueTooth");
    lv_obj_add_event_cb(btn3, Setting_EventHandler, LV_EVENT_CLICKED, (void*)"SD Card");
    lv_obj_add_event_cb(btn4, Setting_EventHandler, LV_EVENT_CLICKED, (void*)"System Update");
    lv_obj_add_event_cb(btn5, Setting_EventHandler, LV_EVENT_CLICKED, (void*)"System Info");
}


static void Setting_EventHandler(lv_event_t* e)
{
    const char* ch = (const char*)lv_event_get_user_data(e);
    if (strcmp(ch, "WALN") == 0)
    {
        printf("WALN");
    }
    else if (strcmp(ch, "BlueTooth") == 0)
    {
        printf("BlueTooth");
    }
    else if (strcmp(ch, "SD Card") == 0)
    {
        printf("SD Card");
    }
    else if (strcmp(ch, "System Update") == 0)
    {
        printf("System Update");
        appFlag.isAppJump = 1;
        Push(&dispHandle, Setting_SysUpdate);
    }
    else if (strcmp(ch, "System Info") == 0)
    {
        printf("System Information");
    }
}



lv_obj_t* check_update_msgbox = NULL; /*更新对话框*/
lv_obj_t* check_over_msgbox = NULL; //显示检测结果 无网，服务器问题，无更新这些


/*********************Setting 1.1*************************/
void Setting_SysUpdate()
{

    //最开始进入的页面
    if (strcmp(switchTaskName, "") == 0)
    {
        lv_obj_t* list = lv_list_create(lv_screen_active());
        lv_obj_set_size(list, scr_w, 0.4 * scr_h);
        lv_obj_set_pos(list, 0, 0);
        lv_list_add_text(list, "System Update");
        lv_obj_t* child_label = lv_obj_get_child(list, 0);

        App_SetPageTitle(child_label);
        App_CreateExitBtn(NULL);


        lv_obj_t* btn1 = lv_list_add_button(list, NULL, "Check Update");
        lv_obj_t* btn2 = lv_list_add_button(list, NULL, "Select History Version");


        //lv_obj_t* title = lv_list_create(lv_screen_active());
        //lv_list_add_text(title, "System Update");
        //lv_obj_set_size(title, scr_w, TITLE_HEIGHT);
        //lv_obj_set_style_bg_color(title, lv_color_hex(screenColor), 0);

        //App_SetPageTitle(title);

        //lv_obj_t* checkBtn = lv_button_create(lv_screen_active());
        //lv_obj_set_align(checkBtn, LV_ALIGN_BOTTOM_MID);
        //lv_obj_t* checkBtnLabel = lv_label_create(checkBtn);
        //lv_label_set_text(checkBtnLabel, "Check Update");


        lv_obj_add_event_cb(btn1, SysUpdate_EventHandler, LV_EVENT_CLICKED, (void*)"Check Update");
        lv_obj_add_event_cb(btn2, SysUpdate_EventHandler, LV_EVENT_CLICKED, (void*)"Search History Version");
    }


    /*1.1检查更新页面*/
    else if (strcmp(switchTaskName, "Check Update") == 0) 
    {
        check_update_msgbox = lv_msgbox_create(lv_screen_active());
        lv_obj_set_size(check_update_msgbox, 0.85 * scr_w, 0.5 * scr_h);
        lv_obj_set_align(check_update_msgbox, LV_ALIGN_CENTER);

        lv_msgbox_add_title(check_update_msgbox, "check update");
        lv_msgbox_add_text(check_update_msgbox, "Please sure open WALN before check update. Check for updates now?");
        lv_obj_t* btn1 = lv_msgbox_add_footer_button(check_update_msgbox, "Confirm");
        lv_obj_t* btn2 = lv_msgbox_add_footer_button(check_update_msgbox, "Cancel");

        /*修改标题标签大小*/
        //lv_obj_t* title_label=lv_msgbox_get_title(msgbox);
        //lv_obj_set_height(title_label, 0.05 * scr_h);
        //lv_obj_set_style_bg_color(title_label, lv_color_hex(screenColor), 0);

        lv_obj_add_event_cb(btn1, SysUpdate_EventHandler, LV_EVENT_CLICKED, (void*)"Check_Confirm");
        lv_obj_add_event_cb(btn2, SysUpdate_EventHandler, LV_EVENT_CLICKED, (void*)"Check_Cancel");
    }
    else if (strcmp(switchTaskName, "Check_Confirm") == 0) /*1.1.2 正在检查页面*/
    {
        lv_obj_t* spinner = lv_spinner_create(lv_screen_active());
        lv_obj_set_size(spinner, 0.2 * scr_w, 0.2 * scr_w);
        lv_obj_set_align(spinner, LV_ALIGN_CENTER);
        lv_spinner_set_anim_params(spinner, 2000, 200);
        
    }
    /*1.2 检查历史版本页面*/
    else if (strcmp(switchTaskName, "Search History Version") == 0) 
    {
        App_CreateExitBtn(NULL);
    }
    
}





static void SysUpdate_EventHandler(lv_event_t* e)
{
    const char* para = (const char*)lv_event_get_user_data(e);
    strcpy(switchTaskName, para);
    if (strcmp(para, "Check Update") == 0)
    {

        appFlag.isPageChange = 1;
    }
    else if (strcmp(para, "Search History Version") == 0)
    {
        appFlag.isAppJump = 1;

    }
    else if (strcmp(para, "Check_Confirm") == 0)
    {
        appFlag.isPageChange = 1;
        lv_msgbox_close_async(check_update_msgbox);
    }
    else if (strcmp(para, "Check_Cancel") == 0)
    {
        lv_msgbox_close_async(check_update_msgbox);
    }
}
