/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#define FLASH_USER_START_ADDR   (FLASH_BASE + (4 * FLASH_PAGE_SIZE))   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (FLASH_BASE + FLASH_SIZE - 1)   /* End @ of user Flash area */

#define DATA_64                 ((uint64_t)0x1234567812345678)
#define DATA_32                 ((uint32_t)0x12345678)
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TM1637.h"
#include "flash_if.h"
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
uint32_t GetPage(uint32_t Addr);
uint32_t Get_CounterVal_From_Flash();
void Set_CounterVal_To_Flash(uint64_t counterVal);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
FLASH_EraseInitTypeDef EraseInitStruct = {0};
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint32_t CounterVar = 0;
volatile uint32_t PrevCounterVar = 0;
extern uint32_t LastResetISRTick;
extern volatile uint8_t ButtonPressed;
volatile uint32_t lowPeriodCountTick = 0;
volatile uint8_t lowPeriodCount = 0;
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
  //HAL_Delay(3000);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  TM1637_Init();
  TM1637_SetBrightness(8);

  CounterVar = Get_CounterVal_From_Flash();
  CounterVar = CounterVar > 9999 ? 0 : CounterVar;
  TM1637_DisplayDecimal(CounterVar, 0);
  lowPeriodCountTick = HAL_GetTick();
  lowPeriodCount = 2;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  if(PrevCounterVar != CounterVar)
	  {
		  TM1637_DisplayDecimal(CounterVar, 0);
		  Set_CounterVal_To_Flash(CounterVar);
		  PrevCounterVar = CounterVar;
	  }
	  if(HAL_GPIO_ReadPin(PB6_MAG_SENSOR_IN_GPIO_Port, PB6_MAG_SENSOR_IN_Pin) == 0 && ((HAL_GetTick() - lowPeriodCountTick) > 600) && lowPeriodCount == 0) {
		  lowPeriodCountTick = HAL_GetTick();
		  if(CounterVar > 9998) {
			  CounterVar = 0;
		  }
		  CounterVar++;
		  lowPeriodCount = 1;
	  }
	  if(HAL_GPIO_ReadPin(PB6_MAG_SENSOR_IN_GPIO_Port, PB6_MAG_SENSOR_IN_Pin) == 1) {
		  lowPeriodCountTick = HAL_GetTick();
		  lowPeriodCount = 0;
	  }
	  if(ButtonPressed && ((HAL_GetTick() - LastResetISRTick) > 1500))
	  {
		  ButtonPressed = 0;
		  CounterVar = 0;
	  }

	  HAL_Delay(1);
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PA1_TM1637_CLK_Pin|PA2_TM1637_DATA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0_RESET_SW_IN_Pin */
  GPIO_InitStruct.Pin = PA0_RESET_SW_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;

  HAL_GPIO_Init(PA0_RESET_SW_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1_TM1637_CLK_Pin PA2_TM1637_DATA_Pin */
  GPIO_InitStruct.Pin = PA1_TM1637_CLK_Pin|PA2_TM1637_DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6_MAG_SENSOR_IN_Pin */
  GPIO_InitStruct.Pin = PB6_MAG_SENSOR_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;

  HAL_GPIO_Init(PB6_MAG_SENSOR_IN_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}

uint32_t Get_CounterVal_From_Flash()
{
	return *(uint32_t *)FLASH_USER_START_ADDR;
}

void Set_CounterVal_To_Flash(uint64_t counterVal)
{
	HAL_FLASH_Unlock();
	uint32_t Address = FLASH_USER_START_ADDR;
	uint32_t FirstPage = GetPage(FLASH_USER_START_ADDR);
	uint32_t PageError = 0;

	/* Get the number of pages to erase from 1st page */
	uint32_t NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page        = FirstPage;
	EraseInitStruct.NbPages     = NbOfPages;

	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, counterVal);
	HAL_FLASH_Lock();
}

void HAL_PWREx_PVD_Falling_Callback(void)
{
	Set_CounterVal_To_Flash(CounterVar);
  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_PWR_PVD_Falling_Callback can be implemented in the user file
  */
}
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
