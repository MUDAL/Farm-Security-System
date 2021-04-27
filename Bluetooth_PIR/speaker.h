#ifndef _SPEAKER_H
#define _SPEAKER_H

#define SPEAKER_FREQ_800HZ				800
#define SPEAKER_FREQ_1KHZ					1000
#define SPEAKER_FREQ_3KHZ					3000
#define SPEAKER_FREQ_6KHZ					6000
#define SPEAKER_FREQ_15KHZ				15000
#define SPEAKER_FREQ_30KHZ				30000

#define SPEAKER_DUTY_CYCLE_45PERCENT		45
#define SPEAKER_DUTY_CYCLE_65PERCENT		65
#define SPEAKER_DUTY_CYCLE_90PERCENT		90

extern void Speaker_Init(void);
extern void Speaker_Activate(uint16_t freq, uint8_t dutyCycle);
extern void Speaker_Deactivate(void);

#endif //_SPEAKER_H
