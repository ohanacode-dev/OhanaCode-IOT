/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  GPIO management module
 */
#include "http.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "mqtt.h"

static int currentPinVal = 0; 
static unsigned long PinWriteTimestamp = 0;


int PINCTRL_getCurrentVal( void ){
  return currentPinVal;
}

void PINCTRL_write(int val)
{
   if((millis() - PinWriteTimestamp) < DEBOUNCE_TIMEOUT){
    return;
  }

  if(val == 0){
    currentPinVal = 0;
  }else if(val > 0){
    currentPinVal = 100;    
  }else{
    if(currentPinVal == 0){
      currentPinVal = 100;
    }else{
      currentPinVal = 0;
    }
  }

  Serial.print("SW:");
  Serial.println(currentPinVal);

  if(currentPinVal == 0){
    #ifdef INVERT_OUTPUT
      /* Wemos D1 MNI onboard LED is reversed polarized. */
      digitalWrite(SWITCHPIN, HIGH);
    #else
      digitalWrite(SWITCHPIN, LOW);
    #endif 
  }else{
    #ifdef INVERT_OUTPUT
    /* Wemos D1 MNI onboard LED is reversed polarized. */
      digitalWrite(SWITCHPIN, LOW);
    #else
      digitalWrite(SWITCHPIN, HIGH);
    #endif 
  }
    
  PinWriteTimestamp = millis();

  MQTT_setCurrentStatus();
  
  String statusMsg = "{\"CURRENT\":[" + String(currentPinVal) + "]}";  
  WS_broadcast(statusMsg);
}
