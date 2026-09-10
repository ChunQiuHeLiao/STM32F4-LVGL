
#include"main.h"

/*文件系统*/
#include"fs_api.h"

/*用于作为lvgl时钟源*/
#include"tim.h"

/*音频*/
#include"audio_recording.h"

/*WIFI + OTA + FreeRTOS 任务*/
#include"wifi_app.h"
#include"ota_ab.h"
#include"app_task.h"
#include"debug.h"
#include"iwdg.h"

#include"spi.h"

void SystemClock_Config(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  delay_init(100);
  UART2_Init(115200);        /*printf / 调试命令串口*/
  UART_Init(115200);         /*ESP8266 / OTA 串口*/

  TIM_Base_Init(100-1,1000-1); //作为lvgl的时钟源(1ms)

  /*==============================================================
   * BootLoader 阶段(复位后最先执行):
   *   读取升级状态 -> 烧写新固件 / 等待确认 / 失败回滚
   *==============================================================*/
  OTA_Boot_Run();

  FS_API_Init((const uint8_t*)DRIVER_DIR);   /*挂载盘符*/
  FS_API_ScanDir((const uint8_t*)DRIVER_DIR);

  /*看门狗 8s, 升级/运行异常时可自动复位并触发回滚*/
  IWDG_Init(8000);

  Audio_Recording_Init(I2S_AUDIOFREQ_16K);

  /*==============================================================
   * 创建 FreeRTOS 任务(串口接收/协议解析/OTA/LVGL/调试/看门狗)
   *==============================================================*/
  App_Task_Init();

  /*启动调度器, 之后永不返回*/
  vTaskStartScheduler();

  while(1)
  {
    IWDG_Reflesh();
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState=RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
