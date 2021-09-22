/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  This is the main sketch file. 
 *  It provides a touch sensor functionality and a periodic pooling of other services.
 */

#include "http.h"
#include "udp_ping.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "pinctrl.h"
#include "ota.h"
#include "pushbutton.h"
#include "mqtt.h"

static String statusMessage = "";         /* This is set and requested from other modules. */

void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

void setup(void) {
  /* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY); /* Use only tx, so rx can be used as GPIO */   
  //ESP.eraseConfig();
  pinMode(SWITCHPIN, OUTPUT);
  PINCTRL_write(0);  
  WIFIC_init();
  HTTP_init();
  WS_init();  
  UDPPING_init(); 
  MQTT_init();
}


void loop(void) {  
  if(OTA_updateInProgress()){
    OTA_process();
  }else{
    HTTP_process();  
    WS_process();
    UDPPING_process();  
    WIFIC_process();     
    PUSHBUTTON_process(); 
    MQTT_process();
  }
}
