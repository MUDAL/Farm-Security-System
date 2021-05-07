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
#define PHONE_NUMBER						"+2348144086708"
#define SMS_NOT_SENT						false
#define SMS_SENT								true
#define TIME_50_MS							50
#define TIME_20_SEC							20000
#define RPI_NO_DETECTION				'0'
#define RPI_PERSON_DETECTED			'1'
#define RPI_ANIMAL_DETECTED			'2'

//private global variables
static sysTimer_t sim800lTimer;

//private functions
static bool SendSMS(char* phoneNumber, char* message);

enum States
{
	STATE_DETECT_MOTION = 0,
	STATE_CHECK_RPI_DATA,
	STATE_SPEAKER_CONTROL
};

int main(void)
{
	//local variables
	static sysTimer_t speakerTimer;
	uint8_t state = STATE_DETECT_MOTION;
	char hc06RxChar = RPI_NO_DETECTION;
	char raspberryPiData = RPI_NO_DETECTION;
	bool smsStatus = SMS_NOT_SENT;
	
	//Initializations
	System_Init();
	System_Alarm_Init(&sim800lTimer, TIME_50_MS);
	System_Alarm_Init(&speakerTimer, TIME_20_SEC);
	SIM800L_Tx_Init();
	Speaker_Init();
	PIR_Init();
	HC06_Tx_Init();
	HC06_Rx_Init();
	
	while(1)
	{		
		switch(state)
		{
			case STATE_DETECT_MOTION:
				
				if (PIR_Motion_Detected())
				{
					HC06_Transmit("T"); //Trigger raspberry pi with bluetooth message
					PIR_Restart(); //Reset PIR sensor state
					state = STATE_CHECK_RPI_DATA;
				}
				
				break;
				
			case STATE_CHECK_RPI_DATA:
				
				hc06RxChar = HC06_Receive_Char();
		
				if (hc06RxChar != '\0')
				{
					raspberryPiData = hc06RxChar;
				}
				
				switch(raspberryPiData)
				{
					case RPI_PERSON_DETECTED:
					smsStatus = SendSMS(PHONE_NUMBER,"Person detected");
					if (smsStatus == SMS_SENT)
					{
						Speaker_Activate(SPEAKER_FREQ_1KHZ,SPEAKER_DUTY_CYCLE_65PERCENT);
						state = STATE_SPEAKER_CONTROL;
					}
					break;
				
					case RPI_ANIMAL_DETECTED:
						smsStatus = SendSMS(PHONE_NUMBER,"Animal detected");
						if (smsStatus == SMS_SENT)
						{
							Speaker_Activate(SPEAKER_FREQ_15KHZ, SPEAKER_DUTY_CYCLE_65PERCENT);
							state = STATE_SPEAKER_CONTROL;
						}
						break;
				}
					
				break;
			
			case STATE_SPEAKER_CONTROL:
				if (System_Alarm_Ready(&speakerTimer))
				{
					Speaker_Deactivate();
					state = STATE_DETECT_MOTION;
					raspberryPiData = RPI_NO_DETECTION;
				}
				break;
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
				memset(smsATCmd, '\0', 27);
				strcpy(smsATCmd, "AT+CMGS=\"");
			}
			break;
	}
	return doneSendingSMS;
}
