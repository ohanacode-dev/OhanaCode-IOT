#ifndef LED_LAMP_H
#define LED_LAMP_H

#include "config.h"

extern String MAIN_getCurrentTempFloatString(void);
extern void MAIN_setStatusMsg(String msg);
extern String MAIN_getStatusMsg(void);
extern String MAIN_getCurrentTargetTemp(void);
extern void MAIN_setTargetTemp(float targetTemp);

#ifdef DISPLAY_INTEGER_TEMPERATURE
extern String MAIN_getCurrentTempIntString(void);
#endif

#endif
