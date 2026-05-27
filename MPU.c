/* USER CODE BEGIN Header */
/*******************************************************************************
*
*******************************************************************************
* @file : MPU.c
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

#include "MPU.h"

// -----------------------------------------------------------------------------
// calibration offsets
// -----------------------------------------------------------------------------

static int16_t ax_offset = 0;
static int16_t ay_offset = 0;
static int16_t az_offset = 0;

static int16_t gx_offset = 0;
static int16_t gy_offset = 0;
static int16_t gz_offset = 0;


// -----------------------------------------------------------------------------
// moving average buffers
// -----------------------------------------------------------------------------

static int16_t ax_buf[MPU9250_AVG_LEN] = {0};
static int16_t ay_buf[MPU9250_AVG_LEN] = {0};
static int16_t az_buf[MPU9250_AVG_LEN] = {0};

static int16_t gx_buf[MPU9250_AVG_LEN] = {0};
static int16_t gy_buf[MPU9250_AVG_LEN] = {0};
static int16_t gz_buf[MPU9250_AVG_LEN] = {0};

static uint8_t avg_idx = 0;

// -----------------------------------------------------------------------------
// angle static variable: Assumes robot starts in upright orientation
// -----------------------------------------------------------------------------
static int32_t stored_angle = 0;

// -----------------------------------------------------------------------------
// function : MPU9250_init
// -----------------------------------------------------------------------------

void MPU9250_init(void) {

	GPIO_init(GPIOF, 0);
	GPIO_init(GPIOF, 1);

	GPIOF->OTYPER |= (1U << 0) | (1U << 1);

	GPIOF->AFR[0] &= ~(GPIO_AFRL_AFSEL0_Msk | GPIO_AFRL_AFSEL1_Msk);
	GPIOF->AFR[0] |=  (4 << GPIO_AFRL_AFSEL0_Pos);
	GPIOF->AFR[0] |=  (4 << GPIO_AFRL_AFSEL1_Pos);

	GPIOF->MODER &= ~(GPIO_MODER_MODE0_Msk | GPIO_MODER_MODE1_Msk);
	GPIOF->MODER |=  (GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1);

	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN;

	I2C2->CR1 &= ~(I2C_CR1_PE);
	I2C2->CR1 &= ~(I2C_CR1_ANFOFF);
	I2C2->CR1 &= ~(I2C_CR1_DNF);

	I2C2->TIMINGR = 0x00100D14;

	I2C2->CR2 |= I2C_CR2_AUTOEND;
	I2C2->CR2 &= ~(I2C_CR2_ADD10);

	I2C2->CR1 |= I2C_CR1_PE;

	MPU9250_write(MPU9250_PWR_MGMT_1, 0x00);
}


// -----------------------------------------------------------------------------
// function : MPU9250_read
// -----------------------------------------------------------------------------

uint8_t MPU9250_read(uint8_t reg) {

	uint8_t data;

	I2C2->CR2 &= ~(I2C_CR2_RD_WRN);
	I2C2->CR2 &= ~(I2C_CR2_NBYTES);
	I2C2->CR2 |=  (1 << I2C_CR2_NBYTES_Pos);

	I2C2->CR2 &= ~(I2C_CR2_SADD);
	I2C2->CR2 |=  (MPU9250_ADDRESS << (I2C_CR2_SADD_Pos + 1));

	I2C2->CR2 |= I2C_CR2_START;

	while (!(I2C2->ISR & I2C_ISR_TXIS));

	I2C2->TXDR = reg;

	while (!(I2C2->ISR & I2C_ISR_STOPF));

	I2C2->ICR = I2C_ICR_STOPCF;


	I2C2->CR2 |= I2C_CR2_RD_WRN;

	I2C2->CR2 &= ~(I2C_CR2_NBYTES);
	I2C2->CR2 |=  (1 << I2C_CR2_NBYTES_Pos);

	I2C2->CR2 &= ~(I2C_CR2_SADD);
	I2C2->CR2 |=  (MPU9250_ADDRESS << (I2C_CR2_SADD_Pos + 1));

	I2C2->CR2 |= I2C_CR2_START;

	while (!(I2C2->ISR & I2C_ISR_RXNE));

	data = I2C2->RXDR & 0xFF;

	while (!(I2C2->ISR & I2C_ISR_STOPF));

	I2C2->ICR = I2C_ICR_STOPCF;

	return data;
}


// -----------------------------------------------------------------------------
// function : MPU9250_write
// -----------------------------------------------------------------------------

void MPU9250_write(uint8_t reg, uint8_t data) {

	I2C2->CR2 &= ~(I2C_CR2_RD_WRN);

	I2C2->CR2 &= ~(I2C_CR2_NBYTES);
	I2C2->CR2 |=  (2 << I2C_CR2_NBYTES_Pos);

	I2C2->CR2 &= ~(I2C_CR2_SADD);
	I2C2->CR2 |=  (MPU9250_ADDRESS << (I2C_CR2_SADD_Pos + 1));

	I2C2->CR2 |= I2C_CR2_START;

	while (!(I2C2->ISR & I2C_ISR_TXIS));

	I2C2->TXDR = reg;

	while (!(I2C2->ISR & I2C_ISR_TXIS));

	I2C2->TXDR = data;

	while (!(I2C2->ISR & I2C_ISR_STOPF));

	I2C2->ICR = I2C_ICR_STOPCF;
}


// -----------------------------------------------------------------------------
// function : MPU9250_read_bytes
// -----------------------------------------------------------------------------

void MPU9250_read_bytes(uint8_t reg, uint8_t *data, uint8_t len) {

	I2C2->CR2 &= ~(I2C_CR2_RD_WRN);

	I2C2->CR2 &= ~(I2C_CR2_NBYTES);
	I2C2->CR2 |=  (1 << I2C_CR2_NBYTES_Pos);

	I2C2->CR2 &= ~(I2C_CR2_SADD);
	I2C2->CR2 |=  (MPU9250_ADDRESS << (I2C_CR2_SADD_Pos + 1));

	I2C2->CR2 |= I2C_CR2_START;

	while (!(I2C2->ISR & I2C_ISR_TXIS));

	I2C2->TXDR = reg;

	while (!(I2C2->ISR & I2C_ISR_STOPF));

	I2C2->ICR = I2C_ICR_STOPCF;


	I2C2->CR2 |= I2C_CR2_RD_WRN;

	I2C2->CR2 &= ~(I2C_CR2_NBYTES);
	I2C2->CR2 |=  (len << I2C_CR2_NBYTES_Pos);

	I2C2->CR2 &= ~(I2C_CR2_SADD);
	I2C2->CR2 |=  (MPU9250_ADDRESS << (I2C_CR2_SADD_Pos + 1));

	I2C2->CR2 |= I2C_CR2_START;

	for (uint8_t i = 0; i < len; i++) {

		while (!(I2C2->ISR & I2C_ISR_RXNE));

		data[i] = I2C2->RXDR & 0xFF;
	}

	while (!(I2C2->ISR & I2C_ISR_STOPF));

	I2C2->ICR = I2C_ICR_STOPCF;
}


// -----------------------------------------------------------------------------
// function : MPU9250_read_sensor
// -----------------------------------------------------------------------------

void MPU9250_read_sensor(MPU9250_Data_t *data) {

	uint8_t raw[14];

	MPU9250_read_bytes(MPU9250_ACCEL_XOUT_H, raw, 14);

	data->ax = (int16_t)((raw[0]  << 8) | raw[1]);
	data->ay = (int16_t)((raw[2]  << 8) | raw[3]);
	data->az = (int16_t)((raw[4]  << 8) | raw[5]);

	data->gx = (int16_t)((raw[8]  << 8) | raw[9]);
	data->gy = (int16_t)((raw[10] << 8) | raw[11]);
	data->gz = (int16_t)((raw[12] << 8) | raw[13]);
}


// -----------------------------------------------------------------------------
// function : MPU9250_calibrate
// -----------------------------------------------------------------------------

void MPU9250_calibrate(void) {

	MPU9250_Data_t data;

	int32_t ax_sum = 0;
	int32_t ay_sum = 0;
	int32_t az_sum = 0;

	int32_t gx_sum = 0;
	int32_t gy_sum = 0;
	int32_t gz_sum = 0;

	for (uint16_t i = 0; i < MPU9250_CAL_SAMPLES; i++) {

		MPU9250_read_sensor(&data);

		ax_sum += data.ax;
		ay_sum += data.ay;
		az_sum += data.az;

		gx_sum += data.gx;
		gy_sum += data.gy;
		gz_sum += data.gz;

		for (volatile uint32_t d = 0; d < 10000; d++);
	}

	ax_offset = ax_sum / MPU9250_CAL_SAMPLES;
	ay_offset = ay_sum / MPU9250_CAL_SAMPLES;

	az_offset = (az_sum / MPU9250_CAL_SAMPLES) - 16384;

	gx_offset = gx_sum / MPU9250_CAL_SAMPLES;
	gy_offset = gy_sum / MPU9250_CAL_SAMPLES;
	gz_offset = gz_sum / MPU9250_CAL_SAMPLES;
}


// -----------------------------------------------------------------------------
// function : MPU9250_moving_average
// -----------------------------------------------------------------------------

void MPU9250_moving_average(MPU9250_Data_t *data) {

	data->ax -= ax_offset;
	data->ay -= ay_offset;
	data->az -= az_offset;

	data->gx -= gx_offset;
	data->gy -= gy_offset;
	data->gz -= gz_offset;


	ax_buf[avg_idx] = data->ax;
	ay_buf[avg_idx] = data->ay;
	az_buf[avg_idx] = data->az;

	gx_buf[avg_idx] = data->gx;
	gy_buf[avg_idx] = data->gy;
	gz_buf[avg_idx] = data->gz;


	int32_t ax_sum = 0;
	int32_t ay_sum = 0;
	int32_t az_sum = 0;

	int32_t gx_sum = 0;
	int32_t gy_sum = 0;
	int32_t gz_sum = 0;

	for (uint8_t i = 0; i < MPU9250_AVG_LEN; i++) {

		ax_sum += ax_buf[i];
		ay_sum += ay_buf[i];
		az_sum += az_buf[i];

		gx_sum += gx_buf[i];
		gy_sum += gy_buf[i];
		gz_sum += gz_buf[i];
	}

	data->ax = ax_sum / MPU9250_AVG_LEN;
	data->ay = ay_sum / MPU9250_AVG_LEN;
	data->az = az_sum / MPU9250_AVG_LEN;

	data->gx = gx_sum / MPU9250_AVG_LEN;
	data->gy = gy_sum / MPU9250_AVG_LEN;
	data->gz = gz_sum / MPU9250_AVG_LEN;


	avg_idx++;

	if (avg_idx >= MPU9250_AVG_LEN) {
		avg_idx = 0;
	}
}

/* -----------------------------------------------------------------------------
 * function : MPU9250_get_angle( )
 * INs      : sensor data struct, and loop duration
 * OUTs     : angle of robot, as a double
 * action   : Initializes LPUART1 on the Nucleo board
 * authors  : EE 329 Lab Manual
 * 			  Arjunan Easwarachandran (ace)
 * version  : 1
 * date     : 260502
 * -------------------------------------------------------------------------- */
int32_t MPU9250_get_angle(MPU9250_Data_t *data, int32_t dt) {
	// Calculate angle with accelerometer
    int32_t acc_angle = (int32_t)(
        atan2f((float)data->ax, (float)data->az) * (180000.0f / PI)
    );

    // Calculate angle with gyroscope
    int32_t gyro_delta = (int32_t)(
        ((int64_t)data->gy * dt * 250) / (32767 * 1000)
    );

    int32_t gyro_angle = stored_angle + gyro_delta;

    // Apply complementary filter
    stored_angle = gyro_angle + (((acc_angle - gyro_angle) * ALPHA) >> 7);

    return stored_angle;
}


