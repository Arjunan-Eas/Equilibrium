/* USER CODE BEGIN Header */
/*******************************************************************************
* 
*******************************************************************************
* @file : MPU.h
* @brief : 
* project : EE 329 S'26 CP
* authors : Cody Carmichael (czc) - czcarmi@calpoly.edu
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

// -----------------------------------------------------------------------------
// MPU9250 definitions
// -----------------------------------------------------------------------------

#define MPU9250_ADDRESS          0x68

#define MPU9250_WHO_AM_I         0x75
#define MPU9250_PWR_MGMT_1       0x6B
#define MPU9250_ACCEL_XOUT_H     0x3B

#define MPU9250_CAL_SAMPLES      200
#define MPU9250_AVG_LEN          5


// -----------------------------------------------------------------------------
// MPU9250 data structure
// -----------------------------------------------------------------------------

typedef struct {

	int16_t ax;
	int16_t ay;
	int16_t az;

	int16_t temp;

	int16_t gx;
	int16_t gy;
	int16_t gz;

} MPU9250_Data_t;


// -----------------------------------------------------------------------------
// function prototypes
// -----------------------------------------------------------------------------

void MPU9250_init(void);

uint8_t MPU9250_read(uint8_t reg);

void MPU9250_write(uint8_t reg, uint8_t data);

void MPU9250_read_bytes(uint8_t reg, uint8_t *data, uint8_t len);

void MPU9250_read_sensor(MPU9250_Data_t *data);

void MPU9250_calibrate(void);

void MPU9250_moving_average(MPU9250_Data_t *data);


#endif /* INC_MPU_H_ */
