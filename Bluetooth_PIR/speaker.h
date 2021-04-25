#ifndef _SPEAKER_H
#define _SPEAKER_H

#define	SPEAKER_ON  							true
#define	SPEAKER_OFF  							false
	
#define SPEAKER_FREQ_1KHZ					1000
#define SPEAKER_FREQ_6KHZ					6000
#define SPEAKER_FREQ_15KHZ				15000

#define SPEAKER_DUTY_CYCLE_45PERCENT		45
#define SPEAKER_DUTY_CYCLE_65PERCENT		65

extern void Speaker_Init(uint16_t freq, uint8_t dutyCycle);
extern void Speaker_Control(bool speakerState);

#endif //_SPEAKER_H
