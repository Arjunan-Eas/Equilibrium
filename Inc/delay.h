/*******************************************************************************
 * DELAY MODULE
 *******************************************************************************
 * @file           : delay.c
 * @brief          : uses SysTick timer to create controllable delays
 * project         : EE 329 S'26 Assignment 3
 * authors         : Arjunan Easwarachandran (ace) - aeaswara@calpoly.edu
 * version         : 1
 * date            : 260412
 * IDE	          : STM32CubeIDE v.1.19.0
 * target          : NUCLEO-L4A6ZG
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * REVISION HISTORY
 * 1 260412 (ace) created file, includes, and function prototypes
 ******************************************************************************/

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include <stdint.h>
#include "stm32l4xx_hal.h"

// Function prototypes
void SysTick_Init(void);
void delay_us(const uint32_t time_us);
void delay_sec(const int seconds);	// Creates a delay function does seconds

#endif /* INC_DELAY_H_ */
