#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sysTimer_struct.h"
#include "system.h"
#include "hc06.h"
#include "PIR.h"
#include "sim800l.h"
#include "speaker.h"

//private global variables
static sysTimer_t sim800lTimer;

//private functions
static bool SendSMS(char* msg);

int main(void)
{
	
	static char hc06ReceiveBuffer[HC06_RX_BUFFER_SIZE];
	
	System_Init();
	System_Alarm_Init(&sim800lTimer,50);
	PIR_Init();
	HC06_Tx_Init();
	HC06_Rx_Init(hc06ReceiveBuffer);
	Speaker_Init(SPEAKER_FREQ_6KHZ, SPEAKER_DUTY_CYCLE_65PERCENT);
	SIM800L_Tx_Init();
	SIM800L_Transmit_String("AT+CMGF=1\r\n");
	System_Timer_DelayMs(50);
	
	while(1)
	{
		
		//Test code for speaker
		
//		Speaker_Control(SPEAKER_ON);
//		System_Timer_DelayMs(3000);
//		Speaker_Control(SPEAKER_OFF);
//		System_Timer_DelayMs(3000);
		
//		if(HC06_Rx_Done_Receiving())
//		{
//			if(!strcmp(hc06ReceiveBuffer,"Baby"))
//			{
//				HC06_Transmit("Maybe I'm in love\n");
//			}
//			else if (!strcmp(hc06ReceiveBuffer,"TEL class"))
//			{
//				HC06_Transmit("It's not worth it\n");
//			}
//			else
//			{
//				HC06_Transmit("Do not break the bro code by simping\n");
//			}
//		}
//		
//		if (PIR_Motion_Detected())
//		{
//			PIR_Restart();
//			/*Add code to send bluetooth message to raspberry pi*/
//		}
		
	}
	
}

bool SendSMS(char* msg)
{
	static uint8_t currentState = STATE_SEND_PHONE_NO;
	const uint8_t ctrlZ = 26; 
	bool doneSendingSMS = false;
	
	switch (currentState)
	{
		case STATE_SEND_PHONE_NO:
			SIM800L_Transmit_String("AT+CMGS=\"+2348167351641\"\r\n");
			currentState = STATE_SEND_MESSAGE;
			break;
		
		case STATE_SEND_MESSAGE:
			if (System_Alarm_Ready(&sim800lTimer))
			{
				SIM800L_Transmit_String(msg);
				currentState = STATE_SEND_CTRL_Z;
			}
			break;
			
		case STATE_SEND_CTRL_Z:
			if (System_Alarm_Ready(&sim800lTimer))
			{
				SIM800L_Transmit_Byte(ctrlZ);
				currentState = STATE_SEND_PHONE_NO;
				doneSendingSMS = true;
			}
			break;
	}
	
	return doneSendingSMS;
	
}
