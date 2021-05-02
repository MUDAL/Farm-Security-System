#include "stm32f10x.h"                  // Device header
#include "afio.h"

void AFIO_Init(uint8_t extIntLine, uint16_t afioConfig)
{
	if (extIntLine <= 3)
	{
		AFIO->EXTICR[0] = afioConfig;
	}
	
	else if (extIntLine >= 4 && extIntLine <= 7)
	{
		AFIO->EXTICR[1] = afioConfig;
	}
	
	else if (extIntLine >= 8 && extIntLine <= 11)
	{
		AFIO->EXTICR[2] = afioConfig;
	}
	
	else if (extIntLine >= 12 && extIntLine <= 15)
	{
		AFIO->EXTICR[3] = afioConfig;
	}
	
	else
	{
		return;
	}

}
