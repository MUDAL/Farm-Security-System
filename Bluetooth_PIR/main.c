#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sysTimer_struct.h"
#include "system.h"
#include "hc06.h"
#include "PIR.h"
#include "speaker.h"

/*
Apparently, the module isn't an HC06 but an
HC05. Press the module's onboard button for 
a short time before using it. Also press the 
button on subsequent power cycles.

Device name: HC05
Pairing code: 1234

The sample application demonstrates data transmission
and reception by the bluetooth module used.

Description of the sample application:
-If 'Baby' is sent to the bluetooth module by the 
bluetooth terminal, the module sends 'Maybe I'm in love\n' to the
bluetooth terminal

-If 'TEL class' is sent to the bluetooth module by the bluetooth
terminal, the module sends 'It's not worth it\n' to the bluetooth
terminal.

-If neither 'Baby' nor 'TEL class' is received by the bluetooth
module, the module sends 'Do not break the bro code by simping\n'
to the bluetooth terminal

*/

/*
The PIR code is commented. Uncomment it when you're ready to test the PIR sensor but
you'd have to complete the PIR_Init() function.
The PIR_Init() function needs to be configured. Check the
'External Interrupts' folder on my Github on how to do it.
*/

int main(void)
{
	
	static char hc06ReceiveBuffer[HC06_RX_BUFFER_SIZE];
	
	System_Init();
	PIR_Init();
	HC06_Tx_Init();
	HC06_Rx_Init(hc06ReceiveBuffer);
	Speaker_Init(SPEAKER_FREQ_6KHZ, SPEAKER_DUTY_CYCLE_65PERCENT);
	
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
