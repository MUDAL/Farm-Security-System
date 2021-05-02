#ifndef _AFIO_H
#define _AFIO_H

#define AFIO_EXTI_SELECT_PA0				AFIO_EXTICR1_EXTI0_PA

//Selects source input for an external interrupt line
extern void AFIO_Init(uint8_t extIntLine, uint16_t afioConfig);

#endif //_AFIO_H
