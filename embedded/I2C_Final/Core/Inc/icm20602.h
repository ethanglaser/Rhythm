#ifndef _ICM20602_H
#define _ICM20602_H

/***** Includes *****/

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief ICM-20602 Register Map
 */

#define	XG_OFFS_TC_H	0x04
#define	XG_OFFS_TC_L	0x05
#define	YG_OFFS_TC_H	0x07
#define	YG_OFFS_TC_L	0x08
#define	ZG_OFFS_TC_H	0x0A
#define	ZG_OFFS_TC_L	0x0B
#define	SELF_TEST_X_ACCEL	0x0D
#define	SELF_TEST_Y_ACCEL	0x0E
#define	SELF_TEST_Z_ACCEL	0x0F
#define	XG_OFFS_USRH	0x13
#define	XG_OFFS_USRL	0x14
#define	YG_OFFS_USRH	0x15
#define	YG_OFFS_USRL	0x16
#define	ZG_OFFS_USRH	0x17
#define	ZG_OFFS_USRL	0x18
#define	SMPLRT_DIV	0x19
#define	CONFIG	0x1A    //The default value of the register is 0x80.
#define	GYRO_CONFIG	0x1B
#define	ACCEL_CONFIG	0x1C
#define	ACCEL_CONFIG2	0x1D
#define	LP_MODE_CFG	0x1E
#define	ACCEL_WOM_X_THR	0x20
#define	ACCEL_WOM_Y_THR	0x21
#define	ACCEL_WOM_Z_THR	0x22
#define	FIFO_EN	0x23
#define	FSYNC_INT	0x36
#define	INT_PIN_CFG	0x37
#define	INT_ENABLE	0x38
#define	FIFO_WM_INT_STATUS	0x39

#define	INT_STATUS	0x3A
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H	0x41
#define	TEMP_OUT_L	0x42
#define	GYRO_XOUT_H	0x43
#define	GYRO_XOUT_L	0x44
#define	GYRO_YOUT_H	0x45
#define	GYRO_YOUT_L	0x46
#define	GYRO_ZOUT_H	0x47
#define	GYRO_ZOUT_L	0x48
#define	SELF_TEST_X_GYRO	0x50
#define	SELF_TEST_Y_GYRO	0x51
#define	SELF_TEST_Z_GYRO	0x52
#define	FIFO_WM_TH1	0x60
#define	FIFO_WM_TH2	0x61
#define	SIGNAL_PATH_RESET	0x68
#define	ACCEL_INTEL_CTRL	0x69
#define	USER_CTRL	0x6A
#define	PWR_MGMT_1	0x6B //The default value of the register is 0x41.
#define	PWR_MGMT_2	0x6C
#define	I2C_IF	0x70
#define	FIFO_COUNTH	0x72
#define	FIFO_COUNTL	0x73
#define	FIFO_R_W	0x74
#define	WHO_AM_I	0x75 //The default value of the register is 0x12.
#define	XA_OFFSET_H	0x77
#define	XA_OFFSET_L	0x78
#define	YA_OFFSET_H	0x7A
#define	YA_OFFSET_L	0x7B
#define	ZA_OFFSET_H	0x7D
#define	ZA_OFFSET_L	0x7E

/***** Macros *****/

/** This initializer will zero out the ICM20602 struct for initialization
  * purposes. This is advised as to avoid having uninitialized garbage values
  * left within the struct */
#define ICM20602_INIT() \
  { \
    .id = 0, \
    .hal_wr = NULL, \
    .hal_rd = NULL, \
    .hal_sleep = NULL, \
    .mutex_lock = NULL, \
    .mutex_unlock = NULL, \
    .use_accel = false, \
    .accel_fifo = false, \
    .accel_dlpf = 0, \
    .accel_g = 0, \
    .use_gyro = false, \
    .gyro_fifo = false, \
    .gyro_dlpf = 0, \
    .gyro_dps = 0, \
    .sample_rate_div = 1, \
    .i2c_disable = false, \
  }

/** This initializer will configure the ICM20602 with settings that should
  * yield some actual output on both the gyroscope and the accelerometer. All
  * that the developer should need to set manually are the "hal_wr", "hal_rd",
  * and "hal_sleep" function pointers. It is recommended to use this for
  * testing purposes. */
#define ICM20602_DEFAULT_INIT() \
  { \
    .id = 0, \
    .hal_wr = NULL, \
    .hal_rd = NULL, \
    .hal_sleep = NULL, \
    .mutex_lock = NULL, \
    .mutex_unlock = NULL, \
    .use_accel = true, \
    .accel_fifo = false, \
    .accel_dlpf = ICM20602_ACCEL_DLPF_10_2_HZ, \
    .accel_g = ICM20602_ACCEL_RANGE_4G, \
    .use_gyro = true, \
    .gyro_fifo = false, \
    .gyro_dlpf = ICM20602_GYRO_DLPF_10_HZ, \
    .gyro_dps = ICM20602_GYRO_RANGE_2000_DPS, \
    .sample_rate_div = 100, \
    .i2c_disable = false, \
  }

/***** Typedefs *****/

/** \brief Function pointer for write function.
  * \param id the ID value of the icm20602 device struct
  * \param reg ICM20602 register address to target
  * \param data pointer to data to write
  * \param len number of bytes to write
  * \return zero on success, anything else is an error
  */
typedef int8_t (*icm20602_hal_wr)(uint8_t id, uint8_t reg, uint8_t * data,
  uint16_t len);

/** \brief Function pointer for read function.
  * \param id the ID value of the icm20602 device struct
  * \param reg ICM20602 register address to target
  * \param data pointer to data to read
  * \param len number of bytes to read
  * \return zero on success, anything else is an error
  */
typedef int8_t (*icm20602_hal_rd)(uint8_t id, uint8_t reg, uint8_t * data,
  uint16_t len);

/** \brief Function pointer for sleep function.
  * \param ms the total number of milliseconds to sleep for
  * \return void
  */
typedef void (*icm20602_hal_sleep)(uint32_t ms);

/** \brief Function pointer for mutex locking function.
  * \param id the ID value of the icm20602 device struct
  * \return void
  */
typedef void (*icm20602_mutex_lock)(uint8_t id);

/** \brief Function pointer for mutex unlocking function.
  * \param id the ID value of the icm20602 device struct
  * \return void
  */
typedef void (*icm20602_mutex_unlock)(uint8_t id);

/***** Enums *****/

/** Enumerated value corresponds with A_DLPF_CFG in the ACCEL_CONFIG2 register
  * unless BYPASS is specified in the name. If BYPASS is used, the DLPF is
  * removed from the signal path and ACCEL_FCHOICE_B is set in the
  * ACCEL_CONFIG2 register. */
enum icm20602_accel_dlpf {
  ICM20602_ACCEL_DLPF_218_1_HZ = 0, // data clocked at 1kHz
  ICM20602_ACCEL_DLPF_99_HZ = 2, // data clocked at 1kHz
  ICM20602_ACCEL_DLPF_44_8_HZ = 3, // data clocked at 1kHz
  ICM20602_ACCEL_DLPF_21_2_HZ = 4, // data clocked at 1kHz
  ICM20602_ACCEL_DLPF_10_2_HZ = 5, // data clocked at 1kHz
  ICM20602_ACCEL_DLPF_5_1_HZ = 6, // data clocked at 1kHz
  ICM20602_ACCEL_DLPF_420_HZ = 7, // data clocked at 1kHz
  ICM20602_ACCEL_DLPF_BYPASS_1046_HZ, // no filter, data clocked at 4kHz
};

/** Enumerated value corresponds with ACCEL_FS_SEL in the ACCEL_CONFIG
  * register. Values listed are the full +/- G range. */
enum icm20602_accel_g {
  ICM20602_ACCEL_RANGE_2G = 0,
  ICM20602_ACCEL_RANGE_4G = 1,
  ICM20602_ACCEL_RANGE_8G = 2,
  ICM20602_ACCEL_RANGE_16G = 3,
};

/** Enumerated value corresponds with DLPF_CFG in the CONFIG register unless
  * BYPASS is specified in the name. If BYPASS is used, the DLPF is removed
  * from the signal path and FCHOICE_B is set in GYRO_CONFIG register. */
enum icm20602_gyro_dlpf {
  ICM20602_GYRO_DLPF_250_HZ = 0, // data clocked at 8kHz
  ICM20602_GYRO_DLPF_176_HZ = 1, // data clocked at 1kHz
  ICM20602_GYRO_DLPF_92_HZ = 2, // data clocked at 1kHz
  ICM20602_GYRO_DLPF_41_HZ = 3, // data clocked at 1kHz
  ICM20602_GYRO_DLPF_20_HZ = 4, // data clocked at 1kHz
  ICM20602_GYRO_DLPF_10_HZ = 5, // data clocked at 1kHz
  ICM20602_GYRO_DLPF_5_HZ = 6, // data clocked at 1kHz
  ICM20602_GYRO_DLPF_3281_HZ = 7, // data clocked at 8kHz
  ICM20602_GYRO_DLPF_BYPASS_3281_HZ, // no filter, data clocked at 32kHz
  ICM20602_GYRO_DLPF_BYPASS_8173_HZ, // no filter, data clocked at 32kHz
};

/** Enumerated value corresponds with FS_SEL in the GYRO_CONFIG register.
  * Values listed are the full +/- DPS range. */
enum icm20602_gyro_dps {
  ICM20602_GYRO_RANGE_250_DPS = 0,
  ICM20602_GYRO_RANGE_500_DPS = 1,
  ICM20602_GYRO_RANGE_1000_DPS = 2,
  ICM20602_GYRO_RANGE_2000_DPS = 3,
};

/***** Structs *****/

struct icm20602_dev {
  /// Identifier, can be I2C address, SPI CS line, or some other unique value.
  uint8_t id;

  /// Required function pointer for register write function.
  icm20602_hal_wr hal_wr;
  /// Required function pointer for register read function.
  icm20602_hal_rd hal_rd;
  /// Required function pointer for system sleep/delay.
  icm20602_hal_sleep hal_sleep;

  /// Optional function pointer to mutex lock if needed, NULL otherwise.
  icm20602_mutex_lock mutex_lock;
  /// Optional function pointer to mutex unlocking if needed, NULL otherwise.
  icm20602_mutex_lock mutex_unlock;

  /// Set to "true" to configure the accelerometer.
  bool use_accel;
  /// Enable or disable fifo for accelerometer.
  bool accel_fifo;
  /// Select the digital low pass filter to use with the accelerometer.
  enum icm20602_accel_dlpf accel_dlpf;
  /// Select the accelerometer's g-force range.
  enum icm20602_accel_g accel_g;

  /// Set to "true" to configure the gyroscope.
  bool use_gyro;
  /// Enable or disable fifo for gyroscope.
  bool gyro_fifo;
  /// Select the digital low pass filter to use with the gyroscope.
  enum icm20602_gyro_dlpf gyro_dlpf;
  /// Select the gyroscope's degrees per second range.
  enum icm20602_gyro_dps gyro_dps;

  /// Divides the data clock for both the accelerometer and gyroscope.
  uint8_t sample_rate_div;

  /// Disable hardware I2C communications to chip, recommeded if using SPI.
  bool i2c_disable;
};

/***** Global Functions *****/

/** \brief Initializes the ICM20602 sensor.
  * \param config pointer to configuration struct
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_init(struct icm20602_dev * dev);

/** \brief Reads current G-force values of accelerometer.
  * \param p_x destination for x G value
  * \param p_y destination for y G value
  * \param p_z destination for z G value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_accel(struct icm20602_dev * dev, float * p_x, float * p_y,
  float * p_z);

/** \brief Reads current degrees per second values of gyroscope.
  * \param p_x destination for x value
  * \param p_y destination for y value
  * \param p_z destination for z value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_gyro(struct icm20602_dev * dev, float * p_x, float * p_y,
  float * p_z);

/** \brief Reads current values of accelerometer and gyroscope.
  * \param p_ax destination for accelerometer x G value
  * \param p_ay destination for accelerometer y G value
  * \param p_az destination for accelerometer z G value
  * \param p_gx destination for gyroscope x DPS value
  * \param p_gy destination for gyroscope y DPS value
  * \param p_gz destination for gyroscope z DPS value
  * \param p_t destination for temperature degrees C value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_data(struct icm20602_dev * dev, float * p_ax, float * p_ay,
  float * p_az, float * p_gx, float * p_gy, float * p_gz, float * p_t);

/** \brief Reads current raw values of accelerometer.
  * \param p_x destination for x value
  * \param p_y destination for y value
  * \param p_z destination for z value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_accel_raw(struct icm20602_dev * dev, int16_t * p_x, int16_t * p_y,
  int16_t * p_z);

/** \brief Reads current raw values of gyroscope.
  * \param p_x destination for x value
  * \param p_y destination for y value
  * \param p_z destination for z value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_gyro_raw(struct icm20602_dev * dev, int16_t * p_x, int16_t * p_y,
  int16_t * p_z);

/** \brief Reads current raw values of accelerometer and gyroscope.
  * \param p_ax destination for accelerometer x value
  * \param p_ay destination for accelerometer y value
  * \param p_az destination for accelerometer z value
  * \param p_gx destination for gyroscope x value
  * \param p_gy destination for gyroscope y value
  * \param p_gz destination for gyroscope z value
  * \param p_t destination for temperature value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_data_raw(struct icm20602_dev * dev, int16_t * p_ax,
  int16_t * p_ay, int16_t * p_az, int16_t * p_gx, int16_t * p_gy,
  int16_t * p_gz, int16_t * p_t);

/** \brief Reads FIFO G-force values of accelerometer.
  * \param p_x destination for x G value
  * \param p_y destination for y G value
  * \param p_z destination for z G value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_accel_fifo(struct icm20602_dev * dev, float * x, float * y,
  float * z);

/** \brief Reads FIFO degrees per second values of gyroscope.
  * \param p_x destination for x value
  * \param p_y destination for y value
  * \param p_z destination for z value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_gyro_fifo(struct icm20602_dev * dev, float * x, float * y,
  float * z);

/** \brief Reads FIFO values of accelerometer and gyroscope. Note, both
  *        accelerometer and gyroscope fifos should be enabled if this
  *        function is to be used.
  * \param p_ax destination for accelerometer x G value
  * \param p_ay destination for accelerometer y G value
  * \param p_az destination for accelerometer z G value
  * \param p_gx destination for gyroscope x DPS value
  * \param p_gy destination for gyroscope y DPS value
  * \param p_gz destination for gyroscope z DPS value
  * \param p_t destination for temperature degrees C value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_fifo_data(struct icm20602_dev * dev, float * p_ax, float * p_ay,
  float * p_az, float * p_gx, float * p_gy, float * p_gz, float * p_t);

/** \brief Reads FIFO raw values of accelerometer.
  * \param p_x destination for x value
  * \param p_y destination for y value
  * \param p_z destination for z value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_fifo_accel_raw(struct icm20602_dev * dev, int16_t * p_x,
  int16_t * p_y, int16_t * p_z);

/** \brief Reads FIFO raw values of gyroscope.
  * \param p_x destination for x value
  * \param p_y destination for y value
  * \param p_z destination for z value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_fifo_gyro_raw(struct icm20602_dev * dev, int16_t * p_x,
  int16_t * p_y, int16_t * p_z);

/** \brief Reads FIFO raw values of accelerometer and gyroscope. Note, both
  *        accelerometer and gyroscope fifos should be enabled if this
  *        function is to be used.
  * \param p_ax destination for accelerometer x value
  * \param p_ay destination for accelerometer y value
  * \param p_az destination for accelerometer z value
  * \param p_gx destination for gyroscope x value
  * \param p_gy destination for gyroscope y value
  * \param p_gz destination for gyroscope z value
  * \param p_t destination for temperature value
  * \return zero on success, anything else is an error
  */
extern int8_t
icm20602_read_fifo_data_raw(struct icm20602_dev * dev, int16_t * p_ax,
  int16_t * p_ay, int16_t * p_az, int16_t * p_gx, int16_t * p_gy,
  int16_t * p_gz, int16_t * p_t);

#endif
