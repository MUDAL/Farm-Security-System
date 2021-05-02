#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "gpio.h"
#include "uart.h"
#include "sim800l.h"

void SIM800L_Tx_Init(void)
{		
	//GPIO configuration for USART3 Tx
	GPIO_Output_Init(GPIOB,
									 GPIO_PORT_REG_HIGH,
									 GPIO_PIN10_OUTPUT_MODE_2MHZ,
									 GPIO_PIN10_ALT_FUNC_PUSH_PULL);
	
	//USART3 configuration
	USART_Init(USART3,
						 BAUD_9600,
						 USART_TX_DMA_DISABLE,
						 USART_TX_ENABLE);
}

void SIM800L_Transmit_Byte(char data)
{
	USART_Transmit_Byte(USART3, data);
}

void SIM800L_Transmit_String(char* pData)
{
	USART_Transmit_String(USART3, pData);
}
