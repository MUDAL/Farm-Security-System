#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sysTimer_struct.h"
#include "system.h"
#include "hc06.h"
#include "PIR.h"
#include "sim800l.h"
#include "speaker.h"

//private defines
#define PHONE_NUMBER				"+2348144086708"
#define SMS_NOT_SENT				false
#define SMS_SENT						true
#define TIME_50_MS					50
#define TIME_20_SEC					20000

//private global variables
static sysTimer_t sim800lTimer;

//private functions
static bool SendSMS(char* phoneNumber, char* message);

int main(void)
{
	//local variables
	static char raspberryPiData;
	static sysTimer_t audibleSpeakerTimer;
	bool smsStatus = SMS_NOT_SENT;
	bool audibleSpeakerActivated = false;

	//Initializations
	System_Init();
	System_Alarm_Init(&sim800lTimer, TIME_50_MS);
	System_Alarm_Init(&audibleSpeakerTimer, TIME_20_SEC);
	SIM800L_Tx_Init();
	Speaker_Init();
	PIR_Init();
	HC06_Tx_Init();
	HC06_Rx_Init(&raspberryPiData);
	
	while(1)
	{
		
		if (PIR_Motion_Detected())
		{
			//Reset PIR sensor state
			PIR_Restart();
			//Trigger raspberry pi with bluetooth message 
			HC06_Transmit("trigger"); 
		}
		
		if (HC06_Rx_Done_Receiving())
		{
			//Checking for data from raspberry pi via bluetooth
			switch (raspberryPiData)
			{
				case '1':
					smsStatus = SendSMS(PHONE_NUMBER,"Person detected");
					if (smsStatus == SMS_SENT)
					{
						Speaker_Activate(SPEAKER_FREQ_800HZ,SPEAKER_DUTY_CYCLE_65PERCENT);
						audibleSpeakerActivated = true;
						HC06_Rx_Restart(); //Reset HC06 module state
					}
					break;
				
				case '2':
					smsStatus = SendSMS(PHONE_NUMBER,"Animal detected");
					if (smsStatus == SMS_SENT)
					{
						HC06_Rx_Restart();
					}
					break;
			}
		}
		
		if (audibleSpeakerActivated)
		{
			if (System_Alarm_Ready(&audibleSpeakerTimer))
			{
				Speaker_Deactivate();
				audibleSpeakerActivated = false;
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
	

