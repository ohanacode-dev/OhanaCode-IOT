#ifndef TERMOSTAT_H
#define TERMOSTAT_H

#include "config.h"

extern void MAIN_setStatusMsg(String msg);
extern String MAIN_getStatusMsg(void);
extern String MAIN_getCurrentTargetTemp(void);
extern void MAIN_setTargetTemp(float targetTemp);
extern String MAIN_getCurrentTempIntString(void);

#endif
