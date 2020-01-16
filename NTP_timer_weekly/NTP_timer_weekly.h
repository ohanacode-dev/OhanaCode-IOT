#ifndef NTP_TIMER_W_H
#define NTP_TIMER_W_H

extern void MAIN_setStatusMsg(String msg);
extern String MAIN_getStatusMsg(void);
extern int MAIN_getState();
extern void MAIN_setState(int newState);

#endif
