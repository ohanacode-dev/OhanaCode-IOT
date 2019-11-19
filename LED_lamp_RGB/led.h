#ifndef LED_H
#define LED_H

/* Values for separate channels expressed in range 0-1024 */
typedef struct{
  uint8_t R;
  uint8_t G;
  uint8_t B;
}rgbValue_t;

extern void LED_writeRGB(rgbValue_t val);
extern rgbValue_t LED_getCurrentRGB( void );
extern void LED_write(int val);
extern int LED_getCurrentVal( void );
extern void LED_init(void);
extern void LED_saveColor(void);

#endif
