/*******************************************************************************
 * DELAY MODULE
 *******************************************************************************
 * @file           : delay.c
 * @brief          : uses SysTick timer to create controllable delays
 * project         : EE 329 S'26 Assignment 3
 * authors         : EE 329 Lab Manual
 * version         : 1
 * date            : 260412
 * IDE	          : STM32CubeIDE v.1.19.0
 * target          : NUCLEO-L4A6ZG
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * REVISION HISTORY
 * 1 260412 (ace) created file and copied code over from lab manual
 ******************************************************************************/

#include "delay.h"

// configure SysTick timer for use with delay_us().
// warning: breaks HAL_delay() by disabling interrupts for shorter delay timing.
void SysTick_Init(void) {
	SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk |     	// enable SysTick Timer
                      SysTick_CTRL_CLKSOURCE_Msk); 	// select CPU clock
	SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);  	// disable interrupt
}

// delay in microseconds using SysTick timer to count CPU clock cycles
// do not call with 0 : error, maximum delay.
// careful calling with small nums : results in longer delays than specified:
//	   e.g. @4MHz, delay_us(1) = 10-15 us delay.
void delay_us(const uint32_t time_us) {
	// set the counts for the specified delay
	SysTick->LOAD = (uint32_t)((time_us * (SystemCoreClock / 1000000)) - 1);
	SysTick->VAL = 0;                                  	 // clear timer count
	SysTick->CTRL &= ~(SysTick_CTRL_COUNTFLAG_Msk);    	 // clear count flag
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // wait for flag
}

// Repurposes the delay_us module to avoid the issue of delays with a higher
// value than the clock frequency
void delay_sec(int seconds) {
	if(seconds == 0) {
		return;
	}

	for(int i = 0; i < seconds; i++) {
		delay_us(1000000);	// Delay in 1s increment
	}
}
