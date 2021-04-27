#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include <math.h>
#include "gpio.h"
#include "gptm.h"
#include "speaker.h"

#define	SPEAKER_ON  							true
#define	SPEAKER_OFF  							false

void Speaker_Init(void)
{
		
	//GPIO configuration
	GPIO_Output_Init(GPIOA,
									 GPIO_PORT_REG_LOW,
									 GPIO_PIN6_OUTPUT_MODE_2MHZ,
									 GPIO_PIN6_ALT_FUNC_PUSH_PULL);
}

void Speaker_Activate(uint16_t freq, uint8_t dutyCycle)
{
	
	if ((freq < SPEAKER_FREQ_800HZ) || (freq > SPEAKER_FREQ_30KHZ))
	{
		return;
	}
	
	//PWM configuration
	const uint16_t timerRegLoadVal = lroundf((float)GPTM_CLK_IN_8MHZ / (GPTM_TIM_PRESCALE_80 * freq));
	const uint16_t duty = lroundf((float)dutyCycle * timerRegLoadVal / 100);
	
	GPTM_PWM_Init(TIM3,
								GPTM_TIM_PRESCALE_80,
								timerRegLoadVal,
								duty); 
	
	GPTM_Control(TIM3, SPEAKER_ON);
}

void Speaker_Deactivate(void)
{
	GPTM_Control(TIM3, SPEAKER_OFF);
}

