#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "gpio.h"
#include "afio.h"
#include "exti.h"

void PIR_Init(void)
{
	/*PA0 GPIO configuration*/
	
	/*AFIO configuration for PA0*/
	
	/*EXTI configuration for PA0*/
	
	/*EXTI interrupt enable*/
}

bool PIR_Motion_Detected(void)
{
	return EXTI_Get_Edge_Detected(EXTI_LINE0);
}

void PIR_Restart(void)
{
	EXTI_Set_Edge_Detected(EXTI_LINE0, false);
}
