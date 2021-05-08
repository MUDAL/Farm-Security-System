#ifndef _HC06_H
#define _HC06_H

extern void HC06_Tx_Init(void);
extern void HC06_Rx_Init(void);
extern void HC06_Transmit(char* pData);
extern char HC06_Receive_Char_Blocking(void);
extern char HC06_Receive_Char_NonBlocking(void);

#endif //_HC06_H
