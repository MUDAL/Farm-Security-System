#ifndef _UART_H
#define _UART_H

enum Baud
{
	BAUD_9600 = 0x341,
	BAUD_115200 = 0x45
};

#define USART_TX_ENABLE						USART_CR1_TE
#define USART_RX_ENABLE						USART_CR1_RE
#define USART_DMA_RX_ENABLE				USART_CR3_DMAR
#define USART_DMA_TX_ENABLE				USART_CR3_DMAT

#define USART_TX_DMA_DISABLE			0x00
#define USART_RX_DMA_DISABLE			0x01		

//Initializes the USART of choice
extern void USART_Init(USART_TypeDef* uartPort, uint16_t baud, uint8_t dmaMode, uint8_t uartMode);
//Transmits single byte serial data
extern void USART_Transmit_Byte(USART_TypeDef* uartPort, char byte);
//Transmits multibyte serial data (excluding NULL) through a configured USART peripheral 
extern void USART_Transmit_String(USART_TypeDef* uartPort, char* pString);
//Transmits multibyte serial data (including NULL) through a configured USART peripheral
extern void USART_Transmit_String_With_Null(USART_TypeDef* uartPort, char* pString);
extern char USART_Rx_Char_Blocking(USART_TypeDef* uartPort);
extern char USART_Rx_Char_NonBlocking(USART_TypeDef* uartPort);

#endif //_UART_H
