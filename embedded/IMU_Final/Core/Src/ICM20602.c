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
#include <math.h>

Struct_ICM20602 ICM20602;
int32_t gyro_x_offset, gyro_y_offset, gyro_z_offset; // To remove offset
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef SPI_InitStruct = {0};

unsigned char SPI1_SendByte(unsigned char data)
{
	while((SPI2->SR & SPI_SR_TXE) == RESET);
	unsigned char returnVal = 0;

	HAL_SPI_Transmit(&SPI_InitStruct, &data, sizeof(unsigned char), pow(2, 32) - 1);

	return returnVal;
}

uint8_t ICM20602_Readbyte(uint8_t reg_addr)
{
	uint8_t val;
	val = SPI1_SendByte(reg_addr | 0x80); //Register. MSB 1 is read instruction.

	return val;
}

void ICM20602_Writebyte(uint8_t reg_addr, uint8_t val)
{
	SPI1_SendByte(reg_addr & 0x7F); //Register. MSB 0 is write instruction.
	SPI1_SendByte(val); //Send Data to write
}

int ICM20602_Initialization(void)
{

	uint8_t who_am_i = 0;
	int16_t accel_raw_data[3] = {0};  // To remove offset
	int16_t gyro_raw_data[3] = {0};   // To remove offset

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
