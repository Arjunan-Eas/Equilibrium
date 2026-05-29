/* USER CODE BEGIN Header */
/*******************************************************************************
*
*******************************************************************************
* @file : MPU.h
* @brief :
* project : EE 329 S'26 CP
* authors : Cody Carmichael (czc) - czcarmi@calpoly.edu
* 				Arjunan Easwarachandran (ace) - aeaswara@calpoly.edu
* version : 0.1
* date : May 25, 2026
* compiler : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
* target : NUCLEO-L4A6ZG
* clocks : 4 MHz MSI to AHB2
* @attention : (c) 2025 STMicroelectronics. All rights reserved.
*******************************************************************************
*
*
*******************************************************************************
* WIRING (pinout NUCLEO-L4A6ZG = L496ZG)
*
*******************************************************************************
* REVISION HISTORY
* 0.1 May 25, 2026 czc created
******************************************************************************/
/* USER CODE END Header */




#ifndef INC_MPU_H_
#define INC_MPU_H_

#include "pin_init.h"
#include <math.h>

// -----------------------------------------------------------------------------
// MPU9250 definitions
// -----------------------------------------------------------------------------

#define MPU9250_ADDRESS          0x68

#define MPU9250_WHO_AM_I         0x75
#define MPU9250_PWR_MGMT_1       0x6B
#define MPU9250_ACCEL_XOUT_H     0x3B

#define MPU9250_CAL_SAMPLES      200
#define MPU9250_AVG_LEN          5

#define PI								(3.141593)
#define ALPHA							(100)		// Weight of accelerometer out of 128
#define RAD_TO_MDEG 					(180000.0 / PI)
#define GYRO_SCALE_NUM 				(250)
#define GYRO_SCALE_DEN 				(32767)


// -----------------------------------------------------------------------------
// MPU9250 data structure
// -----------------------------------------------------------------------------

typedef struct {

	int16_t ax;
	int16_t ay;
	int16_t az;

	int16_t gx;
	int16_t gy;
	int16_t gz;

} MPU9250_Data_t;


// -----------------------------------------------------------------------------
// function prototypes
// -----------------------------------------------------------------------------

void MPU9250_init(void);

//uint8_t MPU9250_read(uint8_t reg);

uint8_t MPU9250_write(uint8_t reg, uint8_t data);

uint8_t MPU9250_read_bytes(uint8_t reg, uint8_t *data, uint8_t len);

void MPU9250_read_sensor(MPU9250_Data_t *data);

void MPU9250_calibrate(void);

void MPU9250_moving_average(MPU9250_Data_t *data);

int32_t MPU9250_get_angle(MPU9250_Data_t *data, int32_t dt);

uint8_t MPU9250_verify_flag(uint32_t flag);

#endif /* INC_MPU_H_ */
