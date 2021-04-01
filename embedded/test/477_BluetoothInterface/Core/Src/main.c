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
#include <stdlib.h>
#include "string.h"
//#include "rn4020.c"
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RN4020_TIMEOUT 5000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
USART_HandleTypeDef husart1;
USART_HandleTypeDef husart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_Init(void);
static void MX_USART2_Init(void);
/* USER CODE BEGIN PFP */
void sendUSART(USART_HandleTypeDef *huart, char _out[], GPIO_TypeDef *GPIOX, int GPIO_Pin);

void RN4020_resetDefaultStep(USART_HandleTypeDef *huart);

void RN4020_resetToFactoryDefault(USART_HandleTypeDef *huart);

void RN4020_setBaudRate115200(USART_HandleTypeDef *huart);

void RN4020_setService(USART_HandleTypeDef *huart);

void RN4020_clearPrivateSettings(USART_HandleTypeDef *huart);

void RN4020_setPrivateService(USART_HandleTypeDef *huart);

void RN4020_setPrivateChar(USART_HandleTypeDef *huart);

void RN4020_setDeviceType(USART_HandleTypeDef *huart);

void sendData(USART_HandleTypeDef *huart, char _out[]);

void RN4020_sendData(USART_HandleTypeDef *huart, const char* line);

void sendUSART(USART_HandleTypeDef *huart, char _out[], GPIO_TypeDef *GPIOX, int GPIO_Pin)
{
	HAL_StatusTypeDef ret;
	ret = HAL_USART_Transmit(huart, (uint8_t *) _out, strlen(_out), 10);
	HAL_Delay(5000);
	if (ret == HAL_OK) {
		HAL_GPIO_TogglePin(GPIOX, GPIO_Pin);
	}
}
//**********************************************************************
//  RN4020 Functions (START)
//**********************************************************************

void RN4020_resetDefaultStep(USART_HandleTypeDef *huart) {
	  RN4020_resetToFactoryDefault(huart);
	  RN4020_setBaudRate115200(huart);
	  RN4020_setService(huart);
	  RN4020_clearPrivateSettings(huart);
	}

void RN4020_resetToFactoryDefault(USART_HandleTypeDef *huart) {
  RN4020_sendData(huart, "SF,1");
}

void RN4020_setBaudRate115200(USART_HandleTypeDef *huart) {
	  RN4020_sendData(huart, "SB,4");
	}

void RN4020_setService(USART_HandleTypeDef *huart) {
	  RN4020_sendData(huart, "SS,C0000001");
	}
void RN4020_clearPrivateSettings(USART_HandleTypeDef *huart) {
	  RN4020_sendData(huart, "PZ");
	}
void RN4020_setPrivateService(USART_HandleTypeDef *huart) {
	  RN4020_sendData(huart, "PS,11223344556677889900AABBCCDDEEFF");
	}
void RN4020_setPrivateChar(USART_HandleTypeDef *huart) {
	  RN4020_sendData(huart, "PC,010203040506070809000A0B0C0D0E0F,08,02");
	}
void RN4020_setDeviceType(USART_HandleTypeDef *huart) {
	  RN4020_sendData(huart, "SR,20000000");
	}

void RN4020_sendData(USART_HandleTypeDef *huart, const char* line) {
  char newLineCh = '\n';
  HAL_USART_Transmit(huart, (uint8_t*)line, strlen(line), RN4020_TIMEOUT);
  HAL_USART_Transmit(huart, (uint8_t*)&newLineCh, 1, RN4020_TIMEOUT);
}

//**********************************************************************
//  RN4020 Functions (END)
//**********************************************************************

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
//  HAL_StatusTypeDef pass;

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
  MX_USART1_Init();
  MX_USART2_Init();
  /* USER CODE BEGIN 2 */
	//setup_usart1();

	//char testline[] = "Hello World.";
//    ret = HAL_USART_Transmit(&husart1, (uint8_t *) "+", strlen("+"), 10);
//    //debugPrint(&husart2, "Hello!");
//    if (ret == HAL_OK) {
//    	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//    }
    //HAL_USART_Receive(&husart1, (uint8_t *) in, 8, 1000);

    //HAL_USART_Receive(&husart1, (uint8_t *) in, 8, 1);

  //sendUSART(&husart2, "+", GPIOA, GPIO_PIN_5);
  //sendUSART(&husart2, "SF,1", GPIOA, GPIO_PIN_5);
  RN4020_resetDefaultStep(&husart2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		//HAL_USART_Receive(&husart1, (uint8_t *)aRxBuffer, 8, 10);
		//HAL_USART_Transmit(&husart2, (uint8_t *)aRxBuffer, strlen(aRxBuffer), 10);
		HAL_USART_Transmit(&husart2, (uint8_t *) "Hello, world!", strlen("Hello, world!"), 10);

		HAL_Delay(1000);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  husart1.Instance = USART1;
  husart1.Init.BaudRate = 115200;
  husart1.Init.WordLength = USART_WORDLENGTH_8B;
  husart1.Init.StopBits = USART_STOPBITS_1;
  husart1.Init.Parity = USART_PARITY_NONE;
  husart1.Init.Mode = USART_MODE_TX_RX;
  husart1.Init.CLKPolarity = USART_POLARITY_LOW;
  husart1.Init.CLKPhase = USART_PHASE_1EDGE;
  husart1.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  if (HAL_USART_Init(&husart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  husart2.Instance = USART2;
  husart2.Init.BaudRate = 115200;
  husart2.Init.WordLength = USART_WORDLENGTH_8B;
  husart2.Init.StopBits = USART_STOPBITS_1;
  husart2.Init.Parity = USART_PARITY_NONE;
  husart2.Init.Mode = USART_MODE_TX_RX;
  husart2.Init.CLKPolarity = USART_POLARITY_LOW;
  husart2.Init.CLKPhase = USART_PHASE_1EDGE;
  husart2.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  if (HAL_USART_Init(&husart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
