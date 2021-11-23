/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_ITERATION (10000)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;

osThreadId serialPrintHandle;
osThreadId counter1Handle;
osThreadId counter2Handle;
osThreadId counter3Handle;
osThreadId counter4Handle;
/* USER CODE BEGIN PV */
uint32_t counter1;
uint32_t counter2;
uint32_t counter3;
uint32_t counter4;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
void StartSerialPrint(void const * argument);
void StartCounter1(void const * argument);
void StartCounter2(void const * argument);
void StartCounter3(void const * argument);
void StartCounter4(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of serialPrint */
  osThreadDef(serialPrint, StartSerialPrint, osPriorityNormal, 0, 256);
  serialPrintHandle = osThreadCreate(osThread(serialPrint), NULL);

  /* definition and creation of counter1 */
  osThreadDef(counter1, StartCounter1, osPriorityIdle, 0, 128);
  counter1Handle = osThreadCreate(osThread(counter1), NULL);

  /* definition and creation of counter2 */
  osThreadDef(counter2, StartCounter2, osPriorityLow, 0, 128);
  counter2Handle = osThreadCreate(osThread(counter2), NULL);

  /* definition and creation of counter3 */
  osThreadDef(counter3, StartCounter3, osPriorityAboveNormal, 0, 128);
  counter3Handle = osThreadCreate(osThread(counter3), NULL);

  /* definition and creation of counter4 */
  osThreadDef(counter4, StartCounter4, osPriorityRealtime, 0, 128);
  counter4Handle = osThreadCreate(osThread(counter4), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

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
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartSerialPrint */
/**
 * @brief  Function implementing the serialPrint thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSerialPrint */
void StartSerialPrint(void const * argument)
{
  /* USER CODE BEGIN 5 */
	char buf[192] = { 0 };
	uint32_t time;
	/* Infinite loop */
	for (;;) {
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, SET);
		time = xTaskGetTickCount() / 1000;
		sprintf(buf,
				"Time:                 %6lu sec\r\nTask1 (idle):         %6lu times\r\nTask2 (low):          %6lu times\r\nTask3 (above normal): %6lu times\r\nTask4 (realtime):     %6lu times\r\n\n",
				time, counter1, counter2, counter3, counter4);
		while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
		HAL_UART_Transmit_DMA(&huart2, (uint8_t*) buf, strlen(buf));
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, RESET);
		osDelay(1000);
	}
	vTaskDelete(NULL);
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartCounter1 */
/**
 * @brief Function implementing the counter1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCounter1 */
void StartCounter1(void const * argument)
{
  /* USER CODE BEGIN StartCounter1 */
	float n;
	/* Infinite loop */
	for (;;) {
		for (size_t i = 0; i < MAX_ITERATION; i++) {
//			__NOP();
			n = 22 / 7;
		}

		counter1++;
		osDelay(1);
	}
	vTaskDelete(NULL);
  /* USER CODE END StartCounter1 */
}

/* USER CODE BEGIN Header_StartCounter2 */
/**
 * @brief Function implementing the counter2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCounter2 */
void StartCounter2(void const * argument)
{
  /* USER CODE BEGIN StartCounter2 */
	float n;
		/* Infinite loop */
		for (;;) {
			for (size_t i = 0; i < MAX_ITERATION; i++) {
	//			__NOP();
				n = 22 / 7;
			}

			counter2++;
			osDelay(1);
		}
	vTaskDelete(NULL);
  /* USER CODE END StartCounter2 */
}

/* USER CODE BEGIN Header_StartCounter3 */
/**
 * @brief Function implementing the counter3 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCounter3 */
void StartCounter3(void const * argument)
{
  /* USER CODE BEGIN StartCounter3 */
	float n;
		/* Infinite loop */
		for (;;) {
			for (size_t i = 0; i < MAX_ITERATION; i++) {
	//			__NOP();
				n = 22 / 7;
			}

			counter3++;
			osDelay(1);
		}
	vTaskDelete(NULL);
  /* USER CODE END StartCounter3 */
}

/* USER CODE BEGIN Header_StartCounter4 */
/**
 * @brief Function implementing the counter4 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCounter4 */
void StartCounter4(void const * argument)
{
  /* USER CODE BEGIN StartCounter4 */
	float n;
		/* Infinite loop */
		for (;;) {
			for (size_t i = 0; i < MAX_ITERATION; i++) {
	//			__NOP();
				n = 22 / 7;
			}

			counter4++;
			osDelay(1);
		}
	vTaskDelete(NULL);
  /* USER CODE END StartCounter4 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
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
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
