#include"freertos_lvgl.h"

//freertos方面文件
#include"FreeRTOS.h"
#include"task.h"

//lvgl方面文件
#include"lvgl.h"
#include"lv_port_disp_template.h"
#include"lv_port_indev_template.h"


//文件系统和SD卡的
#include"fs_api.h"



#include"touch_lcd.h"
#include"lcd.h"


#include"main_lib.h"
#include"main_screen.h"


//开始任务。用于在里面创造任务
void Task_Start(void* pvParameters);
TaskHandle_t task_startHandle={0}; 
#define TASK_START_STATCK_SIZE 128
#define TASK_START_PRIORITY 1



//定时刷新屏幕。
void Task_ReflushLCD(void* pvParameters);
TaskHandle_t task_reflushLCDHandle={0}; 
#define TASK_REFLUSH_LCD_STATCK_SIZE (1024*2+512) 
#define TASK_REFLUSH_LCD_PRIORITY 12



//播放视频任务(即视频播放器)
void Task_PlayVedio(void* pvParameters);
TaskHandle_t task_playVedioHandle={0}; 
#define TASK_PLAY_VEDIO_STATCK_SIZE 128
#define TASK_PLAY_VEDIO_PRIORITY 2




void Task_Test(void* pvParameters);
TaskHandle_t task_testHandle={0}; 
#define TASK_TEST_STATCK_SIZE 128
#define TASK_TEST_PRIORITY 2





#include"timers.h"

void FL_Init()
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    printf("start\n");
    
    // lv_obj_t* fe=lv_file_explorer_create(lv_screen_active());
    // lv_file_explorer_open_dir(fe, "S:");
    
    MainScreen_Init();
    uint8_t nextTime=0;
    uint8_t a=0;
    while (1)
    {
        Flag_Handler();
        nextTime=lv_timer_handler();
        //if(nextTime==1)
        delay_ms(nextTime);
        a++;
        if(a==200)
        {
            printf("run...\n");
        }
    }


    uint8_t ret=xTaskCreate(Task_Start,"Task_Start",TASK_START_STATCK_SIZE,NULL,TASK_START_PRIORITY,&task_startHandle); 
    if(ret!=pdPASS) printf("Task_Start fail:%d\n",ret);

    vTaskStartScheduler(); //启动调度器（必须调用，否则任务不运行）
}


void Task_Start(void* pvParameters)
{
    printf("enter start task\n");
    //进入临界区，也就相当于关闭中断和任务调度
    taskENTER_CRITICAL(); // 执行需要保护的操作，如创建任务
  
    //返回1成功，-1失败(堆内存不够)

    uint8_t ret=xTaskCreate(Task_ReflushLCD,"Task_ReflushLCD",TASK_REFLUSH_LCD_STATCK_SIZE,NULL,TASK_REFLUSH_LCD_PRIORITY,&task_reflushLCDHandle); 
    if(ret!=pdPASS) printf("Task_ReflushLCD fail:%d\n",ret);
    ret=xTaskCreate(Task_PlayVedio,"Task_PlayVedio",TASK_PLAY_VEDIO_STATCK_SIZE,NULL,TASK_PLAY_VEDIO_PRIORITY,&task_playVedioHandle); 
    if(ret!=pdPASS) printf("Task_PlayVedio fail:%d\n",ret);

    ret=xTaskCreate(Task_Test,"Task_Test",TASK_TEST_STATCK_SIZE,NULL,TASK_TEST_PRIORITY,&task_testHandle); 
    if(ret!=pdPASS) printf("Task_Test fail:%d\n",ret);

    taskEXIT_CRITICAL();

    vTaskDelete(NULL); //释放当前任务
}


uint8_t nextTime=0;
void Task_ReflushLCD(void* pvParameters)
{   
    printf("enter lcd\n");
    while (1)
    {
        //更新屏幕
        Flag_Handler();
        nextTime=lv_timer_handler();
        //if(nextTime==1)
        vTaskDelay(nextTime);
    }
}





void Task_Test(void* pvParameters)
{
    while (1)
    {
        printf("run...%d\n", xPortGetFreeHeapSize());
        vTaskDelay(2000);
    }
}

void Task_PlayVedio(void* pvParameters)
{
    while (1)
    {
        
        // printf("task2\n");
        vTaskDelay(1000);
    }
}
