/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  MQTT client module to anounce itself to iot-portal and accept MQTT commands.
 */
#include <PubSubClient.h>
#include <mDNSResolver.h>
#include <ESP8266WiFi.h>
#include "mqtt.h"
#include "wifi_connection.h"
#include "config.h"
#include "DS18B20.h"


#define MQTT_SERVER_HOSTNAME  "iot-portal.local"
#define STATUS_TOPIC          "iot"
#define PORT                  1883
#define CONNECT_TIMEOUT       (2000ul) 

WiFiClient espClient;
PubSubClient mqttclient(espClient);
static char msgBuffer[256] = {0};
static uint32_t connectAttemptTime = 0;
static IPAddress mqttServerIp = IPADDR_NONE;

WiFiUDP wifiudp;
mDNSResolver::Resolver resolver(wifiudp);

bool MQTT_connect() {
  if(((millis() - connectAttemptTime) > CONNECT_TIMEOUT) || (connectAttemptTime == 0)){
    connectAttemptTime = millis();
    
    if(mqttServerIp == IPADDR_NONE){
      mqttServerIp = resolver.search(MQTT_SERVER_HOSTNAME);
  
      if(mqttServerIp == IPADDR_NONE){
        Serial.println("ERROR: MQTT server IP could not be resolved."); 
        return false;
      }
    }      
      
    mqttclient.setServer(mqttServerIp, PORT);
    // Attempt to connect
    if (mqttclient.connect(DEVICE_NAME)) {
      Serial.println("connected");

      float tempC = DS_getTemp();      
      String device_status = "{\"name\":\"" + String(DEVICE_NAME) + "\", \"msg\":\"" + String((int)tempC) + "°C\"}";
      device_status.toCharArray(msgBuffer, sizeof(msgBuffer));
       
      mqttclient.publish(STATUS_TOPIC, msgBuffer);
      mqttclient.loop(); 
      return true;      
    } else {
      Serial.print("MQTT failed. Server IP:");   
      Serial.println(mqttServerIp);
      Serial.print("ERROR:");
      Serial.println(mqttclient.state());
    }    
  }
  
  mqttclient.loop(); 
  return false;
}
