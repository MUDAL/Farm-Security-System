#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "gptm.h"

void GPTM_Init(TIM_TypeDef* timerPort,
							 uint16_t prescale,
							 uint16_t reload, 
							 bool interruptEn)
{
	timerPort->PSC = prescale;
	timerPort->ARR = reload;
	if (interruptEn)
	{
		timerPort->DIER |= TIM_DIER_UIE; //interrupt
	}
	timerPort->CR1 |= TIM_CR1_CEN;
}

void GPTM_PWM_Init(TIM_TypeDef* timerPort, 
									 uint16_t prescale,
									 uint16_t reload,
									 uint16_t dutyCycle)
{
	/*Configuration for channel 1 
	defaults:
	* channel 1 is configured as output
	* OC1 pin is active high
	*/
	timerPort->PSC = prescale;
	timerPort->ARR = reload;
	timerPort->CCR1 = dutyCycle;
	timerPort->CCMR1 |= ( TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 ); //enable PWM1
	timerPort->CCMR1 |= TIM_CCMR1_OC1PE; //enable preload register
	timerPort->CR1 |= TIM_CR1_ARPE; //Buffer the auto-reload register
	timerPort->CCER |= TIM_CCER_CC1E; //enable output capture
	timerPort->CR1 |= TIM_CR1_CEN; //enable timer's counter
}

void GPTM_Control(TIM_TypeDef* timerPort, bool timerEn)
{
	if (timerEn)
	{
		timerPort->CR1 |= TIM_CR1_CEN;
	}
	else
	{
		timerPort->CCMR1 &= ~( TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 ); //enable PWM1
		timerPort->CCMR1 &= ~TIM_CCMR1_OC1PE; //enable preload register
		timerPort->CR1 &= ~TIM_CR1_ARPE; //Buffer the auto-reload register
		timerPort->CCER &= ~TIM_CCER_CC1E; //enable output capture
		timerPort->CR1 &= ~TIM_CR1_CEN;
	}
}	
