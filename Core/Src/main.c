
#include"main.h"
/*lvgl*/
#include"lvgl.h"
#include"lv_port_disp_template.h"
#include"lv_port_indev_template.h"

/*UI 工具的文件*/
#include"ui.h"


/*文件系统*/
#include"fs_api.h"

/*用于作为lvgl时钟源*/
#include"tim.h"

/*WIFI */
#include"wifi_app.h"

/*音频*/
#include"audio_player.h"
#include"audio_recording.h"

// #define SAMPLE_BUF_SIZE 9600
// #define RX_BUF_SIZE 9600

// /*每次采样音频数据的缓存区大小*/
// uint8_t buf[SAMPLE_BUF_SIZE]={0}; /**/
// uint8_t buf1[SAMPLE_BUF_SIZE]={0}; /**/
// uint32_t rxBuf[RX_BUF_SIZE]={0};

void SystemClock_Config(void);
void Music_Play_AP(const char* filePath);
lv_ui guider_ui;
#define EN_LVGL_INIT 

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  delay_init(100);
  UART2_Init(115200);
  WIFI_APP_Init(115200); /*初始化WIFI模块*/

  uint8_t a=8;
  while(1)
  {
    delay_ms(1000);
    ///--;
    if(a==0) break;
    printf("wait00...\n");
  }
  
  
  TIM_Base_Init(100-1,1000-1); //作为lvgl的时钟源
  
  FS_API_Init(DRIVER_DIR); /*初始化盘符*/
  FS_API_ScanDir(DRIVER_DIR);

  Audio_Player_Init(I2S_AUDIOFREQ_48K);
  Audio_Recording_Init(I2S_AUDIOFREQ_16K);

  // LCD_Init(); //显示屏初始化
  // //显示图片
  // char fileName[16]={0};
  
  // for(uint8_t i=1;i<8;i++)
  // {
  //   sprintf(fileName,"S:/img/baizi_%d.bjpg",i);
  //   LCD_Clear_All(WHITE);
  //   LCD_DispImg(fileName);
  //   HAL_Delay(1000);
  // }
  // LCD_Clear_All(BLACK);
  // Vedio_Disp("S:/vedio/vedio5.bmp4",NULL);
  // Vedio_Disp("S:/vedio/dm.bmp4",NULL);


  /*初始化lvgl*/
  #ifdef EN_LVGL_INIT
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  /*初始化lvgl图形化工具的文件*/
  ui_init();
  #endif
  



  uint16_t timeout=0;
  uint8_t nextTime=0;
  while(1)
  {
    Flag_Handler();
    nextTime=lv_timer_handler();
    delay_ms(nextTime);
    //HAL_Delay(10);

    timeout++;

    if(timeout==200)
    {
      timeout=0;
      printf("running...\n");
    }
  }
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  // RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  // RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // /** Configure the main internal regulator output voltage
  // */
  // __HAL_RCC_PWR_CLK_ENABLE();
  // __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // /** Initializes the RCC Oscillators according to the specified parameters
  // * in the RCC_OscInitTypeDef structure.
  // */
  // RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  // RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  // RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  // RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  // RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  // RCC_OscInitStruct.PLL.PLLM = 8;
  // RCC_OscInitStruct.PLL.PLLN = 100;
  // RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  // RCC_OscInitStruct.PLL.PLLQ = 5;
  // if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  // {
  //   Error_Handler();
  // }

  // /** Initializes the CPU, AHB and APB buses clocks
  // */
  // RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
  //                             |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  // RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  // RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  // RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  // RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  // if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  // {
  //   Error_Handler();
  // }



  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  // RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  // RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  // RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  // RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  // RCC_OscInitStruct.PLL.PLLM = 12;
  // RCC_OscInitStruct.PLL.PLLN = 96;
  // RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  // RCC_OscInitStruct.PLL.PLLQ = 5;
  // if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  // {
  //   Error_Handler();
  // }

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

  /** Initializes the CPU, AHB and APB buses clocks
  */
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
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
