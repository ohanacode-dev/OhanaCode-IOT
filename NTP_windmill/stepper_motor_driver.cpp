#include <stdint.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "stepper_motor_driver.h"

#define TICK_TIMEOUT      (216)    /* Number of seconds to tick the hour hand motor  */
#define TICK_DURATION     (500)   /* Number of milliseconds to keep the tick active */

static uint16_t tickCount;
static uint32_t secCount;
static uint32_t tickTimestamp;

static void M_H_tickStop(void){
  digitalWrite(M_H_PIN1, LOW);
  digitalWrite(M_H_PIN2, LOW);
  digitalWrite(M_H_PIN3, LOW);
  digitalWrite(M_H_PIN4, LOW);
}

static void M_H_tick(void){
  switch(tickCount & 0b11){
    case 0:
      digitalWrite(M_H_PIN1, HIGH);
      digitalWrite(M_H_PIN2, LOW);
      digitalWrite(M_H_PIN3, LOW);
      digitalWrite(M_H_PIN4, LOW);
      break;
     case 1:
      digitalWrite(M_H_PIN1, LOW);
      digitalWrite(M_H_PIN2, HIGH);
      digitalWrite(M_H_PIN3, LOW);
      digitalWrite(M_H_PIN4, LOW);
      break;
     case 2:
      digitalWrite(M_H_PIN1, LOW);
      digitalWrite(M_H_PIN2, LOW);
      digitalWrite(M_H_PIN3, HIGH);
      digitalWrite(M_H_PIN4, LOW);
      break;
     case 3:
      digitalWrite(M_H_PIN1, LOW);
      digitalWrite(M_H_PIN2, LOW);
      digitalWrite(M_H_PIN3, LOW);
      digitalWrite(M_H_PIN4, HIGH);
      break;
  }
 
  tickCount++; 
}


void STMDRV_tick(void)
{  
  secCount++;
  if((secCount - tickTimestamp) >= TICK_TIMEOUT){
    tickTimestamp += TICK_TIMEOUT;

    M_H_tick();
    delay(TICK_DURATION);
    M_H_tickStop();    
  }
}

void STMDRV_init(void){
  pinMode(M_H_PIN1, OUTPUT);
  pinMode(M_H_PIN2, OUTPUT);
  pinMode(M_H_PIN3, OUTPUT);
  pinMode(M_H_PIN4, OUTPUT);

  M_H_tickStop();
  tickCount = 0;
  secCount = 0;
  tickTimestamp = 0;
}
