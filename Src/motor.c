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


#include "motor.h"

/* -----------------------------------------------------------------------------
 * function : Motor_init( )
 * INs      : none
 * OUTs     : none
 * action   : initialize GPIO for L298N motor driver
 * authors  : EE 329 Lab Manual
 * 			  Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 1
 * date     : 260513
 * -------------------------------------------------------------------------- */
void Motor_init(void) {

	// Enable port B, C, and F
		RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOBEN |
								  RCC_AHB2ENR_GPIOCEN |
								  RCC_AHB2ENR_GPIOFEN );

		// Set INx to output
		MOTOR_A_PORT->MODER 	&= ~(GPIO_MODER_MODE4 |
									  	  GPIO_MODER_MODE5);

		MOTOR_A_PORT->MODER 	|=  (GPIO_MODER_MODE4_0 |
									  	  GPIO_MODER_MODE5_0);

		MOTOR_B_PORT->MODER 	&= ~(GPIO_MODER_MODE2 |
									  	  GPIO_MODER_MODE6);

		MOTOR_B_PORT->MODER 	|=  (GPIO_MODER_MODE2_0 |
									  	  GPIO_MODER_MODE6_0);

		// Configure the alternate function on EN pins for PMW output from TIM5
		EN_PORT->AFR[0]	&= ~(GPIO_AFRL_AFSEL7_Msk);
		EN_PORT->AFR[0]	|=  (GPIO_AFRL_AFSEL7_1);
		EN_PORT->AFR[1]	&= ~(GPIO_AFRH_AFSEL8_Msk);
		EN_PORT->AFR[1]	|=  (GPIO_AFRH_AFSEL8_1);

		// Push/pull
		MOTOR_A_PORT->OTYPER &= ~(GPIO_OTYPER_OT4 |
			  	  	  	  	    	  	  GPIO_OTYPER_OT5);
		MOTOR_B_PORT->OTYPER &=	~(GPIO_OTYPER_OT2 |
 	  	    	  	  	  	  	  	     GPIO_OTYPER_OT6);
		EN_PORT->OTYPER &= ~(GPIO_OTYPER_OT7 |
								   GPIO_OTYPER_OT8);

		// No PUPD
		MOTOR_A_PORT->PUPDR	&= ~(GPIO_PUPDR_PUPD4 |
								  	  	  GPIO_PUPDR_PUPD5);
		MOTOR_B_PORT->PUPDR	&= ~(GPIO_PUPDR_PUPD2 |
								  	  	  GPIO_PUPDR_PUPD6);
		EN_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD7 |
		  	  	  	  	  	  	  GPIO_PUPDR_PUPD8);

		// Very high speed
		MOTOR_A_PORT->OSPEEDR	&= ~(GPIO_OSPEEDR_OSPEED4_Msk |
									  	  	  GPIO_OSPEEDR_OSPEED5_Msk);
		MOTOR_B_PORT->OSPEEDR	&= ~(GPIO_OSPEEDR_OSPEED2_Msk |
									  	  	  GPIO_OSPEEDR_OSPEED6_Msk);
		EN_PORT->OSPEEDR	&= ~(GPIO_OSPEEDR_OSPEED7_Msk |
									  GPIO_OSPEEDR_OSPEED8_Msk);

		MOTOR_A_PORT->OSPEEDR	|= (GPIO_OSPEEDR_OSPEED4_Msk |
									  	  	 GPIO_OSPEEDR_OSPEED5_Msk);
		MOTOR_B_PORT->OSPEEDR	|= (GPIO_OSPEEDR_OSPEED2_Msk |
									  	  	 GPIO_OSPEEDR_OSPEED6_Msk);
		EN_PORT->OSPEEDR	|= (GPIO_OSPEEDR_OSPEED7_Msk |
									 GPIO_OSPEEDR_OSPEED8_Msk);

		// Set ENA and ENB to AF mode (PWM pins)
		EN_PORT->MODER	&=	~(GPIO_MODER_MODE7);
		EN_PORT->MODER	|=	(GPIO_MODER_MODE7_1);

		EN_PORT->MODER	&=	~(GPIO_MODER_MODE8);
		EN_PORT->MODER	|=	(GPIO_MODER_MODE8_1);

		// Initialize motors off
		MOTOR_A_PORT->BRR 	|= ( IN1 | IN2 );
		MOTOR_B_PORT->BRR 	|= ( IN3 | IN4 );

		// Setup TIM5 for PWM output
		setup_TIM5_PWM();
}


/* -----------------------------------------------------------------------------
 * function : setup_TIM5_PWM( )
 * INs      : none
 * OUTs     : none
 * action   : initialize TIM5 to provide two separate PWM signals on channels
 * 			  2 and 3.
 * authors  : ChatGPT
 * 			  Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 1
 * date     : 260516
 * -------------------------------------------------------------------------- */
void setup_TIM5_PWM(void) {
    // 1. Enable TIM5 clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;

    // 2. Reset timer configuration
    TIM5->CR1 = 0;
    TIM5->CCMR1 = 0;
    TIM5->CCER = 0;

    // 3. Set ARR for the desired PWM frequency given a 4 MHz base clock
    TIM5->ARR = BASE_CLOCK / PWM_FREQUENCY - 1;    // PWM period

    // 4. Set different duty cycles
    TIM5->CCR2 = 0;     // Initialize as 0 duty cycle
    TIM5->CCR3 = 0;     // Initiallize as 0 duty cycle

    // 5. Configure Channel 2 as PWM mode 1
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M;
    TIM5->CCMR1 |=  (6 << TIM_CCMR1_OC2M_Pos);   // PWM mode 1
    TIM5->CCMR1 |=  TIM_CCMR1_OC2PE;             // enable preload for CCR2

    // 6. Configure Channel 3 as PWM mode 1
    TIM5->CCMR2 &= ~TIM_CCMR2_OC3M;
    TIM5->CCMR2 |=  (6 << TIM_CCMR2_OC3M_Pos);   // PWM mode 1
    TIM5->CCMR2 |=  TIM_CCMR2_OC3PE;             // enable preload for CCR3

    // 7. Enable outputs for channels 2 and 3
    TIM5->CCER |= TIM_CCER_CC2E;
    TIM5->CCER |= TIM_CCER_CC3E;

    // 8. Enable auto-reload preload
    TIM5->CR1 |= TIM_CR1_ARPE;

    // 9. Force update event so registers load immediately
    TIM5->EGR |= TIM_EGR_UG;

    // 10. Start TIM5
    TIM5->CR1 |= TIM_CR1_CEN;
}

/* -----------------------------------------------------------------------------
 * function : Motor_write( speed[0], speed[1] )
 * INs      : speed[0] -> Motor A, speed[1] -> Motor B
 * OUTs     : none
 * action   : Update motor driver inputs to move motors a certain speed and
 * 			  direction. Speed must be between -1000 and 1000.
 * 			  where the sign is the direction.
 * authors  : Arjunan Easwarachandran - aeaswara@calpoly.edu
 * version  : 1
 * date     : 260516
 * -------------------------------------------------------------------------- */
void Motor_write(int16_t *speed) {
	// Motor A moving forward
	if(speed[0] > 0 && speed[0] <= 1000) {
		MOTOR_A_PORT->BSRR |= IN1;	// IN1 HI
		MOTOR_A_PORT->BRR  |= IN2;	// IN2 LO
		TIM5->CCR2 = speed[0];	// Sets duty cycle of PWM output
	// Motor A moving backward
	} else if(speed[0] < 0 && speed[0] >= -1000) {
		MOTOR_A_PORT->BRR  |= IN1;	// IN1 LO
		MOTOR_A_PORT->BSRR |= IN2;	// IN2 HI
		TIM5->CCR2 = (-1*speed[0]);	// Sets duty cycle of PWM output
	// Motor A stopped (braking)
	} else if(speed[0] == 0) {
		MOTOR_A_PORT->BSRR |= IN1;	// IN1 HI
		MOTOR_A_PORT->BSRR |= IN2;	// IN2 HI
		TIM5->CCR2 = BASE_CLOCK / PWM_FREQUENCY;	// Max duty cycle
	}

	// Motor B moving forward
	if(speed[1] > 0 && speed[1] <= 1000) {
		MOTOR_B_PORT->BSRR |= IN3;	// IN3 HI
		MOTOR_B_PORT->BRR  |= IN4;	// IN4 LO
		TIM5->CCR3 = speed[1];	// Sets duty cycle of PWM output
	// Motor B moving backward
	} else if(speed[1] < 0 && speed[1] >= -1000) {
		MOTOR_B_PORT->BRR  |= IN3;	// IN3 LO
		MOTOR_B_PORT->BSRR |= IN4;	// IN4 HI
		TIM5->CCR3 = (-1*speed[1]);	// Sets duty cycle of PWM output
	// Motor B stopped (braking)
	} else if(speed[1] == 0) {
		MOTOR_B_PORT->BSRR |= IN3;	// IN3 HI
		MOTOR_B_PORT->BSRR |= IN4;	// IN4 HI
		TIM5->CCR3 = BASE_CLOCK / PWM_FREQUENCY;	// Max duty cycle
	}

	// If an invalid speed is set, do not update the speed
}

