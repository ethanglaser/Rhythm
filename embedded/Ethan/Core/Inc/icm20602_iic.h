/*
 * icm20602_iic.h
 *
 *  Created on: Mar 20, 2021
 *      Author: glase
 */
#ifndef INC_ICM20602_IIC_H_
#define INC_ICM20602_IIC_H_

#define ICM20602_SELF_TEST_X_ACCEL 0x0D     // X-axis accelerometer self-check register
#define ICM20602_SELF_TEST_Y_ACCEL 0x0E     // Y-axis accelerometer self-check register
#define ICM20602_SELF_TEST_Z_ACCEL 0x0F     // Z-axis accelerometer self-check register
#define ICM20602_SMPLRT_DIV 0x19            // Sampling frequency divider
#define ICM20602_CONFIG_REG 0x1A            // Configuration: fifo MODE, Enables the FSYNC and DLPF low pass filter
#define ICM20602_GYRO_CONFIG_REG 0x1B       // (4:3 bit)gyroscope configuration
#define ICM20602_ACCEL_CONFIG1_REG 0x1C     // accelerometer configuration
#define ICM20602_ACCEL_CONFIG2_REG 0x1D     // accelerometer configuration
#define ICM20602_LP_MODE_CFG 0x1E           // gyroscope low-power mode or 6-axis low-power mode
#define ICM20602_LACCEL_WOM_X_THR 0x20      // Accelerometer X axis threshold
#define ICM20602_LACCEL_WOM_Y_THR 0x21      // Accelerometer Y axis threshold
#define ICM20602_LACCEL_WOM_Z_THR 0x22      // Accelerometer Z axis threshold
#define ICM20602_FIFO_EN 0x23               // GYRO/ACCEL _FIFO_EN
#define ICM20602_FSYNC_INT 0x36
#define ICM20602_INT_PIN_CFG 0x37           // Interrupt pin configuration
#define ICM20602_INT_ENABLE_REG 0x38        // interrupt enable
#define ICM20602_INT_STATUS 0x3A            // INTERRUPT STATUS
#define ICM20602_ACCEL_XOUT_H 0x3B          // accelerometer measurements
#define ICM20602_ACCEL_XOUT_L 0x3C
#define ICM20602_ACCEL_YOUT_H 0x3D
#define ICM20602_ACCEL_YOUT_L 0x3E
#define ICM20602_ACCEL_ZOUT_H 0x3F
#define ICM20602_ACCEL_ZOUT_L 0x40
#define ICM20602_TEMP_OUT_H 0x41            // TEMPERATURE MEASUREMENT,Temperature = 36.53 + regval/340
#define ICM20602_TEMP_OUT_L 0x42
#define ICM20602_GYRO_XOUT_H 0x43           // gyroscope measurements
#define ICM20602_GYRO_XOUT_L 0x44
#define ICM20602_GYRO_YOUT_H 0x45
#define ICM20602_GYRO_YOUT_L 0x46
#define ICM20602_GYRO_ZOUT_H 0x47
#define ICM20602_GYRO_ZOUT_L 0x48
#define ICM20602_SELF_TEST_X_GYRO 0x50
#define ICM20602_SELF_TEST_Y_GYRO 0x51
#define ICM20602_SELF_TEST_Z_GYRO 0x52
#define ICM20602_SIGNAL_PATH_RESET 0x68     // ACCEL_RST,TEMP_RST
#define ICM20602_ACCEL_INTEL_CTRL 0x69
#define ICM20602_USER_CTRL 0x6A             // FIFO_EN,FIFO_RST,SIG_COND_RST
#define ICM20602_PWR_MGMT1_REG 0x6B         // DEVICE_RESET,SLEEP,CYCLE,GYRO_STANDBY,TEMP_DIS,CLKSEL
#define ICM20602_PWR_MGMT2_REG 0x6C         // STBY_XA - STBY_ZG
#define ICM20602_I2C_IF 0x70                //1 – Disable I2C Slave module and put the serial interface in SPI mode only.
#define ICM20602_FIFO_COUNTH 0x72
#define ICM20602_FIFO_R_W 0x74              //FIFO READ WRITE
#define ICM20602_WHO_AM_I 0x75              //RO, default value ID = 0x12;
#define ICM20602_CONFIG 0x80
#define ICM20602_ADDR 0x69                  // Slave address, ad0 set 0

#endif
#include <stdint.h>
/*------------------------- Initialize ICM20602 --------------------- --------*/
uint8_t ICM20602_sensor_init(void);


/*------------------------ Set accelerometer and gyroscope------------------ -------*/
uint8_t ICM_Set_Gyro_Fsr(uint8_t fsr);
uint8_t ICM_Set_Accel_Fsr(uint8_t fsr);
uint8_t ICM_Set_DLPF(uint16_t lpf);
uint8_t ICM_Set_Rate(uint16_t rate);
uint8_t ICM_Set_Fifo(uint8_t sens);


/*---------------------- Get accelerometer, gyroscope and temperature data----------------* /
short ICM_Get_Temperature(void);
uint8_t ICM_Get_Gyroscope(short* gx, short* gy, short* gz);
uint8_t ICM_Get_Accelerometer(short* ax, short* ay, short* az);


#endif /* INC_ICM20602_IIC_H_ */
