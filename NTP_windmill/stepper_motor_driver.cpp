#include <stdint.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "stepper_motor_driver.h"
#include "ntp.h"

#define TICK_TIMEOUT      (216)    /* Number of seconds to tick the hour hand motor  */
#define TICK_DURATION     (500)   /* Number of milliseconds to keep the tick active */

static uint16_t hourTickCount;
static unsigned long lastTickTime;
static unsigned long tickStartTime;
static bool tickActive = false;

static void M_H_tickStop(void){
  digitalWrite(M_H_PIN1, LOW);
  digitalWrite(M_H_PIN2, LOW);
  digitalWrite(M_H_PIN3, LOW);
  digitalWrite(M_H_PIN4, LOW);
}

static void M_H_tick(void){
  switch(hourTickCount & 0b11){
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
 
  hourTickCount++; 
}

void STMDRV_process(void)
{  
  if(tickActive){   
    if((millis() - tickStartTime) > TICK_DURATION){
      M_H_tickStop();
      tickActive = false;
    }
  }else{    
    unsigned long currentLocalTime = NTP_getLocalTime();

    if((currentLocalTime - lastTickTime) >= TICK_TIMEOUT){
      if(lastTickTime == 0){
        /* Set initial tick time as increment of TICK_TIMEOUT */
        lastTickTime = currentLocalTime - (currentLocalTime % TICK_TIMEOUT);
      }else{
        lastTickTime += TICK_TIMEOUT;
      }
  
      tickActive = true;
      tickStartTime = millis();
  
      M_H_tick();
    }  
  }
}

void STMDRV_init(void){
  pinMode(M_H_PIN1, OUTPUT);
  pinMode(M_H_PIN2, OUTPUT);
  pinMode(M_H_PIN3, OUTPUT);
  pinMode(M_H_PIN4, OUTPUT);

  M_H_tickStop();
  hourTickCount = 0;
  lastTickTime = 0;
  tickStartTime = 0;
}
