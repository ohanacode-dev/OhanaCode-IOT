#include <ESP8266WiFiMulti.h>
#include "config.h"

ESP8266WiFiMulti wifiMulti;


void WIFIC_init()
{
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(SSID_1, PASS_1);
  wifiMulti.addAP(SSID_2, PASS_2);
  
  Serial.print( "\n\nConnecting" );
  while ( wifiMulti.run() != WL_CONNECTED ) {
    delay ( 500 );
    ESP.wdtFeed();
    Serial.print ( "." );
  }
  String IP =  WiFi.localIP().toString();
  String wifi_statusMessage = "\nConnected to: " + WiFi.SSID() + String(". IP address: ") + IP;   
  Serial.println(wifi_statusMessage); 
}
