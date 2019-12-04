/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  LED control module.
 */
#include "http.h"
#include "udp_ping.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"

static int currentLedVal = 0;            /* Current LED level. used in forming HTML and id */
static unsigned long LedWriteTimestamp = 0;


int LED_getCurrentVal( void ){
  return currentLedVal;
}

/* Set LED level in percentage */
void LED_write(int val)
{
  if((millis() - LedWriteTimestamp) < DEBOUNCE_TIMEOUT){
    return;
  }
  Serial.print("LED:");
  Serial.println(val);
  double level = val;
  
  if(level < 0){
      level = 0;
  }
  if(level > LED_LEVEL_MAX){
    level = LED_LEVEL_MAX;
  }
  currentLedVal = level;

#ifdef USE_LINEAR_SCALING 
  level = level * 10 + level / 4;
#else
  if(level < 30){
    level = level / 4;
  }else{
    level = pow(2, level / 10);
  }
#endif

  if(level > 1023){
    level = 1023;
  }    

#ifdef INVERT_OUTPUT
  /* Wemos D1 MNI onboard LED is reversed polarized. */
  analogWrite(LEDPIN, 1023 - level);
#else
  analogWrite(LEDPIN, level);
#endif 

  LedWriteTimestamp = millis();
}
