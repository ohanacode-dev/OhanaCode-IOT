#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "web_socket.h"
#include "config.h"
#include "ntp.h"
#include <TimeLib.h>
#include "stepper_motor_driver.h"


static unsigned long localTime;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER_NAME, 0, NTP_SYNC_INTERVAL);

void NTP_init()
{
  timeClient.begin();
}

void NTP_process()
{
  timeClient.update();
  if(localTime != timeClient.getEpochTime()){    

    localTime = timeClient.getEpochTime();

    setTime(localTime);

    String timeStatus = "{ \"CURRENT\":\"" + timeClient.getFormattedTime() + "\"}";  
    WS_ServerBroadcast(timeStatus); 
    
    uint8_t m = minute();
    Serial.write(&m, 1); 

    STMDRV_tick(second());
  } 
}

String NTP_getTimeString(){
  return timeClient.getFormattedTime();
}

unsigned long NTP_getLocalTime(){
  return localTime;
}
