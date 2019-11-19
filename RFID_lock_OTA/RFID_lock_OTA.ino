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
#include "led.h"
#include "ota.h"
#include "RFID.h"
#include "spi.h"


static String statusMessage = "";         /* This is set and requested from other modules. */


void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

void setup(void) {
  //  ESP.eraseConfig(); 
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  digitalWrite(LED_STATUS_PIN, HIGH);

  /* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
  Serial.begin(115200);   
  ESP.wdtFeed();

  HTTP_init();
  WS_init();  
  UDPPING_init();   
  WIFIC_init();  
}


void loop(void) {
  ESP.wdtFeed();
  /* Over The Air update takes up considerable processor time and causes the light to blink, so it should not be executed at the same time. */
  if(OTA_updateInProgress()){
    OTA_process();
  }else{  
    HTTP_process();  
    WS_process();
    UDPPING_process();  
    WIFIC_process(); 
    
    
  }
}
