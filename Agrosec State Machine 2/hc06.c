#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "gpio.h"
#include "uart.h"
#include "sysTimer_struct.h"
#include "systick.h"
#include "hc06.h"

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

void HC06_Rx_Init(void)
{
	//GPIO configuration for USART1 Rx
	GPIO_Input_Init(GPIOA,
									GPIO_PORT_REG_HIGH,
									GPIO_PIN10,
									GPIO_PIN10_INPUT_PULLUP_OR_PULLDOWN,
									GPIO_PULLUP_ENABLE);
	
	//USART1 configuration
	USART_Init(USART1,
						 BAUD_9600,
						 USART_RX_DMA_DISABLE,
						 USART_RX_ENABLE);
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


char HC06_Receive_Char_Blocking(void)
{
	return USART_Rx_Char_Blocking(USART1);
}

char HC06_Receive_Char_NonBlocking(void)
{
	return USART_Rx_Char_NonBlocking(USART1);
}

