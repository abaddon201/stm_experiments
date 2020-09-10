/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "dac.h"
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "base.h"

#include <cmsis_os2.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern osMessageQueueId_t SDQueueID;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    /* Prevent unused argument(s) compilation warning */
    UNUSED(GPIO_Pin);
    /* NOTE: This function Should not be modified, when the callback is needed,
     the HAL_GPIO_EXTI_Callback could be implemented in the user file
     */
    int a = 0;
    a++;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    __HAL_RCC_TIM1_CLK_ENABLE();
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_FSMC_Init();
    MX_DAC_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();
    MX_SPI2_Init();
    MX_TIM1_Init();
    MX_TIM9_Init();
    MX_TIM12_Init();
    /* USER CODE BEGIN 2 */
    //__HAL_RCC_TIM1_CLK_ENABLE();
    //__HAL_TIM_MOE_ENABLE(&htim1);
    //__HAL_RCC_TIM1_CLK_ENABLE();
    /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
    //uint32_t tmpsmcr = htim1.Instance->SMCR & TIM_SMCR_SMS;
    //__HAL_TIM_ENABLE(&htim1);
    //HAL_TIM_Base_Start(&htim1);
    //Z80 tact
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    //RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM1->PSC = 1;
    // 0 - 7MHz
    // 1 - 3.5MHz
    // 3 - 1.75 MHz

    TIM1->ARR = 23;

    TIM1->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
    TIM1->BDTR |= TIM_BDTR_MOE;

    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

    TIM1->CR1 |= TIM_CR1_CEN;

    TIM1->CCR1 = 12;
    TIM1->CCR2 = 0;

//AY Tact
    RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    //RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM9->PSC = 3;
    // 0 - 7MHz
    // 1 - 3.5MHz
    // 3 - 1.75 MHz

    TIM9->ARR = 23;

    TIM9->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
    TIM9->BDTR |= TIM_BDTR_MOE;

    TIM9->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

    TIM9->CR1 |= TIM_CR1_CEN;

    TIM9->CCR1 = 12;
    TIM9->CCR2 = 0;

    //Z80 int
    RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
    //RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM12->PSC = 839;
    // 0 - 7MHz
    // 1 - 3.5MHz
    // 3 - 1.75 MHz

    TIM12->ARR = 1999;

    TIM12->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
    TIM12->BDTR |= TIM_BDTR_MOE;

    TIM12->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

    TIM12->CR1 |= TIM_CR1_CEN;

    TIM12->CCR1 = 10;
    TIM12->CCR2 = 0;

//  TIM_CtrlPWMOutputs(TIM1, ENABLE);

    //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    //HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    baseInit();
    // TIM_Cmd(TIM1, ENABLE);//Обязательно сначала слейв
    /* USER CODE END 2 */

    /* Init scheduler */
    osKernelInitialize(); /* Call init function for freertos objects (in freertos.c) */
    MX_FREERTOS_Init();
    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
/*
 #define READ_CPLT_MSG      (uint32_t) 1
 #define WRITE_CPLT_MSG     (uint32_t) 2

 void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)


 {

 osMessagePut(SDQueueID, WRITE_CPLT_MSG, osWaitForever);


 }



 //void BSP_SD_ReadCpltCallback()



 void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)


 {


 osMessagePut(SDQueueID, READ_CPLT_MSG, osWaitForever);


 }*/
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM2 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM2) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
