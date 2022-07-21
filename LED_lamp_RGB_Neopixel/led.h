#ifndef LED_H
#define LED_H

extern void LED_getCurrentRGBA( uint8_t* val );
extern void LED_writeRGBA(uint8_t* val);
extern void LED_init(void);
extern void LED_saveColor(void);

#endif
