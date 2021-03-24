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
#include <icm20602_iic.h>
#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include "stm32l0xx_hal.h"

//#include <SparkFunBQ27441.h>

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
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

USART_HandleTypeDef husart1;
USART_HandleTypeDef husart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_Init(void);
static void MX_USART1_Init(void);
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
  MX_I2C1_Init();
  MX_USART2_Init();
  MX_USART1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint16_t Main_ADDR = 0x55 << 1; // Use 8-bit address

  HAL_StatusTypeDef ret;
  HAL_StatusTypeDef ret2;

  uint8_t data[2];
  uint8_t test = 0x0C;
  uint8_t buf[12];
  int16_t val;
  float temp_c;
  uint8_t data2[2];

    while (1)
    {
  	  uint16_t rem = 0x0C;
  	  uint16_t full = 0x0E;

      /*uint8_t subCommandMSB = (function >> 8);
      uint8_t subCommandLSB = (function & 0x00FF);
      uint8_t command[2] = {subCommandLSB, subCommandMSB};*/
      uint8_t data[2];
      uint8_t data2[2];

  	  ret = HAL_I2C_Mem_Read(&hi2c1, Main_ADDR, rem, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
  	  ret2 = HAL_I2C_Mem_Read(&hi2c1, Main_ADDR, full, I2C_MEMADD_SIZE_8BIT, data2, 2, HAL_MAX_DELAY);

  	  if ( ret != HAL_OK || ret2 != HAL_OK ) {
  		  HAL_USART_Transmit(&husart2, (uint8_t *) " RECEIVE ERROR", strlen(" RECEIVE ERROR"), 100);
  	  }
  	  else {
  		  char bef[4] = {0,0,0,0}; //create an empty string to store number
  		  char aft[4] = {0,0,0,0}; //create an empty string to store number
  		  uint16_t finalval = ((uint16_t) data[1] << 8) | data[0];
  		  uint16_t finalval2 = ((uint16_t) data2[1] << 8) | data2[0];
  		  int stuff = 100000 * finalval / finalval2;
  	      sprintf(bef, "%d", stuff / 1000);
  	      sprintf(aft, "%03d", stuff % 1000);
  		  HAL_USART_Transmit(&husart2, (uint8_t *) "BATTERY LEVEL: ", strlen("BATTERY LEVEL: "), 100);
  		  HAL_USART_Transmit(&husart2, &bef, 4, 100);
  		  HAL_USART_Transmit(&husart2, (uint8_t *) ".", strlen("."), 100);
  		  HAL_USART_Transmit(&husart2, &aft, 4, 100);
  		  HAL_USART_Transmit(&husart2, (uint8_t *) "%\r\n", strlen("%\r\n"), 100);

  	  }
  	  /*ret = HAL_I2C_Master_Transmit(&hi2c1, Main_ADDR, command, 2, 2000);
  	  if ( ret != HAL_OK ) {
  		  HAL_USART_Transmit(&husart1, (uint8_t *) " TRANSMIT ERROR", strlen(" TRANSMIT ERROR"), 100);
  		  HAL_USART_Transmit(&husart2, (uint8_t *) " TRANSMIT ERROR", strlen(" TRANSMIT ERROR"), 100);
  	  }
  	  else {
  		  ret = HAL_I2C_Master_Receive(&hi2c1, Main_ADDR | 0x01, data2, 2, 2000);
  		  if ( ret != HAL_OK ) {
  			  HAL_USART_Transmit(&husart1, (uint8_t *) " RECEIVE ERROR", strlen(" RECEIVE ERROR"), 100);
  			  HAL_USART_Transmit(&husart2, (uint8_t *) " RECEIVE ERROR", strlen(" RECEIVE ERROR"), 100);
  		  }
  		  else {
  			  uint16_t valll = (((data2[0] & 0x0F) << 8) | data2[1]);
  		      uint16_t vall = ((int16_t)data2[0] << 4) | (data2[1] >> 4);
  			  uint16_t finalval = ((uint16_t)data2[1] << 8) | data2[0];
  			  if (finalval == 0x0421) {
  	  			  HAL_USART_Transmit(&husart1, (uint8_t *) " YIPPEE", strlen(" YIPPEE"), 100);
  	  			  HAL_USART_Transmit(&husart2, (uint8_t *) " YIPPEE", strlen(" YIPPEE"), 100);
  			  }
  			  ret2 = HAL_USART_Transmit(&husart1, finalval, sizeof(uint16_t), 100);
  			  ret2 = HAL_USART_Transmit(&husart2, finalval, sizeof(uint16_t), 100);
  		  }
  	  }*/
  	  if (ret == HAL_OK) {
  		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  	  }
	  HAL_Delay(1000);
    }

  //val = ((int16_t)buf[0] << 4) | (buf[1] >> 4);

  //HAL_USART_Transmit(&husart2, (uint8_t *) buf, 12, 10);

  //sprintf((char*)buf, "%d \r\n", val);
  while (1)
  {
	  buf[0] = test;
	  ret = HAL_I2C_Master_Transmit(&hi2c1, Main_ADDR, buf, 1, 1000);

	  //HAL_USART_Transmit(&husart2, (uint8_t *) ret, sizeof(HAL_StatusTypeDef), 10);
	  if ( ret != HAL_OK ) {
		  HAL_USART_Transmit(&husart2, (uint8_t *) " TRANSMIT ERROR", strlen(" TRANSMIT ERROR"), 10);
	  }
	  else {
		  //HAL_USART_Transmit(&husart2, (uint8_t *) " GREATT SUCCESS", strlen(" GREATT SUCCESS"), 10);
		  ret = HAL_I2C_Master_Receive(&hi2c1, Main_ADDR, data, 2, 1000);
		  if ( ret != HAL_OK ) {
			  HAL_USART_Transmit(&husart2, (uint8_t *) " RECEIVE ERROR", strlen(" RECEIVE ERROR"), 10);
		  }
		  else {
			  val = ((int16_t)buf[0] << 4) | (buf[1] >> 4);
		      if ( val > 0x7FF ) {
		        val |= 0xF000;
		      }
		      temp_c = val * 1.0;
		      sprintf((char*)buf,
		            "%u \%\r\n",
		            (unsigned int)temp_c);

			  HAL_USART_Transmit(&husart2, buf, strlen((char*)buf), 10);
			  //HAL_USART_Transmit(&husart2, (uint8_t *) " GREATR SUCCESS", strlen(" GREATR SUCCESS"), 10);
		  }
	  }
	  /*ret = HAL_I2C_Master_Receive(&hi2c1, Main_ADDR, buf, 2, 1000);
	  if ( ret != HAL_OK ) {
		  HAL_USART_Transmit(&husart2, (uint8_t *) "TX ERROR", strlen("RX ERROR"), 10);
	  }*/
	  /*
	  HAL_USART_Transmit(&husart2, (uint8_t *) "Hello, world!", strlen("Hello, world!"), 10);
	  //debugPrint(&husart2, "oi, mate!"); // print*/
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);


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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000708;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

 /* USER CODE END 5 */

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
