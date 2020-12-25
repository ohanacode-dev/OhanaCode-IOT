#include "http.h"
#include "udp_ping.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "ntp.h"
#include "ota.h"
#include "stepper_motor_driver.h"

static String statusMessage = ""; 

void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

void setup(void) {
	/* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
  Serial.begin(300,SERIAL_8N1,SERIAL_TX_ONLY); 
  pinMode(LED_PIN, OUTPUT);
  //ESP.eraseConfig();
  STMDRV_init();
  WIFIC_init();   
  HTTP_init();
  WS_init();  
  UDPPING_init(); 
  NTP_init(); 
  ESP.wdtFeed();
}


void loop(void) {
  if(OTA_updateInProgress()){
    OTA_process();
  }else{
    HTTP_process();
    WS_process();
    UDPPING_process();
    WIFIC_process(); 
    NTP_process();
    STMDRV_process();
    ESP.wdtFeed();    
  }
}
