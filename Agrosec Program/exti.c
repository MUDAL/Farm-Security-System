#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "exti.h"

#define NUMBER_OF_EXTI_LINES		4

static volatile bool edgeDetected[ NUMBER_OF_EXTI_LINES ];

void EXTI_Init(uint8_t extIntLine, bool edgeTrigger)
{
	/*
	Description:
	
	Parameters:
	1.)extIntLine
	
	2.)edgeTrigger: if this parameter is passed 'EXTI_RISING_EDGE' or 'true',
	the rising trigger is enabled otherwise if passed 'EXTI_FALLING_EDGE'
	or 'false', the falling trigger is enabled.
	
	Return:
	
	*/
	EXTI->IMR |= (1<<extIntLine); //unmask EXT interrupt line
	
	if (edgeTrigger)
	{
		EXTI->RTSR |= (1<<extIntLine); //enable rising trigger for EXT interrupt line
	}
	
	else
	{
		EXTI->FTSR |= (1<<extIntLine); //enable falling trigger for EXT interrupt line
	}
	
}

void EXTI_Set_Edge_Detected(uint8_t extIntLine, bool isDetected)
{
	edgeDetected[extIntLine] = isDetected;
}

bool EXTI_Get_Edge_Detected(uint8_t extIntLine)
{
	return edgeDetected[extIntLine];
}

void EXTI0_IRQHandler(void)
{
	if ( (EXTI->PR & EXTI_PR_PR0) == EXTI_PR_PR0 )
	{
		EXTI->PR |= EXTI_PR_PR0; //clear pending register bit
	}
	edgeDetected[EXTI_LINE0] = true;
}
	
