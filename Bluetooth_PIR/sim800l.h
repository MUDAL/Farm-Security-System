#ifndef _SIM800L_H
#define _SIM800L_H

enum SIM800L_State
{
	STATE_SEND_PHONE_NO = 0,
	STATE_SEND_MESSAGE,
	STATE_SEND_CTRL_Z
};

extern void SIM800L_Tx_Init(void);
extern void SIM800L_Transmit_Byte(char data);
extern void SIM800L_Transmit_String(char* pData);

#endif //_SIM800L_H
