//@@ -0,0 +1,273 @@
/**

 * ICM20602.c
 * @author ChrisP @ M-HIVE

 * This library source code has been created for STM32F4. Only supports SPI.
 *
 * Development environment specifics:
 * STM32CubeIDE 1.0.0
 * STM32CubeF4 FW V1.24.1
 * STM32F4 LL Driver(SPI) and HAL Driver(RCC for HAL_Delay() function)
 *
 * Created by ChrisP(Wonyeob Park) @ M-HIVE Embedded Academy, July, 2019
 * Rev. 1.0
 *
 * https://github.com/ChrisWonyeobPark/
*/

/**
 * @brief ICM20602 structure definition.
 */

#include <ICM20602.h>

Struct_ICM20602 ICM20602;
int32_t gyro_x_offset, gyro_y_offset, gyro_z_offset; // To remove offset
GPIO_InitTypeDef GPIO_InitStruct = {0};
SPI_HandleTypeDef SPI_InitStruct = {0};

/*void ICM20602_GPIO_SPI_Initialization(void)
{
//	SPI_HandleTypeDef SPI_InitStruct = {0};

//	GPIO_InitTypeDef GPIO_InitStruct = {0};
	 Peripheral clock enable -- done in main.c
//	APB2_GRP1_EnableClock(APB2_GRP1_PERIPH_SPI1);
//	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

//	AHB1_GRP1_EnableClock(AHB1_GRP1_PERIPH_GPIOA);
//	AHB1_GRP1_EnableClock(AHB1_GRP1_PERIPH_GPIOC);
//	RCC->IOPENR |= RCC_IOPENR_IOPBEN;


	*SPI1 GPIO Configuration
	PA5   ------> SPI1_SCK
	PA6   ------> SPI1_MISO
	PA7   ------> SPI1_MOSI


//	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
////	GPIO_InitStruct.OutputType = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_MspInit();
	SPI_InitStruct.Instance = SPI2;
	HAL_SPI_MspInit(&SPI_InitStruct);
	SPI_InitStruct.Init.Direction = SPI_DIRECTION_2LINES;
	SPI_InitStruct.Init.Mode = SPI_MODE_MASTER;
	SPI_InitStruct.Init.DataSize = SPI_DATASIZE_8BIT;
	SPI_InitStruct.Init.CLKPolarity = SPI_POLARITY_HIGH;
	SPI_InitStruct.Init.CLKPhase = SPI_PHASE_2EDGE;
	SPI_InitStruct.Init.NSS = SPI_NSS_SOFT;
	SPI_InitStruct.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; //ICM-20602 MAX SPI CLK is 10MHz. But DIV2(42MHz) is available.
	SPI_InitStruct.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SPI_InitStruct.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SPI_InitStruct.Init.CRCPolynomial = 7;
	SPI_InitStruct.State = HAL_SPI_STATE_READY;
	if (HAL_SPI_Init(&SPI_InitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }
//	__HAL_SPI_ENABLE(&SPI_InitStruct);
//	SPI1->CR1 |= SPI_CR1_SPE;
//	HAL_SPI_Init(&SPI_InitStruct);
//	LL_SPI_SetStandard(ICM20602_SPI_CHANNEL, LL_SPI_PROTOCOL_MOTOROLA);

	*ICM20602 GPIO Control Configuration
	 * PC4  ------> ICM20602_SPI_CS_PIN (output)
	 * PC5  ------> ICM20602_INT_PIN (input)


//	CHIP_SELECT(ICM20602);

	 setup GPIO pin for CS -- not needed - CS tied to ground
	GPIO_InitStruct.Pin = ICM20602_SPI_CS_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(ICM20602_SPI_CS_PORT, &GPIO_InitStruct);

	 setup GPIO pin for INT -- not used
	GPIO_InitStruct.Pin = ICM20602_INT_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(ICM20602_INT_PORT, &GPIO_InitStruct);

//	LL_SPI_Enable(ICM20602_SPI_CHANNEL);
//	__HAL_SPI_ENABLE(SPI_InitStruct);

//	CHIP_DESELECT(ICM20602);
}*/


unsigned char SPI1_SendByte(unsigned char data)
{
	while((SPI1->SR & SPI_SR_TXE) == RESET);
//	uint8_t dataAry[4] = {0xa0, 0xa2, 0xa4, 0xa6};
	unsigned char returnVal = 0;

	HAL_SPI_Transmit(&SPI_InitStruct, &data, sizeof(unsigned char), pow(2, 32) - 1);
//	HAL_SPI_TransmitReceive(&SPI_InitStruct, &data, &returnVal, sizeof(data)/sizeof(uint8_t), (uint32_t) (pow(2, 32) - 1));
	return returnVal;
}

//////////////////////////////////////////////////////////////

uint8_t ICM20602_Readbyte(uint8_t reg_addr)
{
	uint8_t val;

	val = SPI1_SendByte(reg_addr | 0x80); //Register. MSB 1 is read instruction.
	val = SPI1_SendByte(0x00); //Send DUMMY to read data

	return val;
}

void ICM20602_Readbytes(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;

	SPI1_SendByte(reg_addr | 0x80); //Register. MSB 1 is read instruction.
	while(i < len)
	{
		data[i++] = SPI1_SendByte(0x00); //Send DUMMY to read data
	}
}

void ICM20602_Writebyte(uint8_t reg_addr, uint8_t val)
{
	SPI1_SendByte(reg_addr & 0x7F); //Register. MSB 0 is write instruction.
	SPI1_SendByte(val); //Send Data to write
}

void ICM20602_Writebytes(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;
	CHIP_SELECT(ICM20602);
	SPI1_SendByte(reg_addr & 0x7F); //Register. MSB 0 is write instruction.
	while(i < len)
	{
		SPI1_SendByte(data[i++]); //Send Data to write
	}
	CHIP_DESELECT(ICM20602);
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
void MX_SPI1_Init(void)
{
  HAL_MspInit();
//  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  SPI_InitStruct.Instance = SPI1;
  __HAL_RCC_SPI1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  SPI_InitStruct.Init.Direction = SPI_DIRECTION_2LINES;
  SPI_InitStruct.Init.Mode = SPI_MODE_MASTER;
  SPI_InitStruct.Init.DataSize = SPI_DATASIZE_8BIT;
  SPI_InitStruct.Init.CLKPolarity = SPI_POLARITY_HIGH;
  SPI_InitStruct.Init.CLKPhase = SPI_PHASE_1EDGE;
  SPI_InitStruct.Init.NSS = SPI_NSS_HARD_INPUT;
  SPI_InitStruct.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; //ICM-20602 MAX SPI CLK is 10MHz. But DIV2(42MHz) is available.
  SPI_InitStruct.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SPI_InitStruct.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.Init.CRCPolynomial = 7;
  SPI_InitStruct.State = HAL_SPI_STATE_READY;
  if (HAL_SPI_Init(&SPI_InitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

int ICM20602_Initialization(void)
{

	uint8_t who_am_i = 0;
	int16_t accel_raw_data[3] = {0};  // To remove offset
	int16_t gyro_raw_data[3] = {0};   // To remove offset

    MX_SPI1_Init();

//	ICM20602_GPIO_SPI_Initialization();

	// check WHO_AM_I (0x75)
	while(1){
		who_am_i = ICM20602_Readbyte(WHO_AM_I);
	}

	// recheck
//	if(who_am_i != 0x12)
//	{
//		who_am_i = ICM20602_Readbyte(WHO_AM_I); // check again WHO_AM_I (0x75)
//
//		if (who_am_i != 0x12){
//			return who_am_i; //ERROR
//		}
//	}

	// Reset ICM20602
	// PWR_MGMT_1 0x6B
	ICM20602_Writebyte(PWR_MGMT_1, 0x80); //Reset ICM20602
//	HAL_Delay(50);

	// PWR_MGMT_1 0x6B
	ICM20602_Writebyte(PWR_MGMT_1, 0x01); // Enable Temperature sensor(bit4-0), Use PLL(bit2:0-01)
									// 온도센서 끄면 자이로 값 이상하게 출력됨
//	HAL_Delay(50);

	// PWR_MGMT_2 0x6C
	ICM20602_Writebyte(PWR_MGMT_2, 0x38); // Disable Acc(bit5:3-111), Enable Gyro(bit2:0-000)
	//ICM20602_Writebyte( PWR_MGMT_2, 0x00 ); // Enable Acc(bit5:3-000), Enable Gyro(bit2:0-000)
//	HAL_Delay(50);

	// set sample rate to 1000Hz and apply a software filter
	ICM20602_Writebyte(SMPLRT_DIV, 0x00);
//	HAL_Delay(50);

	// Gyro DLPF Config
	//ICM20602_Writebyte(CONFIG, 0x00); // Gyro LPF fc 250Hz(bit2:0-000)
	ICM20602_Writebyte(CONFIG, 0x05); // Gyro LPF fc 20Hz(bit2:0-100) at 1kHz sample rate
//	HAL_Delay(50);

	// GYRO_CONFIG 0x1B
	ICM20602_Writebyte(GYRO_CONFIG, 0x18); // Gyro sensitivity 2000 dps(bit4:3-11), FCHOICE (bit1:0-00)
//	HAL_Delay(50);

	// ACCEL_CONFIG 0x1C
	ICM20602_Writebyte(ACCEL_CONFIG, 0x18); // Acc sensitivity 16g
//	HAL_Delay(50);

	// ACCEL_CONFIG2 0x1D
	ICM20602_Writebyte(ACCEL_CONFIG2, 0x03); // Acc FCHOICE 1kHz(bit3-0), DLPF fc 44.8Hz(bit2:0-011)
//	HAL_Delay(50);

	// Enable Interrupts when data is ready
	ICM20602_Writebyte(INT_ENABLE, 0x01); // Enable DRDY Interrupt
//	HAL_Delay(50);

	//printf("gyro bias: %d %d %d\n", gyro_x_offset, gyro_y_offset, gyro_z_offset);

	// Remove Gyro X offset
//	ICM20602_Writebyte( XG_OFFS_USRH, offset_x>>8 );	// gyro x offset high byte
//	ICM20602_Writebyte( XG_OFFS_USRL, offset_x );	// gyro x offset low byte
//
//	// Remove Gyro Y offset
//	ICM20602_Writebyte( YG_OFFS_USRH, offset_y>>8 );	// gyro y offset high byte
//	ICM20602_Writebyte( YG_OFFS_USRL, offset_y );	// gyro y offset low byte
//
//	// Remove Gyro Z offset
//	ICM20602_Writebyte( ZG_OFFS_USRH, offset_z>>8 );	// gyro z offset high byte
//	ICM20602_Writebyte( ZG_OFFS_USRL, offset_z );	// gyro z offset low byte

	return 0; //OK
}

void ICM20602_Get6AxisRawData(short* accel, short* gyro)
{
	unsigned char data[14];
	ICM20602_Readbytes(ACCEL_XOUT_H, 14, data);

	accel[0] = (data[0] << 8) | data[1];
	accel[1] = (data[2] << 8) | data[3];
	accel[2] = (data[4] << 8) | data[5];

	gyro[0] = ((data[8] << 8) | data[9]);
	gyro[1] = ((data[10] << 8) | data[11]);
	gyro[2] = ((data[12] << 8) | data[13]);
}

void ICM20602_Get3AxisGyroRawData(short* gyro)
{
	unsigned char data[6];
	ICM20602_Readbytes(GYRO_XOUT_H, 6, data);

	gyro[0] = ((data[0] << 8) | data[1]);
	gyro[1] = ((data[2] << 8) | data[3]);
	gyro[2] = ((data[4] << 8) | data[5]);
}

void ICM20602_Get3AxisAccRawData(short* accel)
{
	unsigned char data[6];
	ICM20602_Readbytes(ACCEL_XOUT_H, 6, data);

	accel[0] = ((data[0] << 8) | data[1]);
	accel[1] = ((data[2] << 8) | data[3]);
	accel[2] = ((data[4] << 8) | data[5]);
}

int ICM20602_DataReady(void)
{
	return HAL_GPIO_ReadPin(ICM20602_INT_PORT, ICM20602_INT_PIN);
}
