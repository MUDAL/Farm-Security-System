#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include <math.h>
#include "gpio.h"
#include "gptm.h"
#include "speaker.h"

void Speaker_Init(uint16_t freq, uint8_t dutyCycle)
{
	
	if ((freq < SPEAKER_FREQ_1KHZ) || (freq > SPEAKER_FREQ_15KHZ))
	{
		return;
	}
	
	//GPIO configuration
	GPIO_Output_Init(GPIOA,
									 GPIO_PORT_REG_LOW,
									 GPIO_PIN0_OUTPUT_MODE_2MHZ,
									 GPIO_PIN0_ALT_FUNC_PUSH_PULL);
	
	//PWM configuration
	const uint16_t timerRegLoadVal = lroundf((float)GPTM_CLK_IN_8MHZ / (GPTM_TIM_PRESCALE_80 * freq));
	const uint16_t duty = lroundf((float)dutyCycle * timerRegLoadVal / 100);
	
	GPTM_PWM_Init(TIM2,
								GPTM_TIM_PRESCALE_80,
								timerRegLoadVal,
								duty); 
	
}

void Speaker_Control(bool speakerState)
{
	GPTM_Control(TIM2, speakerState);
}


