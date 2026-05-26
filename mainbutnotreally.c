/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {

   HAL_Init();
   SystemClock_Config();

	MPU9250_Data_t imu;
	uint8_t who;
	char msg[128];

	LPUART_init();

	LPUART_Print(CLEAR_SCREEN);
	LPUART_Print(CURSOR_HOME);
	LPUART_Print(FG_CYAN "MPU-9250 UART Display\r\n" ATTR_RESET);
	LPUART_Print("UART alive\r\n");

	MPU9250_init();

	who = MPU9250_read(MPU9250_WHO_AM_I);

	sprintf(msg, "WHO_AM_I = 0x%02X\r\n", who);
	LPUART_Print(msg);

	LPUART_Print("Keep MPU flat and still...\r\n");
	delay_us(1000000);

	MPU9250_calibrate();

	LPUART_Print("Calibration done\r\n");
	delay_us(500000);

	while (1) {
		MPU9250_read_sensor(&imu);
		MPU9250_moving_average(&imu);

		LPUART_Print(CURSOR_HOME);
		LPUART_Print(CLEAR_SCREEN);

		LPUART_Print(FG_CYAN "MPU-9250 LIVE DATA\r\n" ATTR_RESET);
		LPUART_Print("--------------------------------\r\n");

		sprintf(msg, "WHO_AM_I : 0x%02X\r\n\r\n", who);
		LPUART_Print(msg);

		sprintf(msg, "ACCEL X  : %6d\r\n", imu.ax);
		LPUART_Print(msg);

		sprintf(msg, "ACCEL Y  : %6d\r\n", imu.ay);
		LPUART_Print(msg);

		sprintf(msg, "ACCEL Z  : %6d\r\n\r\n", imu.az);
		LPUART_Print(msg);

		sprintf(msg, "TEMP RAW : %6d\r\n\r\n", imu.temp);
		LPUART_Print(msg);

		sprintf(msg, "GYRO X   : %6d\r\n", imu.gx);
		LPUART_Print(msg);

		sprintf(msg, "GYRO Y   : %6d\r\n", imu.gy);
		LPUART_Print(msg);

		sprintf(msg, "GYRO Z   : %6d\r\n", imu.gz);
		LPUART_Print(msg);

		delay_us(50000);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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
