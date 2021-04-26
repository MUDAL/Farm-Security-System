#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "gpio.h"
#include "afio.h"
#include "exti.h"

void PIR_Init(void)
{
	/*PA0 GPIO configuration*/
	GPIO_Input_Init(GPIOA,
									GPIO_PORT_REG_LOW,
									GPIO_PIN0,
									GPIO_PIN0_INPUT_PULLUP_OR_PULLDOWN,
									true);
	/*AFIO configuration for PA0*/
	AFIO_Init(EXTI_LINE0, AFIO_EXTI_SELECT_PA0);
	/*EXTI configuration for PA0*/
	EXTI_Init(EXTI_LINE0, EXTI_FALLING_EDGE);
	/*EXTI interrupt enable*/
	NVIC_EnableIRQ(EXTI0_IRQn);
}

bool PIR_Motion_Detected(void)
{
	return EXTI_Get_Edge_Detected(EXTI_LINE0);
}

void PIR_Restart(void)
{
	EXTI_Set_Edge_Detected(EXTI_LINE0, false);
}
