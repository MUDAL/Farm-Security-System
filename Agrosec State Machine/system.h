#ifndef _SYSTEM_H
#define _SYSTEM_H

extern void System_Init(void);
extern void System_Timer_DelayMs(uint32_t delayTime);
extern void System_Alarm_Init(sysTimer_t* pSysTimer, uint32_t timerRepTime);
extern bool System_Alarm_Ready(sysTimer_t* pSysTimer);

#endif //_SYSTEM_H
