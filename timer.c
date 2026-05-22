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

#include "timer.h"

/* Private static variables */
// Flag for whether the reaction timer has timed out
static uint8_t TIM2_timeout;

/* -----------------------------------------------------------------------------
 * function : setup_TIM2( )
 * INs      : number of clock cycles for make a certain duty cycle
 * OUTs     : none
 * action   : initialize TIM2 parameters and configure (but do not enable)
 * 			  interrupts
 * authors  : EE 329 Lab Manual
 * 			  Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 3
 * date     : 260423
 * -------------------------------------------------------------------------- */
void setup_TIM2(void) {
   RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;           // enable clock for TIM2
   TIM2->DIER |= (TIM_DIER_UIE);							// enable overflow event gen
   TIM2->ARR = TIMER_CYCLES;                       // 10s timeout @ 4 MHz
   TIM2->CR1 |= (TIM_CR1_OPM);							// One shot mode
   TIM2->SR &=  ~(TIM_SR_UIF);							// clr IRQ flag in status reg
   NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F));     // set NVIC interrupt: 0x1F
}

/* -----------------------------------------------------------------------------
 * function : enable_TIM2( )
 * INs      : none
 * OUTs     : none
 * action   : reset counter, and enable IRQ
 * authors  : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 1
 * date     : 260424
 * -------------------------------------------------------------------------- */
void enable_TIM2(void) {
	TIM2->SR &= ~(TIM_SR_UIF);							// clr IRQ flag in status reg
	TIM2->CNT = 0;											// reset counter
	TIM2_timeout = 0;										// clr timeout flag
   __enable_irq();                              // global IRQ enable
   TIM2->CR1 |= TIM_CR1_CEN;                    // start TIM2 counter
}

/* -----------------------------------------------------------------------------
 * function : disable_TIM2( )
 * INs      : none
 * OUTs     : none
 * action   : turn off counter and disbale IRQ
 * authors  : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 1
 * date     : 260424
 * -------------------------------------------------------------------------- */
void disable_TIM2(void) {
	TIM2->SR &= ~(TIM_SR_CC1IF | TIM_SR_UIF);		// clr IRQ flag in status reg
   __disable_irq();                             // global IRQ disable
   TIM2->CR1 &= ~(TIM_CR1_CEN);                 // stop TIM2 counter
}

/* -----------------------------------------------------------------------------
 * function : TIM2_reaction_time( )
 * INs      : none
 * OUTs     : reaction time in milliseconds
 * action   : Convert the number of cycles into ms
 * authors  : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 1
 * date     : 260424
 * -------------------------------------------------------------------------- */
uint32_t TIM2_reaction_time(void) {
	uint32_t cycles = TIM2->CNT;

	uint32_t time_ms = cycles / CYC_TO_MS;
	return time_ms;
}

/* -----------------------------------------------------------------------------
 * function : TIM2_check_timeout( )
 * INs      : none
 * OUTs     : uint8_t flag
 * action   : return timeout flag
 * authors  : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 1
 * date     : 260425
 * -------------------------------------------------------------------------- */
uint8_t TIM2_check_timeout(void) {
	return TIM2_timeout;
}

/* -----------------------------------------------------------------------------
 * function : TIM2_IRQHandler( )
 * INs      : none
 * OUTs     : none
 * action   : drive one pin to time ISR execution, and use the other to output
 * 			  a 50% duty cycle square wave
 * authors  : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * 			  EE 329 Lab Manual
 * version  : 2
 * date     : 260423
 * -------------------------------------------------------------------------- */
void TIM2_IRQHandler(void) {
   // Timeout: Timer will not restart automatically, since one-shot mode
   if(TIM2->SR & TIM_SR_UIF) {				// triggered by overflow
   	TIM2->SR &= ~(TIM_SR_UIF);     		// manage the flag
   	// Set fail variable to true
   	TIM2_timeout = 1;							// Raise timeout flag
   }
}
