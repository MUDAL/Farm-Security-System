#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "gpio.h"

void PIR_Init(void)
{
	/*PA0 GPIO configuration*/
	GPIO_Input_Init(GPIOA,
									GPIO_PORT_REG_LOW,
									GPIO_PIN0,
									GPIO_PIN0_INPUT_FLOATING,
									false);
}

bool PIR_Logic_Level(void)
{
	return GPIO_Input_Read(GPIOA, GPIO_PIN0);
}
