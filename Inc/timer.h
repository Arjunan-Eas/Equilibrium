/*******************************************************************************
 * TIMER MODULE
 *******************************************************************************
 * @file           : timer.h
 * @brief          : uses TIM2 to create two separate PWM signals to drive a
 * 						the speed of two DC motors through the L298N motor driver
 * project         : EE 329 S'26 Custom Project
 * authors         : Arjunan Easwarachandran (ace)
 * 						EE 329 Lab Manual
 * version         : 1
 * date            : 260513
 * IDE	          : STM32CubeIDE v.1.19.0
 * target          : NUCLEO-L4A6ZG
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * REVISION HISTORY
 * 1 260513 (ace) copied file over from Project A4
 ******************************************************************************/

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32l4xx_hal.h"

// Function Prototypes
void setup_TIM2(void);					// Init function for TIM2
void enable_TIM2(void);					// Functions to easily toggle TIM2
void disable_TIM2(void);				// End timer
uint32_t TIM2_reaction_time(void);	// Return the reaction time in ms
uint8_t TIM2_check_timeout(void);	// Check flag
void TIM2_IRQHandler(void);			// ISR handler

// Macros
#define BASE_CLK			(4000000)
#define CYC_TO_MS			(BASE_CLK / 1000)	// Conversion factor
#define TIMER_PORT 		(GPIOB)			// Port B
#define TIMER_CYCLES		(40000000)		// 10 seconds @ 40 MHz

#endif /* INC_TIMER_H_ */
