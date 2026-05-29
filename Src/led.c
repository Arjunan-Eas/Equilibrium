/*******************************************************************************
 * LED MODULE
 *******************************************************************************
 * @file           : led.c
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

#include "led.h"

void USER_LED_init(void) {

	// Enable port B
	RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOBEN);

	/* User LED */
	USER_LED_PORT->MODER &= ~(GPIO_MODER_MODE7);
	USER_LED_PORT->MODER |= (GPIO_MODER_MODE7_0);					// output
	USER_LED_PORT->OTYPER &= ~(GPIO_OTYPER_OT7);						// p/p
	USER_LED_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD7);						// no pupd
	USER_LED_PORT->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED7_Pos);	// vhs
	USER_LED_PORT->BRR |= USER_LED;										// preset off

}

/* -----------------------------------------------------------------------------
 * function : USER_LED_write( )
 * INs      : integer number (on/off signal)
 * OUTs     : none
 * action   : turns blue user LED on or off based on the input value
 * authors  : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 1
 * date     : 260424
 * -------------------------------------------------------------------------- */
void USER_LED_write(uint8_t val) {
	if(val > 0) {
		USER_LED_PORT->BSRR |= (USER_LED);	// Turn on
	} else {
		USER_LED_PORT->BRR |= (USER_LED);	// Turn off
	}
}
