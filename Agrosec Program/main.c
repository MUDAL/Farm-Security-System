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
static bool SendSMS(char* phoneNumber, char* message);

int main(void)
{
	//local variables
	static char hc06ReceiveBuffer[HC06_RX_BUFFER_SIZE];
	bool motionPrevDetected = false;

	//Initializations
	System_Init();
	System_Alarm_Init(&sim800lTimer,50);
	PIR_Init();
	HC06_Tx_Init();
	HC06_Rx_Init(hc06ReceiveBuffer);
	Speaker_Init();
	SIM800L_Tx_Init();

	while(1)
	{
		
		//Test code for speaker
//		Speaker_Activate(SPEAKER_FREQ_800HZ, SPEAKER_DUTY_CYCLE_65PERCENT);
//		System_Timer_DelayMs(5000);
//		Speaker_Deactivate();
//		System_Timer_DelayMs(5000);
		
		//Test code for bluetooth
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
		if (PIR_Motion_Detected())
		{
			if (!motionPrevDetected)
			{
				PIR_Restart();
				motionPrevDetected = true;
				/*Add code to send bluetooth message to raspberry pi*/
			}
		}
		
		else
		{
			if (motionPrevDetected)
			{
				/*Add code to interprete bluetooth data to determine whether
				SMS should be sent or not. [switch..case construct]*/
				
//				bool smsSent = SendSMS("+2348144086708","Go home");
//				
//				if (smsSent)
//				{
//					motionPrevDetected = false;
//				}
			}
		}
		
	}
	
}

bool SendSMS(char* phoneNumber, char* message)
{
	/*
	Description:
	Sends SMS to a number and indicates whether the..
	SMS was successfully sent or not.
	
	Parameters:
	1.) phoneNumber: phone number to send SMS to
	2.) message: SMS to send 
	
	Return:
	1.) true: if SMS has been successfully sent
	2.) false: if SMS hasn't been sent
	*/
	
	static char smsATCmd[27] = "AT+CMGS=\"";
	static uint8_t currentState = STATE_SEND_INIT_AT_CMD;
	bool doneSendingSMS = false;
	
	switch (currentState)
	{
		case STATE_SEND_INIT_AT_CMD:
			SIM800L_Transmit_String("AT+CMGF=1\r\n");
			currentState = STATE_SEND_PHONE_NO;
			break;
		
		case STATE_SEND_PHONE_NO:
			if (System_Alarm_Ready(&sim800lTimer))
			{
				strcat(smsATCmd, phoneNumber);
				strcat(smsATCmd, "\"\r\n");
				SIM800L_Transmit_String(smsATCmd);
				currentState = STATE_SEND_MESSAGE;
			}
			break;
		
		case STATE_SEND_MESSAGE:
			if (System_Alarm_Ready(&sim800lTimer))
			{
				SIM800L_Transmit_String(message);
				currentState = STATE_TERMINATE_CMD_LINE;
			}
			break;
			
		case STATE_TERMINATE_CMD_LINE:
			if (System_Alarm_Ready(&sim800lTimer))
			{
				SIM800L_Transmit_Byte(0x1A); //send CTRL+Z to terminate command line
				currentState = STATE_SEND_INIT_AT_CMD;
				doneSendingSMS = true; 
			}
			break;
	}
	
	return doneSendingSMS;
}
	

