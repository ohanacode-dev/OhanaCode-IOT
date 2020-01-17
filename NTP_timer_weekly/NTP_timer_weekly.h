#ifndef NTP_TIMER_W_H
#define NTP_TIMER_W_H

extern void MAIN_setStatusMsg(String msg);
extern String MAIN_getStatusMsg(void);
extern bool MAIN_getActivated();
extern void MAIN_setActivated(bool newState);

#endif
