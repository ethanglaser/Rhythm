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
//
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
#define TRUE 1
#define FALSE 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

//-------------------------------------------------------------------------------
// RN4020 Global Variables
//-------------------------------------------------------------------------------
RN4020_State currState;
uint8_t rxBuffer[3];
uint8_t rxBuffer2[5];
uint8_t compareAOK[3] = {'A','O','K'};
uint8_t compareCMD[3] = {'C', 'M', 'D'};
uint8_t compareREB[3] = {'R', 'E', 'B'};
uint8_t compareREBOOT[6] = {'R', 'E', 'B', 'O', 'O', 'T'};
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// Battery Babysitter Global Variables
//-------------------------------------------------------------------------------
//important address definitions
  uint16_t Main_ADDR = 0x55 << 1;
  uint16_t rem = 0x0C;
  uint16_t full = 0x0E;
  //-------------------------------------------------------------------------------

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

//**********************************************************************
//  RN4020 Function Declarations (START)
//**********************************************************************

int RN4020_resetDefaultStep(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_resetToFactoryDefault(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_killConnection(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_setBaudRate115200(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_setService(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_clearPrivateSettings(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_setPrivateService(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_setPrivateChar(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_setDeviceType(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_rebootDevice(UART_HandleTypeDef *huart);

HAL_StatusTypeDef RN4020_waitForReadyState();

void sendData(UART_HandleTypeDef *huart, char _out[]);

void RN4020_sendData(UART_HandleTypeDef *huart, const char* line);

int compStr(uint8_t strcomp[], uint8_t expcomp[]);

void resetRxBuffer(uint8_t *rxBuffer);

void RN4020_setState(RN4020_State *state, RN4020_State newState);

void RN4020_checkState();

//**********************************************************************
//  RN4020 Function Declarations (END)
//**********************************************************************

//**********************************************************************
//  RN4020 Maybe useful later (START)
//**********************************************************************
//void setupReceiveInterrupt(UART_HandleTypeDef *huart, int size);
//void sendUSART(UART_HandleTypeDef *huart, char _out[], GPIO_TypeDef *GPIOX, int GPIO_Pin);


//
//void setupReceiveInterrupt(UART_HandleTypeDef *huart, int size) {
//	HAL_UART_Receive_IT(huart, rxBuffer, size);
//}

//void sendUSART(UART_HandleTypeDef *huart, char _out[], GPIO_TypeDef *GPIOX, int GPIO_Pin)
//{
//	HAL_StatusTypeDef ret;
//	ret = HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out), 10);
//	HAL_Delay(5000);
//	if (ret == HAL_OK) {
//		HAL_GPIO_TogglePin(GPIOX, GPIO_Pin);
//	}
//}

//**********************************************************************
//  RN4020 Maybe useful later (END)
//**********************************************************************

//**********************************************************************
//  HELPER RN4020 Functions (START)
//**********************************************************************

int compStr(uint8_t* strcomp, uint8_t* expcomp)
{
	//int sizeStrComp = sizeof(strcomp)/sizeof(strcomp[0]);
	int sizeExpComp = sizeof(expcomp)/sizeof(expcomp[0]);
	for(int i = 0; i < sizeExpComp; i++)
	{
		if (strcomp[i] != expcomp[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}

void resetRxBuffer(uint8_t *rxBuffer)
{
	int sizeRx = sizeof(rxBuffer) / sizeof(rxBuffer[0]);
	for (int i = 0; i < sizeRx; i++) {
		rxBuffer[i] = 0;
	}
}

//**********************************************************************
//  HELPER RN4020 Functions (END)
//**********************************************************************

//**********************************************************************
//  RN4020 Functions (START)
//**********************************************************************

int RN4020_resetDefaultStep(UART_HandleTypeDef *huart) {
	int completeFlag = TRUE;
	if (RN4020_resetToFactoryDefault(huart) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		completeFlag = FALSE;
	}
//	if (RN4020_killConnection(huart) != HAL_OK) {
//			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//			completeFlag = FALSE;
//		}
	if (RN4020_setBaudRate115200(huart) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		completeFlag = FALSE;
	}
	if (RN4020_setService(huart) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		completeFlag = FALSE;
	}
	if (RN4020_clearPrivateSettings(huart) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		completeFlag = FALSE;
	}
	if (RN4020_setPrivateService(huart) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		completeFlag = FALSE;
	}
	if (RN4020_setPrivateChar(huart) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		completeFlag = FALSE;
	}
	if (RN4020_setDeviceType(huart) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		completeFlag = FALSE;
	}
	if (RN4020_rebootDevice(huart) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		completeFlag = FALSE;
	}

	return completeFlag;
}


HAL_StatusTypeDef RN4020_resetToFactoryDefault(UART_HandleTypeDef *huart) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_AOK);
	RN4020_sendData(huart, "SF,1\r\n");
	return RN4020_waitForReadyState();
	}
HAL_StatusTypeDef RN4020_killConnection(UART_HandleTypeDef *huart) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_AOK);
	RN4020_sendData(huart, "K\r\n");
	return RN4020_waitForReadyState();
	}
HAL_StatusTypeDef RN4020_setBaudRate115200(UART_HandleTypeDef *huart) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_AOK);
	RN4020_sendData(huart, "SB,4\r\n");
	return RN4020_waitForReadyState();
	}

HAL_StatusTypeDef RN4020_setService(UART_HandleTypeDef *huart) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_AOK);
	RN4020_sendData(huart, "SS,C0000000\r\n");
	return RN4020_waitForReadyState();
	}
HAL_StatusTypeDef RN4020_clearPrivateSettings(UART_HandleTypeDef *huart) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_AOK);
	RN4020_sendData(huart, "PZ\r\n");
	return RN4020_waitForReadyState();
	}
HAL_StatusTypeDef RN4020_setPrivateService(UART_HandleTypeDef *huart) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_AOK);
	RN4020_sendData(huart, "PS,11223344556677889900AABBCCDDEEFF\r\n");
	return RN4020_waitForReadyState();
	}
HAL_StatusTypeDef RN4020_setPrivateChar(UART_HandleTypeDef *huart) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_AOK);
	RN4020_sendData(huart, "PC,010203040506070809000A0B0C0D0E0F,08,02\r\n");
	return RN4020_waitForReadyState();
	}
HAL_StatusTypeDef RN4020_setDeviceType(UART_HandleTypeDef *huart) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_AOK);
	RN4020_sendData(huart, "SR,20000000\r\n");
	return RN4020_waitForReadyState();
	}
HAL_StatusTypeDef RN4020_rebootDevice(UART_HandleTypeDef *huart) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_REBOOT);
	RN4020_sendData(huart, "R,1\r\n");
	return RN4020_waitForReadyState();
	}

HAL_StatusTypeDef RN4020_sendBatteryLife(UART_HandleTypeDef *huart, char* batteryLevel) {
	RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_AOK);
	RN4020_sendData(huart, "SUW,2A19,");
	RN4020_sendData(huart, batteryLevel);
	RN4020_sendData(huart, "\r\n");
	//return RN4020_waitForReadyState();
	return HAL_OK;
	}

//Uses global variable currState
HAL_StatusTypeDef RN4020_waitForReadyState() {
	if(currState == RN4020_STATE_READY) {
		return HAL_OK;
	}

	uint32_t startTime = HAL_GetTick();
	while (1) {
		if(currState == RN4020_STATE_READY) {
			resetRxBuffer(rxBuffer);
			return HAL_OK;
		}
		if ((HAL_GetTick() - startTime) > RN4020_TIMEOUT) {
			resetRxBuffer(rxBuffer);
			return HAL_TIMEOUT;
		}
		RN4020_checkState();
	}


}

//Uses global variable currState
void RN4020_checkState() {
	switch(currState) {
	case RN4020_STATE_WAITING_FOR_AOK:
		if (compStr(rxBuffer, compareAOK) == TRUE) {
			RN4020_setState(&currState, RN4020_STATE_READY);
			return;
		}
	break;
	case RN4020_STATE_WAITING_FOR_REBOOT:
		if (compStr(rxBuffer, compareREB) == TRUE) {
			RN4020_setState(&currState, RN4020_STATE_WAITING_FOR_CMD);
			return;
		}
	break;
	case RN4020_STATE_WAITING_FOR_CMD:
		if (compStr(rxBuffer, compareCMD) == TRUE) {
			RN4020_setState(&currState, RN4020_STATE_READY);
		}
	break;
	case RN4020_STATE_READY:
	break;
	}
}

void RN4020_setState(RN4020_State *state, RN4020_State newState) {
	*state = newState;
}

void RN4020_sendData(UART_HandleTypeDef *huart, const char* line) {
//  char carriageLineCh = '\r';
//  char newLineCh = '\n';
  HAL_UART_Transmit(huart, (uint8_t*)line, strlen(line), RN4020_TIMEOUT);
  //HAL_UART_Transmit(huart, (uint8_t*)&carriageLineCh, 1, RN4020_TIMEOUT);
  //HAL_UART_Transmit(huart, (uint8_t*)&newLineCh, 1, RN4020_TIMEOUT);
  //HAL_Delay(300);
}

//**********************************************************************
//  RN4020 Functions (END)
//**********************************************************************


void babysitter_SendData() {
	//Variables for babysitter interrupt
	HAL_StatusTypeDef ret;
	HAL_StatusTypeDef ret2;
	uint8_t data[2];
	uint8_t data2[2];
	int bigbattery = 0;

	ret = HAL_I2C_Mem_Read(&hi2c1, Main_ADDR, rem, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
	ret2 = HAL_I2C_Mem_Read(&hi2c1, Main_ADDR, full, I2C_MEMADD_SIZE_8BIT, data2, 2, HAL_MAX_DELAY);

	if ( ret != HAL_OK || ret2 != HAL_OK ) {
		HAL_UART_Transmit(&huart1, (uint8_t *) " RECEIVE ERROR", strlen(" RECEIVE ERROR"), 100);
	}
	else {
		char integer[4] = {0,0,0,0}; //create an empty string to store number
		char decimal[4] = {0,0,0,0}; //create an empty string to store number
		uint16_t finalval = ((uint16_t) data[1] << 8) | data[0];
		uint16_t finalval2 = ((uint16_t) data2[1] << 8) | data2[0];
		bigbattery = 100000 * finalval / finalval2;
		sprintf(integer, "%d", bigbattery / 1000);
		sprintf(decimal, "%03d", bigbattery % 1000);
//		HAL_UART_Transmit(&huart1, (uint8_t *) "BATTERY LEVEL: ", strlen("BATTERY LEVEL: "), 100);
//		HAL_UART_Transmit(&huart1, &integer, 4, 100);
//		HAL_UART_Transmit(&huart1, (uint8_t *) ".", strlen("."), 100);
//		HAL_UART_Transmit(&huart1, &decimal, 4, 100);
//		HAL_UART_Transmit(&huart1, (uint8_t *) "%\r\n", strlen("%\r\n"), 100);
		RN4020_sendBatteryLife(&huart1, &integer);

	  }
//	  if (ret == HAL_OK) {
//		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//	  }

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//HAL_UART_Transmit(&huart1, (uint8_t *)"Success\n\r", strlen("Success\n\r"), 100);
	HAL_UART_Receive_IT(&huart1, rxBuffer, sizeof(rxBuffer));

// --------------------------------------------------------------------------------------------
// DEBUGGING by TRANSMITTING BACK TO TERMINAL
//	// DEBUG: Write back to terminal to see if it is the same
//	HAL_UART_Transmit(&huart1, (uint8_t *)"\n\r", strlen("\n\r"), 100);
//    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 100);
//	HAL_UART_Transmit(&huart1, (uint8_t *)"\n\r", strlen("\n\r"), 100);
//
//	// Compare string using compStr function to see if they are the same
//	if(compStr(rxBuffer, compareAOK) == TRUE) {
//		HAL_UART_Transmit(&huart1, (uint8_t *)"Success\n\r", strlen("Success\n\r"), 100);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//	}
//	else {
//		HAL_UART_Transmit(&huart1, (uint8_t *)"Fail\n\r", strlen("Fail\n\r"), 100);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//	}
//    HAL_UART_Receive_IT(&huart1, rxBuffer, sizeof(rxBuffer));
// --------------------------------------------------------------------------------------------
}

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
	int RN4020_config_ret;

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
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // Set PA5 pin off
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);


//==================================================================================
//	Bluetooth LE Configuration Step
//==================================================================================
  HAL_UART_Receive_IT(&huart1, rxBuffer, sizeof(rxBuffer));
  RN4020_config_ret = RN4020_resetDefaultStep(&huart1);
  if (RN4020_config_ret == TRUE) {
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//==================================================================================

//==================================================================================
//	Start TIMER2 Step (function @ HAL_TIM_PeriodElapsedCallback)
//==================================================================================
  HAL_TIM_Base_Start_IT(&htim2);
//==================================================================================


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		//Only UART Channel 1 works for Discovery board... PB6/PB7
		//HAL_UART_Transmit(&huart2, (uint8_t *) "Hello, world!", strlen("Hello, world!"), 10);
		//HAL_UART_Transmit(&huart1, (uint8_t *) "Hello, world!", strlen("Hello, world!"), 10); // This is to the right of nucleo board
		//HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//      ----------------------------------------------------------------------------------------
//		POLLING FOR UART
//		HAL_UART_Receive(&huart1, rxBuffer, sizeof(rxBuffer), HAL_MAX_DELAY);
//		HAL_UART_Transmit(&huart1, (uint8_t *)"\n\r", strlen("\n\r"), 100);
//		HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 100);
//		HAL_UART_Transmit(&huart1, (uint8_t *)"\n\r", strlen("\n\r"), 100);
//		if(rxBuffer[0] == (uint8_t *)"A") {
//			HAL_UART_Transmit(&huart1, (uint8_t *)"Success\n\r", strlen("Success\n\r"), 100);
//		}
//		else {
//			HAL_UART_Transmit(&huart1, (uint8_t *)"Fail\n\r", strlen("Fail\n\r"), 100);
//		}

//		HAL_USART_Receive(&husart2, rxBuffer, sizeof(rxBuffer), HAL_MAX_DELAY);
//		HAL_USART_Transmit(&husart2, rxBuffer, sizeof(rxBuffer), HAL_MAX_DELAY);
//      ----------------------------------------------------------------------------------------

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
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
  hi2c1.Init.Timing = 0x00000509;
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 10000-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1500-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
	babysitter_SendData();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/