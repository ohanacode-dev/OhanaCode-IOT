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

static String statusMessage = "";         /* This is set and requested from other modules. */
static unsigned long executeTime = 0;     /* On touch LED level change timestamp */
static int lastLedVal = 0;                /* History to be used with touch sensor function (Value to restore to) */
static uint32_t lightTimeout = 1000 * 60 * 1;
static bool userManualControllFlag = true;

static void checkPirSignal(){ 
  if(userManualControllFlag){
    return;
  }
  
  if(digitalRead(SENSPIN) == HIGH){
    executeTime = millis(); 
    
    if(lastLedVal > 0){
      LED_write(lastLedVal);
    }else{
      LED_write(LED_LEVEL_MAX);
    }     
  }else if((millis() - executeTime) > lightTimeout){
    // Timed out. Turn light off. Note the intensity.
    int currentLedVal = LED_getCurrentVal();
    
    if(currentLedVal > 0){
      lastLedVal = currentLedVal;
    }
  }  
}

void MAIN_setManualControll(bool manual){
  userManualControllFlag = manual;
}

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
  pinMode(SENSPIN, INPUT);
  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(LED_G_PIN, LOW);
  digitalWrite(LED_B_PIN, LOW);  
  /* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
  Serial.begin(115200);   
  ESP.wdtFeed();
  LED_init();
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
    ESP.wdtFeed();
    checkPirSignal();  
  }
}
