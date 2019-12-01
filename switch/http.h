#ifndef HTTP_H
#define HTTP_H

#include <Arduino.h>

extern void HTTP_process(void);
extern void HTTP_init(void);
extern String HTTP_getStatus(void);
extern String HTTP_getFeatures(void);

#endif
