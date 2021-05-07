#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "sysTimer_struct.h"
#include "systick.h"
#include "dma.h"

//Generic DMA Channel enable
#define  DMA_CCR_EN		((uint16_t)0x0001)

#define NO_OF_DMA1_CHANNELS					7
#define NO_OF_DMA2_CHANNELS					5

static volatile bool dma1Channel[ NO_OF_DMA1_CHANNELS ];
static volatile bool dma2Channel[ NO_OF_DMA2_CHANNELS ];

void DMA_USART_Rx_Init(DMA_Channel_TypeDef* dmaChannel,
											 USART_TypeDef* uartPort,
											 char* uartRxBuffer, 
											 uint32_t bufferSize,
											 uint32_t dmaConfig)
{
	/*
	Description:
	Initializes a USART Rx DMA channel
	
	Parameters:
	1.) dmaChannel: pointer to DMA_Channel_TypeDef struct which contains
	all registers for the desired DMA channel
	e.g. if dmaChannel is passed an argument of DMA1_Channel5, DMA1 Channel 5
	is configured and if passed an argument of DMA2_Channel1, DMA2 Channel 1
	is configured.
	
	2.) uartPort: pointer to USART_TypeDef struct which contains
	all registers for the desired USART peripheral
	e.g. if uartPort is passed an argument of USART1, the USART1 peripheral
	is configured and if passed an argument of USART2, the USART2 peripheral
	is configured etc.
	
	3.) uartRxBuffer: pointer to first element of character array (or buffer) which stores
	data from the configured USART's receiver via the configured DMA channel. e.g. 
	if dmaChannel = DMA1_Channel5 and uartPort = USART1, USART1 receives 
	multiple byte data which will be directly transferred to memory(i.e. uartRxBuffer) 
	via DMA1 channel 5.
	
	4.) bufferSize: size of uartRxBuffer (i.e. number of bytes or characters to receive from USART Rx
	via DMA transfer). e.g. if this parameter is passed with an argument of 25, it means 25 bytes
	of data will be directly transferred from the USART Rx to memory (i.e. uartRxBuffer).
	
	5.) dmaConfig: this parameter enables different types of configurations for a DMA channel.
	The possible arguments that can be passed to this parameter are defined in the dma.h header
	and they can be logically ORed depending on the desired configuration. Some of these 
	arguments (in generic form) include:
	
	- DMA_CHANNELX_MEMORY_INC_MODE: Enables memory increment mode for channel 'x' of
	a DMA peripheral
	
	- DMA_CHANNELX_CLEAR_TC_INT_FLAG: Clears transfer conplete interrupt flag for channel 'x' of
	a DMA peripheral
	
	- DMA_CHANNELX_CIRCULAR_BUFFER: Enables circular buffer mode for channel 'x' of a DMA peripheral
	
	- DMA_CHANNELX_TC_INT_ENABLE: Enables transfer complete interrupt for channel 'x' of a DMA
	peripheral
	
	- DMA_CHANNELX_ENABLE	: Enables channel 'x' of a DMA peripheral
	
	Return:
	None
	
	*/ 
	volatile uint32_t* pUart_DR = (uint32_t*)&uartPort->DR;
	
	dmaChannel->CPAR = (uint32_t)pUart_DR;
	dmaChannel->CMAR = (uint32_t)uartRxBuffer;
	dmaChannel->CNDTR = bufferSize;
	dmaChannel->CCR |= dmaConfig;
}

bool DMA_Rx_Is_Receiving(DMA_TypeDef* dmaPort, 
												 DMA_Channel_TypeDef* dmaChannel,
								         uint32_t dmaClearInterruptFlag,
								         uint32_t numberOfBytesToReceive)

{

	bool dmaActive = false;
	
	dmaPort->IFCR |= dmaClearInterruptFlag;
	dmaChannel->CNDTR = numberOfBytesToReceive;
	dmaChannel->CCR |= DMA_CCR_EN; 
	
	if (dmaChannel->CNDTR != numberOfBytesToReceive)
	{ 
		dmaActive = true;
	}
	
	return dmaActive;
}

void DMA_Rx_Restart(DMA_TypeDef* dmaPort, 
										DMA_Channel_TypeDef* dmaChannel,
								    uint32_t dmaClearInterruptFlag)
{
	dmaChannel->CCR &= ~DMA_CCR_EN;
	dmaPort->IFCR |= dmaClearInterruptFlag;
	dmaChannel->CNDTR = 0;
}

bool DMA_Rx_InterruptReady(DMA_TypeDef* dmaPort, uint8_t channelNumber)
{
	if (dmaPort == DMA1)
	{
		return dma1Channel[channelNumber - 1];
	}
	
	else
	{
		if (channelNumber > DMA_CHANNEL5)
		{ //DMA2 doesn't have channels 6 and 7
			return false;
		}
		else
		{
			return dma2Channel[channelNumber - 1];
		}
	}
}

void DMA_Rx_Clear_Interrupt(DMA_TypeDef* dmaPort, uint8_t channelNumber)
{
	if (dmaPort == DMA1)
	{
		dma1Channel[channelNumber - 1] = false;
	}
	
	else
	{
		if (channelNumber > DMA_CHANNEL5)
		{ //DMA2 doesn't have channels 6 and 7
			return;
		}
		else
		{
			dma2Channel[channelNumber - 1] = false;
		}
	}
	
}

void DMA1_Channel3_IRQHandler(void)
{
	DMA1->IFCR |= DMA_IFCR_CTCIF3;
}

void DMA1_Channel5_IRQHandler(void)
{
	DMA1->IFCR |= DMA_IFCR_CTCIF5;
	dma1Channel[DMA_CHANNEL5 - 1] = true;
}

void DMA1_Channel6_IRQHandler(void)
{
	DMA1->IFCR |= DMA_IFCR_CTCIF6;
}
