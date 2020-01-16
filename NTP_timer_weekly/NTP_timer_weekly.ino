#include "http.h"
#include "udp_ping.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "ntp.h"
#include "schedule.h"


static String statusMessage = ""; 
static volatile int currentState = 0;

int MAIN_getState(){
  return currentState;
}

void MAIN_setState(int newState){
  if(newState > 0){
     currentState = 1;
  }else{
    currentState = 0;
  } 
}

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
  
  WIFIC_init();   
  HTTP_init();
  WS_init();  
  UDPPING_init(); 
  NTP_init(); 
  SCH_init();
  ESP.wdtFeed();
}


void loop(void) {
  HTTP_process();
  WS_process();
  UDPPING_process();
  WIFIC_process();   
  NTP_process();  
  SCH_process();  
  ESP.wdtFeed();
}
