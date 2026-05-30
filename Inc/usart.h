/*******************************************************************************
 * USART MODULE
 *******************************************************************************
 * @file           : usart.h
 * @brief          : Provides USART2 functionality
 * project         : EE 329 S'26 Custom Project
 * authors         : Calab Fernandez - cferna50@calpoly.edu
 * 						Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version         : 1
 * date            : 260529
 * IDE	          : STM32CubeIDE v.1.19.0
 * target          : NUCLEO-L4A6ZG
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * REVISION HISTORY
 * 1 260502 (ace) created file and added simple USART functionality
 ******************************************************************************/

#ifndef INC_USART_H_
#define INC_USART_H_

#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "led.h"

// Macros
#define LPUARTDIV 	(417)	// Divisor to set baud rate
#define LPUART_PORT	(GPIOG)	// Port for LPUART pins
#define ESC				(0x1B)	// Escape character
#define BS				(0x7F)	// Backspace
#define ENTER			(0x0D)	// Enter (carriage return)
#define NULL_TERM		(0x00)	// \0
#define FORWARD		(0x77)	// Move forward command 'w'
#define BACKWARD		(0x73)	// Move backward command 's'
#define BALANCE		(0x20)	// Move balance command [space]
#define STOP			(0x70)	// Stop command 'p'

extern uint8_t command_flag;

void usart2_rx_init();


#endif /* INC_USART_H_ */
