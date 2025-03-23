#include "http.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "led.h"
#include "ota.h"

static String statusMessage = "";         /* This is set and requested from other modules. */
static int lastLedVal = 0;                /* History to be used with touch sensor function (Value to restore to) */

void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

void setup(void) {
  //  ESP.eraseConfig(); 
  delay(100); /* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
          
  LED_init();
  WIFIC_init();    
  WS_init();  
  HTTP_init();
}


void loop(void) {     
  /* Over The Air update takes up considerable processor time and causes the light to blink, so it should not be executed at the same time. */
  if(OTA_updateInProgress()){
    OTA_process();
  }else{
    HTTP_process();  
    WS_process();
  }
}
