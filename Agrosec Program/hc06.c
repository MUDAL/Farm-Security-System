#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "gpio.h"
#include "uart.h"
#include "sysTimer_struct.h"
#include "systick.h"
#include "dma.h"
#include "hc06.h"

#if !HC06_USE_INTERRUPTS
#define HC06_RX_TIMEOUT_MS				400
static sysTimer_t privHc06RxTimer;
#endif

void HC06_Tx_Init(void)
{
	
	//GPIO configuration for USART1 Tx
	GPIO_Output_Init(GPIOA,
									 GPIO_PORT_REG_HIGH,
									 GPIO_PIN9_OUTPUT_MODE_2MHZ,
									 GPIO_PIN9_ALT_FUNC_PUSH_PULL);
	
	//USART1 configuration
	USART_Init(USART1,
						 BAUD_9600,
						 USART_TX_DMA_DISABLE,
						 USART_TX_ENABLE);
	
}

void HC06_Rx_Init(char* hc06RxBuffer)
{
	/*
	Description:
	
	Parameters:
	
	Return:
	
	*/
	
	//GPIO configuration for USART1 Rx
	GPIO_Input_Init(GPIOA,
									GPIO_PORT_REG_HIGH,
									GPIO_PIN10,
									GPIO_PIN10_INPUT_PULLUP_OR_PULLDOWN,
									GPIO_PULLUP_ENABLE);
	
	//USART1 configuration
	USART_Init(USART1,
						 BAUD_9600,
						 USART_DMA_RX_ENABLE,
						 USART_RX_ENABLE);
	
	#if HC06_USE_INTERRUPTS
	//DMA1 channel 5 configuration for USART1 Rx
	DMA_USART_Rx_Init(DMA1_Channel5,
									  USART1,
									  hc06RxBuffer,
									  HC06_RX_BUFFER_SIZE, 
									  DMA_CHANNEL5_MEMORY_INC_MODE |
										DMA_CHANNEL5_CIRCULAR_BUFFER |
										DMA_CHANNEL5_TC_INT_ENABLE |
										DMA_CHANNEL5_ENABLE);
										
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	#else
	//DMA1 channel 5 configuration for USART1 Rx
	DMA_USART_Rx_Init(DMA1_Channel5,
									  USART1,
									  hc06RxBuffer,
									  HC06_RX_BUFFER_SIZE, 
									  DMA_CHANNEL5_MEMORY_INC_MODE);
	//Initialize alarm for HC06 module
	privHc06RxTimer.start = 0;
	privHc06RxTimer.ticksToWait = HC06_RX_TIMEOUT_MS;
	privHc06RxTimer.isCounting = false;
	#endif
	
}
	
void HC06_Transmit(char* pData)
{
	/*
	Description:
	
	Parameters:
	
	Return:
	
	*/
	
	USART_Transmit_String(USART1,pData);
}

bool HC06_Rx_Done_Receiving(void)
{
	
	#if HC06_USE_INTERRUPTS
	return DMA_Rx_InterruptReady(DMA1, DMA_CHANNEL5);
	#else
	bool RxDoneReceiving = false;
	
	if (!privHc06RxTimer.isCounting)
	{
		if (DMA_Rx_Is_Receiving(DMA1,
														DMA1_Channel5,
														DMA_CHANNEL5_CLEAR_TC_INT_FLAG,
														HC06_RX_BUFFER_SIZE))
		{
			privHc06RxTimer.start = SysTick_GetTick();
			privHc06RxTimer.isCounting = true;
		}
	}
	
	else
	{
		if ( SysTick_Timer_Done_Counting(&privHc06RxTimer) )
		{
			DMA_Rx_Restart(DMA1,
										 DMA1_Channel5,
										 DMA_CHANNEL5_CLEAR_TC_INT_FLAG);
			
			privHc06RxTimer.start = 0;
			privHc06RxTimer.isCounting = false;
			RxDoneReceiving = true;
		}
	}
	return RxDoneReceiving;
	#endif
}

void HC06_Rx_Restart(void)
{
	#if HC06_USE_INTERRUPTS
	DMA_Rx_Clear_Interrupt(DMA1,DMA_CHANNEL5);
	#endif
}


