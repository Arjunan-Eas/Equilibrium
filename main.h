/*******************************************************************************
 * EE 329 CUSTOM PROJECT: EQUILIBRIUM
 *******************************************************************************
 * @file           : main.h
 * @brief          : Communication and control algorithm for Project
 * 						Equilibrium, a two-wheeled balancing robot
 * project         : EE 329 S'26 Custom Project
 * authors         : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version         : 1
 * date            : 260516
 * IDE	          : STM32CubeIDE v.1.19.0
 * target          : NUCLEO-L4A6ZG
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * REVISION HISTORY
 * 1 260516 (ace) created file
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include "motor.h"
#include "delay.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
