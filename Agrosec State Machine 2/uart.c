#include "stm32f10x.h"                  // Device header
#include "uart.h"

void USART_Init(USART_TypeDef* uartPort,
								uint16_t baud,
								uint8_t dmaMode,
								uint8_t uartMode)
{
	/*
	Description:
	Initializes USART peripheral
	
	Parameters:
	1.) uartPort: pointer to USART_TypeDef struct which contains
	all registers for the desired USART peripheral
	e.g. if uartPort is passed an argument of USART1, the USART1 peripheral
	is configured and if passed an argument of USART2, the USART2 peripheral
	is configured etc.
	
	2.) baud: configures the baud rate of the USART peripheral. If this parameter
	is passed BAUD_9600, the baud rate is set to 9600 baud. 
	
	3.) dmaMode: enables DMA mode for transmission or reception. If passed
	USART_DMA_RX_ENABLE, the DMA is enabled for reception and if passed
	USART_DMA_TX_ENABLE, the DMA is enabled for transmisson. If both
	transmitter and receiver DMA modes are required, USART_DMA_RX_ENABLE
	and USART_DMA_TX_ENABLE must be logically ORed.
	
	4.) uartMode: enables USART for transmission or reception. If this
	parameter is passed USART_TX_ENABLE, the USART transmitter is
	enabled and if passed with USART_RX_ENABLE, the USART receiver is 
	enabled. If both transmitter and receiver are required, the USART_TX_ENABLE
	and USART_RX_ENABLE must be logically ORed.
	
	Return:
	None
	
	*/
	uartPort->CR1 |= USART_CR1_UE;
	
	switch (dmaMode)
	{
		case USART_TX_DMA_DISABLE:
			uartPort->CR3 &= ~USART_DMA_TX_ENABLE;
			break;
		case USART_RX_DMA_DISABLE:
			uartPort->CR3 &= ~USART_DMA_RX_ENABLE;
			break;
	}
	
	uartPort->CR3 |= dmaMode;
	uartPort->BRR = baud;
	uartPort->CR1 |= uartMode;
}

void USART_Transmit_Byte(USART_TypeDef* uartPort, char byte)
{
	/*
	Description:
	Transmits a byte of data through the Tx pin of
	the configured USART peripheral
	
	Parameters:
	1.) uartPort:  pointer to USART_TypeDef struct which contains
	all registers for the desired USART peripheral
	e.g. if uartPort is passed an argument of USART1, the USART1 peripheral
	is configured and if passed an argument of USART2, the USART2 peripheral
	is configured etc.
	
	2.) byte: Data to be transmitted. The data to be transmitted
	is a character or byte.
	
	Return:
	None
	
	*/
	
	while ( (uartPort->SR & USART_SR_TXE) != USART_SR_TXE);
	uartPort->DR = byte;
	
}

void USART_Transmit_String(USART_TypeDef* uartPort, char* pString)
{
	/*
	Description:
	Transmits multiple bytes of data through the Tx pin of
	the configured USART peripheral
	
	Parameters:
	1.) uartPort:  pointer to USART_TypeDef struct which contains
	all registers for the desired USART peripheral
	e.g. if uartPort is passed an argument of USART1, the USART1 peripheral
	is configured and if passed an argument of USART2, the USART2 peripheral
	is configured etc.
	
	2.) pString: pointer to data to be transmitted. The data to be transmitted
	is an array of characters excluding the 'NULL' character.
	
	Return:
	None
	
	*/
	uint32_t i = 0;
	
	while (pString[i] != '\0')
	{
		while ( (uartPort->SR & USART_SR_TXE) != USART_SR_TXE);
		uartPort->DR = pString[i];
		i++;
	}
	
}

void USART_Transmit_String_With_Null(USART_TypeDef* uartPort, char* pString)
{
	/*
	Description:
	Transmits multiple bytes of data through the Tx pin of
	the configured USART peripheral
	
	Parameters:
	1.) uartPort:  pointer to USART_TypeDef struct which contains
	all registers for the desired USART peripheral
	e.g. if uartPort is passed an argument of USART1, the USART1 peripheral
	is configured and if passed an argument of USART2, the USART2 peripheral
	is configured etc.
	
	2.) pString: pointer to data to be transmitted. The data to be transmitted
	is an array of characters including the 'NULL' character.
	
	Return:
	None
	
	*/
	
	uint32_t i = 0;
	
	while (pString[i] != '\0')
	{
		while ( (uartPort->SR & USART_SR_TXE) != USART_SR_TXE);
		uartPort->DR = pString[i];
		i++;
	}
	
	if (pString[i] == '\0')
	{
		while ( (uartPort->SR & USART_SR_TXE) != USART_SR_TXE);
		uartPort->DR = pString[i];
	}
	
}

char USART_Rx_Char_Blocking(USART_TypeDef* uartPort)
{
	while ( (uartPort->SR & USART_SR_RXNE) != USART_SR_RXNE );
	return uartPort->DR;
}

char USART_Rx_Char_NonBlocking(USART_TypeDef* uartPort)
{
	char uartData = '\0';
	
	if ( (uartPort->SR & USART_SR_RXNE) == USART_SR_RXNE )
	{
		uartData = uartPort->DR;
	}
	return uartData;
}



