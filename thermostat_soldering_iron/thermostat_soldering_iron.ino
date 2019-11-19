#include "http.h"
#include "udp.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include <EEPROM.h>
#include "otaa.h"
#include "max6675.h"

MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
unsigned long readTimeout = 0;
float currentTemp;                /* Current temperature. Can be used for controll. */
float currentTargetTemp = -2000;
float savedTargetTemp;
String curTempIntStr;             /* Current temperature in integer format. used in forming HTML and id */
static String statusMessage = ""; 

void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

String MAIN_getCurrentTargetTemp(void){
  return String(currentTargetTemp);
}

String MAIN_getCurrentTempIntString(void){
  return curTempIntStr;
}

/* Reads saved settings from EEPROM */
static void readSavedSettings(){  
  EEPROM.begin(EEPROM_SIZE);
 
  EEPROM.get(TARGET_TEMP_ADDR, savedTargetTemp);  
  if(isnan(savedTargetTemp)){
    savedTargetTemp = 2000;
  }else{
    currentTargetTemp = savedTargetTemp;
  }

  SERIAL_PRINT("\nReading saved temp:");
  SERIAL_PRINT(savedTargetTemp);

  EEPROM.end();
}

void MAIN_setTargetTemp(float targetTemp){

  if(targetTemp < 0){
    /* Deactivate */
    currentTargetTemp = -2000;  
  }else if(targetTemp > 1000){
    currentTargetTemp = savedTargetTemp;
  }else if((targetTemp >= MIN_TEMP) && (targetTemp <= MAX_TEMP)){
    /* Save target */
    savedTargetTemp = targetTemp;
    currentTargetTemp = targetTemp;

    SERIAL_PRINT("Setting target temp:");
    SERIAL_PRINTLN(targetTemp);
 
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(TARGET_TEMP_ADDR, targetTemp);
    EEPROM.commit();
    EEPROM.end();
  } 
}

void updateCurrentTemperature(void){
  /* Measure temperature */ 
   
  if((millis() - readTimeout) > 1000){
    /* Perform a read every 2 seconds */
    readTimeout = millis();
    currentTemp = thermocouple.readCelsius(); 
  }  
  
  String oldTempStr = MAIN_getCurrentTempIntString();

  if(currentTemp < currentTargetTemp){
    /* Activate */ 
    digitalWrite(RELAY_PIN, HIGH); 
  }else{
    /* Deactivate */ 
    digitalWrite(RELAY_PIN, LOW); 
  }

  curTempIntStr = String(round(currentTemp));

  /* Update connected clients */
  if(oldTempStr != curTempIntStr){   
    String msg = "{\"CURRENT\":" + curTempIntStr + "}";
    WS_ServerBroadcast(msg);
  }
}

static void initApp(void){
  digitalWrite(RELAY_PIN, LOW); 
  pinMode(RELAY_PIN, OUTPUT);
  
  readSavedSettings();  
}

void setup(void) {
	/* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
#ifdef USE_D1_MINI
  Serial.begin(115200); 
#else
  #warning "Can not use serial port for debugging because both of UART pins are used for MAX6675 interface"
#endif
  //ESP.eraseConfig();
  initApp();
  WIFIC_init(); 
  HTTP_init();
  ESP.wdtFeed();
  WS_init();  
  UDP_init(); 
  OTAA_init();
}


void loop(void) {
  if(!OTAA_updateInProgress()){
    ESP.wdtFeed();
    HTTP_process();
    WS_process();
    ESP.wdtFeed();
    UDP_process();
    WIFIC_process(); 
    ESP.wdtFeed();    
    updateCurrentTemperature(); 
  }

  OTAA_process();
}
