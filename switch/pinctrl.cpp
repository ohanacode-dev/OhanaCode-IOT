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

static int currentPinVal = 0;            /* Current LED level. used in forming HTML and id */
static unsigned long PinWriteTimestamp = 0;


int PINCTRL_getCurrentVal( void ){
  return currentPinVal;
}

/* Set LED level in percentage */
void PINCTRL_write(int val)
{
   if((millis() - PinWriteTimestamp) < DEBOUNCE_TIMEOUT){
    return;
  }
  Serial.print("LED:");
  Serial.println(val);
  double level = val;
  
  if(level <= 0){
    currentPinVal = 0;
    #ifdef USE_D1_MINI
      /* Wemos D1 MNI onboard LED is reversed polarized. */
      digitalWrite(SWITCHPIN, HIGH);
    #else
      digitalWrite(SWITCHPIN, LOW);
    #endif 
  }
  if(level > 0){
    currentPinVal = 100;
    #ifdef USE_D1_MINI
    /* Wemos D1 MNI onboard LED is reversed polarized. */
      digitalWrite(SWITCHPIN, LOW);
    #else
      digitalWrite(SWITCHPIN, HIGH);
    #endif 
  }

  PinWriteTimestamp = millis();
}
