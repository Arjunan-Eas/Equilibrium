/*******************************************************************************
 * LED MODULE
 *******************************************************************************
 * @file           : led.h
 * @brief          : Interface for user LED
 * project         : EE 329 S'26 Assignment 9
 * authors         : Arjunan Easwarachandran (ace) - aeaswara@calpoly.edu
 * version         : 2
 * date            : 260412
 * IDE	          : STM32CubeIDE v.1.19.0
 * target          : NUCLEO-L4A6ZG
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * REVISION HISTORY
 * 1 260412 (ace) created header file and defined constants
 * 2 260514	(ace) removed all external LED functionality
 ******************************************************************************/

#ifndef INC_LED_H_
#define INC_LED_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>

// User LED related constants
#define USER_LED_PORT	(GPIOB)
#define USER_LED			(GPIO_PIN_7)	// Blue LED

void USER_LED_init(void);	// Initialize user LED
void USER_LED_write(uint8_t val);	// Write to the blue onboard LED

#endif /* INC_LED_H_ */
