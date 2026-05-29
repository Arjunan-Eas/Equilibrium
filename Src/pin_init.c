/* USER CODE BEGIN Header */
/*******************************************************************************
*
*******************************************************************************
* @file : pin_init.c
* @brief :
* project : EE 329 S'26 A4
* authors : Cody Carmichael (czc) - czcarmi@calpoly.edu
* version : 0.1
* date : Apr 24, 2026
* compiler : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
* target : NUCLEO-L4A6ZG
* clocks : 4 MHz MSI to AHB2
* @attention : (c) 2025 STMicroelectronics. All rights reserved.
*******************************************************************************
*
*
*******************************************************************************
* WIRING (pinout NUCLEO-L4A6ZG = L496ZG)
*
*******************************************************************************
* REVISION HISTORY
* 0.1 Apr 24, 2026 czc created
******************************************************************************/
/* USER CODE END Header */

//includes
#include "pin_init.h"


/* -----------------------------------------------------------------------------
* function : GPIO_init
* INs : GPIO port, pin number
* OUTs : N/A
* action : Clear all relevant GPIO reg values and set it to high speed
* authors : Cody Carmichael (czc) - czcarmic@calpoly.edu
* version : 0.1
* date : Apr 24, 2026
* -------------------------------------------------------------------------- */
void GPIO_init (GPIO_TypeDef *port, uint8_t pin) {
   //this port stuff is from chat
	if (port == GPIOA) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
   else if (port == GPIOB) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
   else if (port == GPIOC) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
   else if (port == GPIOD) RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
   else if (port == GPIOE) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
   else if (port == GPIOF) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
   else if (port == GPIOG) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;

   // clear mode bits for this pin
	// 00 input
	// 01 output
	// 10 alternate function
	// 11 analog
   port->MODER &= ~(3U << (2 * pin));

   // set push-pull
   // 0 push-pull
   // 1 open-drain
   port->OTYPER &= ~(1U << pin);

   // no pull-up / pull-down
   // 00: No pull-up, pull-down
   // 01: Pull-up
   // 10: Pull-down
   // 11: Reserved
   port->PUPDR &= ~(3U << (2 * pin));

   // very high speed
   // 00 low
   // 01 medium
   // 10 high
   // 11 very high
   port->OSPEEDR |=  (3U << (2 * pin));

   // default low
   port->BRR = (1U << pin);
}
