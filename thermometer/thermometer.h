#ifndef LED_LAMP_H
#define LED_LAMP_H

#include "config.h"

extern String MAIN_getCurrentTempFloatString(void);
extern void MAIN_setStatusMsg(String msg);
extern String MAIN_getStatusMsg(void);

#ifdef DISPLAY_INTEGER_TEMPERATURE
extern String MAIN_getCurrentTempIntString(void);
#endif

#endif
