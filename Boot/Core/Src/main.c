#include "main.h"

#include "tim.h"
#include "usart.h"
#include "app_update.h"
#include "iwdg.h"
void SystemClock_Config(void);


int main(void)
{
	HAL_Init();
	SystemClock_Config();

    UART_Init(115200);
	delay_init(72);

   // IWDG_Init(3000);

    Print_ChipInfo();


    //先读取系统配置
    SysConfInfo_Get();

    // Sys_SoftReset();

    int ret=0;
    uint8_t addrBuf[1024]={0};
	uint16_t times=0;  
	while (1)
	{
        if(times==200)
        {
        times=0;
       // IWDG_Reflesh();

        printf("start to judge update:%d\r\n",sysConfInfo.isUpdate);

        //1.判断是否需要更新
        if(sysConfInfo.isUpdate==1)
        {
            ret=AppUpdate_UpdateFirmware();
            if(ret!=RET_OK)
            {
                LOG_E(ret,"update fail:%d\r\n",ret);
                continue;
            }
            sysConfInfo.isUpdate=0;
            SysConfInfo_Set();  // 写回Flash，确保断电重启后不再重复更新
        }


        //2.开始跳转APP
        SYS_JumpApp(APP_START_ADDR);

        //3.跳转失败


        // STM_FLASH_ReadData(0x800F000 ,addrBuf,sizeof(addrBuf));
        // for(uint16_t i=0;i<sizeof(addrBuf);i++)
        // { 
        //   printf("%02x ",addrBuf[i]);
        // }
        // printf("\r\n");

        // uint8_t data[4]={0x12,0x34,0x56,0x78};
        // STM_FLASH_Write(0x800F000+0x3F0,data,4);
        }

		delay_ms(10);
		times++;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
    printf("err handler\r\n");

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
