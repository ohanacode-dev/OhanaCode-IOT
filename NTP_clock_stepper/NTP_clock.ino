#include "http.h"
#include "udp.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "ntp.h"
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
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY); /* Use only tx, so rx can be used as GPIO */   
  //ESP.eraseConfig();
  STMDRV_init();
  
  WIFIC_init(); 
  ESP.wdtFeed();
  HTTP_init();
  ESP.wdtFeed();
  WS_init();  
  ESP.wdtFeed();
  UDP_init(); 
  ESP.wdtFeed();
  NTP_init(); 
}


void loop(void) {
  ESP.wdtFeed();
  HTTP_process();
  ESP.wdtFeed();
  WS_process();
  ESP.wdtFeed();
  UDP_process();
  ESP.wdtFeed();
  WIFIC_process(); 
  ESP.wdtFeed();
  NTP_process();
  STMDRV_process();
}
