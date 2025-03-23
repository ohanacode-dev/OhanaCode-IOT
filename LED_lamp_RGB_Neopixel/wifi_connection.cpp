/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  WiFi connection module. At startup connects to a user configured access point. If none is configured, creates its own access point to provide a way to adjust settings.
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "config.h"


static char myApName[32] = {0};    /* Array to form AP name based on read MAC */
static IPAddress stationIP;
static IPAddress apIP(192, 168, 1, 1);


char* WIFIC_getDeviceName(void){
  return myApName;
}

void WIFIC_init(void){ 
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  String ApName = AP_NAME_PREFIX + mac;
  ApName.toCharArray(myApName, ApName.length() + 1);
  
  WiFi.mode(WIFI_AP);  
  WiFi.begin();
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(myApName, AP_PASS);
}
