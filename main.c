/*******************************************************************************
 * EE 329 CUSTOM PROJECT: EQUILIBRIUM
 *******************************************************************************
 * @file           : main.h
 * @brief          : Communication and control algorithm for Project
 * 						Equilibrium, a two-wheeled balancing robot
 * project         : EE 329 S'26 Custom Project
 * authors         : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version         : 1
 * date            : 260516
 * IDE	          : STM32CubeIDE v.1.19.0
 * target          : NUCLEO-L4A6ZG
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * REVISION HISTORY
 * 1 260516 (ace) created file
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

// PID coefficients
static int32_t kp = 150;
static int32_t ki = 0;
static int32_t kd = 0;

// Accumulator for integral error
static int64_t integral_err = 0;

int main(void) {
	// Initializes system components and all peripherals
	HAL_Init();
	SystemClock_Config();
	SysTick_Init();
	Motor_init();
	MPU9250_init();
	LPUART_init();
	USER_LED_init();

	// Struct to hold IMU data and UART messages
	MPU9250_Data_t imu;
	char msg[128];

	// Initialize test pin
	GPIO_init(GPIOA, 2);
	GPIOA->MODER |= GPIO_MODER_MODE2_0;	// Output mode

	// Calibrate the IMU
	LPUART_ESC_print("[H");
	LPUART_print("Keep MPU flat and still...\r\n");
	delay_us(1000000);

	MPU9250_calibrate();

	LPUART_print("Calibration done\r\n");
	delay_us(500000);

	// err_angle[0] = prev angle, err_angle[1] = current angle
	int32_t err_angle[2] = {0, 0};

	// Seed moving average array
	for(int i = 0; i < 5; i++) {
		MPU9250_read_sensor(&imu);
		MPU9250_moving_average(&imu);
	}

	// Initial angle
	err_angle[1] = MPU9250_get_angle(&imu, LOOP_LENGTH);

	// Motor speed variables
	int16_t motor_speed[2] = {0,0};
	Motor_write(motor_speed);

	// Initial reference angle
	int32_t reference_angle = 0;

	// Clear screen
	LPUART_ESC_print("[2J");

	USER_LED_write(1);	// Turn on calibration done LED
	/* PID Control Loop */
	while (1) {

		// Read from IMU
		MPU9250_read_sensor(&imu);
		MPU9250_moving_average(&imu);

		// Shift over previous error term
		err_angle[0] = err_angle[1];

		// Get current angle
		err_angle[1] = MPU9250_get_angle(&imu, LOOP_LENGTH) - reference_angle;

		// Run PID loop to get motor speed
		pid_control(err_angle, motor_speed);
		Motor_write(motor_speed);

		LPUART_ESC_print("[H");
		sprintf(msg, "Angle [mdeg]: %.3f  \r\n", ((float)err_angle[1]) / 1000);
		LPUART_print(msg);
		sprintf(msg, "Control Effort: %d  \r\n", motor_speed[0]);
		LPUART_print(msg);

		delay_us(1000);

		// Turn off pin
		GPIOA->ODR ^= GPIO_PIN_2;
	}
}

/* -----------------------------------------------------------------------------
 * function : pid_control(int32_t *err, int32_t *speed)
 * INs      : error signal past and present term, and motor speed array
 * OUTs     : none
 * action   : Update motor driver inputs to move motors a certain speed based on
 * 			  the angle of the robot. PID controller.
 * authors  : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 1
 * date     : 260526
 * -------------------------------------------------------------------------- */
void pid_control(int32_t *err, int16_t *speed) {
    int64_t derivative_err;
    int64_t output;

    // mdeg/s, not mdeg/ms
    derivative_err = ((int64_t)(err[1] - err[0]) * 1000) / LOOP_LENGTH;

    // deg*s
    integral_err += ((int64_t)err[1] * LOOP_LENGTH) / 1000000;

    output =
        ((int64_t)kp * err[1]) +
        ((int64_t)ki * integral_err) +
        ((int64_t)kd * derivative_err);

    // Scale down for integer operations, and swap sign for motor direction
    output /= -1000;

    // Saturate at +/- 1000
    if (output > MOTOR_MAX) output = MOTOR_MAX;
    if (output < -1*MOTOR_MAX) output = -1*MOTOR_MAX;

    speed[0] = (int16_t)output;
    speed[1] = (int16_t)output;
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
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
#ifdef USE_FULL_ASSERT
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
