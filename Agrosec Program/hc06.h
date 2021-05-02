#ifndef _HC06_H
#define _HC06_H

#define HC06_RX_BUFFER_SIZE				100
#define HC06_USE_INTERRUPTS				0

extern void HC06_Tx_Init(void);
extern void HC06_Rx_Init(char* hc06RxBuffer);
extern void HC06_Transmit(char* pData);
extern bool HC06_Rx_Done_Receiving(void);
extern void HC06_Rx_Restart(void);

#endif //_HC06_H
