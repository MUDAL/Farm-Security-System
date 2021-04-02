#ifndef _HC06_H
#define _HC06_H

#define HC06_RX_BUFFER_SIZE				100
#define HC06_USE_INTERRUPTS				0

//Initializes the HC06 module
extern void HC06_Tx_Init(void);
extern void HC06_Rx_Init(char* hc06RxBuffer);
//Transmits data wirelessly via the HC06 module
extern void HC06_Transmit(char* pData);
/*Returns 'true' if HC06 module has received data and the
data is ready to be processed.
*/
extern bool HC06_Rx_Done_Receiving(void);

extern void HC06_Rx_Restart(void);

#endif //_HC06_H
