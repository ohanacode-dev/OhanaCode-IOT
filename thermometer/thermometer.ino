#include "http.h"
#include "udp_ping.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "ota.cpp"

#ifdef USE_DS18B20
#include "DS18B20.h"
#endif


#ifdef USE_MAX6675
#include "max6675.h"

MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
unsigned long readTimeout = 0;
#endif


float currentTemp;                /* Current temperature. Can be used for controll. */
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

#ifdef DISPLAY_INTEGER_TEMPERATURE
String MAIN_getCurrentTempIntString(void){
  return curTempIntStr;
}
#endif

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

void setup(void) {
	/* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY); /* Use only tx, so rx can be used as GPIO */   
  //ESP.eraseConfig();
  
  WIFIC_init(); 
  HTTP_init();
  WS_init();  
  UDPPING_init(); 
}


void loop(void) {
  ESP.wdtFeed();
  if(OTA_updateInProgress()){
    OTA_process();
  }else{
    HTTP_process();
    WS_process(); 
    UDPPING_process();
    WIFIC_process(); 
    updateCurrentTemperature(); 
  }
}
