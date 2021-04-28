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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct accelPacket
{
	float accX;
	float accY;
	float accZ;
	float mag;
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

uint16_t tempRaw;
uint8_t accXH;
uint8_t accYH;
uint8_t accZH;
uint16_t accXraw;
uint16_t accYraw;
uint16_t accZraw;

int i = 0;
float temp_sens;
float temp;
float accel_sens;
float accX = 1;
float accY = 1;
float accZ = 1;
float mag;

float temps[100] = {};
struct accelPacket accels[100] = {};
uint8_t data[8];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
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
  /* USER CODE BEGIN 2 */
  uint16_t Main_ADDR = 0x68 << 1;

  HAL_StatusTypeDef ret = HAL_OK;
  HAL_StatusTypeDef ret2 = HAL_OK;
  HAL_StatusTypeDef ret3 = HAL_OK;
  HAL_StatusTypeDef ret4 = HAL_OK;
  HAL_StatusTypeDef ret5 = HAL_OK;
  HAL_StatusTypeDef ret6 = HAL_OK;
  HAL_StatusTypeDef ret7 = HAL_OK;
  HAL_StatusTypeDef ret8 = HAL_OK;
  HAL_StatusTypeDef ret9 = HAL_OK;
  HAL_StatusTypeDef ret10 = HAL_OK;
  HAL_StatusTypeDef ret11 = HAL_OK;

  uint8_t deviceID;
  uint8_t pwr1 = 0x80;
  uint8_t pwr1_2 = 0x01;
  uint8_t pwr2 = 0x38;
  uint8_t pwr2_2 = 0x00;
  uint8_t rtDiv = 0x00;
  uint8_t config = 0x05;
  uint8_t gyroConfig = 0x18;
  uint8_t accelConfig = 0x18;
  uint8_t accelConfig2 = 0x03;
  uint8_t intEnable = 0x01;

//  FILE *fptr = fopen("acc_data.txt", "w+");
//  if(fptr == NULL){
//	  return 0;
//  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // read who_am_i
	  ret = HAL_I2C_Mem_Read(&hi2c1, Main_ADDR, WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &deviceID, 1, HAL_MAX_DELAY);
	  HAL_Delay(25);

	  // reset IMU
	  ret2 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &pwr1, 1, HAL_MAX_DELAY);
	  HAL_Delay(25);

	  // enable temperature sensor
	  ret11 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &pwr1_2, 1, HAL_MAX_DELAY);
	  HAL_Delay(25);
	  temp_sens =  326.8;

	  // disable acc and gyro
//	  ret3 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, PWR_MGMT_2, I2C_MEMADD_SIZE_8BIT, &pwr2, 1, HAL_MAX_DELAY);
//	  HAL_Delay(25);

	  // enable acc and gyro
//	  ret4 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, PWR_MGMT_2, I2C_MEMADD_SIZE_8BIT, &pwr2_2, 1, HAL_MAX_DELAY);
//	  HAL_Delay(25);

	  // set sample rate to 1kHz and apply
	  ret5 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, &rtDiv, 1, HAL_MAX_DELAY);
	  HAL_Delay(25);

	  // set DLPF_CFG to 5 for gyro configuration
//	  ret6 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, CONFIG, I2C_MEMADD_SIZE_8BIT, &config, 1, HAL_MAX_DELAY);
//	  HAL_Delay(25);

	  // gryo full-scale range = 2000dps(0b11) -- sensitivity scale factor = 2,048 LSB/(dps)
//	  ret7 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &gyroConfig, 1, HAL_MAX_DELAY);
//	  HAL_Delay(25);
//
	  // accel full-scale range = 16g(0b11) -- sensitivity scale factor = 2,048 LSB/(dps)
	  ret8 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &accelConfig, 1, HAL_MAX_DELAY); // ACCEL full-scale range = 16g -- sensitivity scale facotr = 2,048 LSB/(dps)
	  HAL_Delay(25);
	  accel_sens = 2048.0;

	  // set A_DLPF_CFG to 3 for accel configuration
	  ret9 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, ACCEL_CONFIG2, I2C_MEMADD_SIZE_8BIT, &accelConfig2, 1, HAL_MAX_DELAY); // ACCEL FCHOICE 1kHz(bit3-0), DLPF fc 44.8Hz(bit2:0-011)
	  HAL_Delay(25);
//
//	  ret10 = HAL_I2C_Mem_Write(&hi2c1, Main_ADDR, INT_ENABLE, I2C_MEMADD_SIZE_8BIT, &intEnable, 1, HAL_MAX_DELAY);
//	  HAL_Delay(25);j

	  while(1) {
		  if (ret == HAL_OK && ret2 == HAL_OK && ret3 == HAL_OK && ret4 == HAL_OK && ret5 == HAL_OK && ret6 == HAL_OK && ret7 == HAL_OK && ret8 == HAL_OK && ret9 == HAL_OK && ret10 == HAL_OK && ret11 == HAL_OK)
		  {

			while(i < 100) {
				ret = HAL_I2C_Mem_Read(&hi2c1, Main_ADDR, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, data, sizeof(data)/sizeof(uint8_t), HAL_MAX_DELAY);
	//			ret = HAL_I2C_Mem_Read(&hi2c1, Main_ADDR, TEMP_OUT_H, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
				HAL_Delay(25);
				if (ret != HAL_OK){
					break;
				}
//				data[0] = (int8_t) data[0];
//				data[2] = (int8_t) data[2];
//				data[4] = (int8_t) data[4];
				if ((data[0] & 0x80) != 0){
					accX = -1;
					data[0] = (data[0] ^ 0xFF) + 1;
				}
				if ((data[2] & 0x80) != 0){
					accY = -1;
					data[2] = (data[2] ^ 0xFF) + 1;
				}
				if ((data[4] & 0x80) != 0){
					accZ = -1;
					data[4] = (data[4] ^ 0xFF) + 1;
				}
				accXraw = (uint16_t) (data[0] << 8 | data[1]);
				accYraw = (uint16_t) (data[2] << 8 | data[3]);
				accZraw = (uint16_t) (data[4] << 8 | data[5]);
				accX *= accXraw / accel_sens;
				accY *= accYraw / accel_sens;
				accZ *= accZraw / accel_sens;
				mag = sqrt((pow(accX, 2) + pow(accY, 2) + pow(accZ, 2)));
				accels[i].accX = accX;
				accels[i].accY = accY;
				accels[i].accZ = accZ;
				accels[i].mag = mag;
//				fprintf(fptr, "%f\n", mag);

				tempRaw = ((uint16_t) data[6] << 8) | data[7];
				temp = (tempRaw / temp_sens) + 25;
				temps[i] = temp;

				accX = 1;
				accY = 1;
				accZ = 1;
				i++;
			}
			i = 0;
	//		accel[0] = (data[0] << 8) | data[1];
	//		accel[1] = (data[2] << 8) | data[3];
	//		accel[2] = (data[4] << 8) | data[5];
	  	  }
	  }

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_3;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_3;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
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
  hi2c1.Init.Timing = 0x00303D5B;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : MFX_IRQ_OUT_Pin */
  GPIO_InitStruct.Pin = MFX_IRQ_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MFX_IRQ_OUT_GPIO_Port, &GPIO_InitStruct);

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
