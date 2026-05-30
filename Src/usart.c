/*******************************************************************************
 * USART MODULE
 *******************************************************************************
 * @file           : usart.c
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

#include "usart.h"

uint8_t command_flag = 0;

void usart2_rx_init(){

	//Configure PA3 for USART2 RX
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	GPIOA->MODER &= ~(3U << 6);
	GPIOA->MODER |= (2U << 6);

	GPIOA->PUPDR &= ~(3U << 6);
	GPIOA->PUPDR |= (1U << 6);

	GPIOA->AFR[0] &= ~(0xF << 12);
	GPIOA->AFR[0] |= (7U << 12);

	//Configure USART2 for RX only
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	USART2 -> CR1 = 0;
	USART2 -> CR1 |= (1U << 2);
	USART2 -> CR1 |= (1U << 5);
	USART2->ISR &= ~(1U << 5);       // clear Recv-Not-Empty flag
	USART2 -> CR1 |= (1U << 0);
	USART2 -> CR2 = 0;
	USART2 -> CR3 = 0;

	USART2 -> BRR = 0x1A1;

	NVIC->ISER[1] = (1U << (USART2_IRQn & 0x1FU));
	__enable_irq();


}

/* -----------------------------------------------------------------------------
 * function : USART2_IRQHandler( )
 * INs      : none
 * OUTs     : none
 * action   : interfaces with wireless com port
 * authors  : Arjunan Easwarachandran (ace)
 * version  : 1
 * date     : 260515
 * -------------------------------------------------------------------------- */
void USART2_IRQHandler( void  ) {

   uint8_t charRecv;
   if (USART2->ISR & USART_ISR_RXNE) {
   	USER_LED_write(1);
      charRecv = USART2->RDR;
		switch(charRecv) {
		case FORWARD :
			command_flag = FORWARD;
			break;
		case BACKWARD :
			command_flag = BACKWARD;
			break;
		case BALANCE:
			command_flag = BALANCE;
			break;
		case STOP:
			command_flag = STOP;
			break;
		}
	}
}
