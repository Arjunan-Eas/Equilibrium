/*******************************************************************************
 * MOTOR MODULE
 *******************************************************************************
 * @file           : motor.h
 * @brief          : integrates L298N motor driver with the Nucleo STM32L4A6ZG
 * 						to drive two DC motors
 * project         : EE 329 S'26 Custom Project
 * authors         : Arjunan Easwarachandran (ace)
 * version         : 1
 * date            : 260513
 * IDE	          : STM32CubeIDE v.1.19.0
 * target          : NUCLEO-L4A6ZG
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * REVISION HISTORY
 * 1 260513 (ace) created file
 ******************************************************************************/

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

// Includes
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include "timer.h"

// L298N Pins and ports
#define ENA				(GPIO_PIN_7)
#define EN_PORT		(GPIOF)
#define IN1				(GPIO_PIN_4)
#define IN2				(GPIO_PIN_5)
#define MOTOR_A_PORT	(GPIOC)
#define IN3				(GPIO_PIN_2)
#define IN4				(GPIO_PIN_6)
#define MOTOR_B_PORT	(GPIOB)
#define ENB				(GPIO_PIN_8)

// Timer constants
#define PWM_FREQUENCY   (4000)     	// Set desired frequency for motor driver
#define BASE_CLOCK		(4000000)	// 4 MHz base clock

// Function prototypes
void Motor_init(void);		// Initialize GPIO for motors
void setup_TIM5_PWM(void);	// Initialize TIM5 for PWM outputs
void Motor_write(int16_t speed1, int16_t speed2);	// Control motor output

#endif /* INC_MOTOR_H_ */
