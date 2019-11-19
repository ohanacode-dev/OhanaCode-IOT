#include "http.h"
#include "udp.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include <EEPROM.h>

#ifdef USE_MAX6675
#include "max6675.h"
#else
#define USE_DS18B20
#include "DS18B20.h"
#endif

#ifdef USE_MAX6675
MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
unsigned long readTimeout = 0;
#endif
float currentTemp;                /* Current temperature. Can be used for controll. */
float currentTargetTemp = -2000;
float savedTargetTemp;

#ifdef DISPLAY_INTEGER_TEMPERATURE
String curTempIntStr;             /* Current temperature in integer format. used in forming HTML and id */
#endif
String curTempFloatStr;           /* Current temperature in one decimal format. used in forming HTML and id */

static String statusMessage = ""; 

void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

String MAIN_getCurrentTempFloatString(void){
  return curTempFloatStr;
}

String MAIN_getCurrentTargetTemp(void){
  return String(currentTargetTemp);
}

#ifdef DISPLAY_INTEGER_TEMPERATURE
String MAIN_getCurrentTempIntString(void){
  return curTempIntStr;
}
#endif

/* Reads saved settings from EEPROM */
static void readSavedSettings(){  
  EEPROM.begin(EEPROM_SIZE);
 
  EEPROM.get(TARGET_TEMP_ADDR, savedTargetTemp);  
  if(isnan(savedTargetTemp)){
    savedTargetTemp = 2000;
  }

  Serial.print("\nReading saved temp:");
  Serial.println(savedTargetTemp);
    
  EEPROM.end();
}

void MAIN_setTargetTemp(float targetTemp){
#ifdef USE_MAX6675
  if(targetTemp < 0){
    /* Deactivate */
    currentTargetTemp = -2000;  
  }else if(targetTemp > 1000){
#else
  if(targetTemp < -50){
    /* Deactivate */
    currentTargetTemp = -2000;  
  }else if(targetTemp > 84){
#endif 
    currentTargetTemp = savedTargetTemp;
  }else{
    /* Save target */
    savedTargetTemp = targetTemp;
    currentTargetTemp = targetTemp;

    Serial.print("Setting target temp:");
    Serial.println(targetTemp);
  
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(TARGET_TEMP_ADDR, targetTemp);
    EEPROM.commit();
    EEPROM.end();
  } 
}

void updateCurrentTemperature(void){
  /* Measure temperature */
  String oldTempStr = curTempFloatStr;
  
  #ifdef USE_MAX6675
  if((millis() - readTimeout) > 2000){
    /* Perform a read every 2 seconds */
    readTimeout = millis();
    currentTemp = thermocouple.readCelsius(); 
  }
  #else
  currentTemp = ((float)DS_readTemp(SENSPIN))/16; 
  #endif

  if(currentTemp < currentTargetTemp){
    /* Activate */ 
    digitalWrite(RELAY_PIN, HIGH); 
  }else{
    /* Deactivate */ 
    digitalWrite(RELAY_PIN, LOW); 
  }

#ifdef DISPLAY_INTEGER_TEMPERATURE
  curTempIntStr = String(round(currentTemp));
#endif
  curTempFloatStr = String((float)(round(currentTemp * 10))/10);
  curTempFloatStr.remove(curTempFloatStr.length()-1);

  /* Update connected clients */
  if(oldTempStr != curTempFloatStr){
    /* Even if we are displaying integer values, we still send float values 
    in case a client needs more precise controll. */
    String msg = "{\"CURRENT\":" + curTempFloatStr + "}";
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
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY); /* Use only tx, so rx can be used as GPIO */   
  //ESP.eraseConfig();
  initApp();
  WIFIC_init(); 
  ESP.wdtFeed();
  HTTP_init();
  ESP.wdtFeed();
  WS_init();  
  ESP.wdtFeed();
  UDP_init(); 
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
  updateCurrentTemperature(); 
}
