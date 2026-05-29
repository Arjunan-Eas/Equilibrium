/* USER CODE BEGIN Header */
/*******************************************************************************
*
*******************************************************************************
* @file : pin_init.h
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
#ifndef INC_PIN_INIT_H_
#define INC_PIN_INIT_H_

#include "stm32l4xx_hal.h"

//function declares

void GPIO_init (GPIO_TypeDef*, uint8_t);


#endif /* INC_PIN_INIT_H_ */
