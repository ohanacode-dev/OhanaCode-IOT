/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  MQTT client module to anounce itself to iot-portal and accept MQTT commands.
 */
#include <PubSubClient.h>
#include <mDNSResolver.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "pinctrl.h"
#include "mqtt.h"
#include "wifi_connection.h"


#define MQTT_SERVER_HOSTNAME  "iot-portal.local"
#define STATUS_TOPIC          "device_status"
#define PORT                  1883
#define CONNECT_TIMEOUT       (10000ul) 

WiFiClient espClient;
PubSubClient mqttclient(espClient);
static String macAddr = "";
static char msgBuffer[256] = {0};
static uint32_t connectAttemptTime = 0;
static char clientName[32] = {0};
static IPAddress mqttServerIp;

WiFiUDP wifiudp;
mDNSResolver::Resolver resolver(wifiudp);
  
static void resolve_mDNS(){
  mqttServerIp = resolver.search(MQTT_SERVER_HOSTNAME);  
}

void MQTT_setCurrentStatus(){
  IPAddress deviceIp = WIFIC_getStIP();
  String device_status = "{\"mac\":\"" + macAddr + "\", \"type\":\"switch\", \"addr\":\"" + deviceIp.toString() + "\", \"CURRENT\":[" + String(PINCTRL_getCurrentVal()) + "]}";
  device_status.toCharArray(msgBuffer, sizeof(msgBuffer));
}


static void callback(char* topic, byte* payload, unsigned int length) {  

  char textMsg[length + 1] = {0};
  for(int i = 0; i < length; i++){
    textMsg[i] = (char)payload[i];        
  }
  
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, textMsg);    

  // Test if parsing succeeds.
  if (!error) {
    JsonObject root = doc.as<JsonObject>();
    
    if(root.containsKey("CURRENT")){
      JsonArray array = root["CURRENT"].as<JsonArray>();
      for(JsonVariant v : array) {
          int value = v.as<int>();
          PINCTRL_write(value); 
      }
    }
  }
}


static void mqtt_connect() {
  if(mqttServerIp == IPADDR_NONE){
    resolve_mDNS();
  }

  if(mqttServerIp == IPADDR_NONE){
    Serial.println("ERROR: MQTT server IP could not be resolved.");   
    return;
  }
    
  mqttclient.setServer(mqttServerIp, PORT);
  mqttclient.setCallback(callback);
  // Attempt to connect
  if (mqttclient.connect(clientName)) {
    Serial.println("connected");
    mqttclient.subscribe(clientName);
    MQTT_setCurrentStatus();    
    mqttclient.publish(STATUS_TOPIC, msgBuffer);

  } else {
    Serial.print("MQTT failed. Server IP:");   
    Serial.println(mqttServerIp);
    Serial.print("ERROR:");
    Serial.println(mqttclient.state());
  }
  
}

void MQTT_init(){
  resolve_mDNS();  
  Serial.print("MQTT Server IP:");   
  Serial.println(mqttServerIp);

  macAddr = WiFi.macAddress();
  macAddr.replace(":", "");
  macAddr.toCharArray(clientName, sizeof(clientName));  
  Serial.println("MQTT client name:" + macAddr);
  mqtt_connect();
}

void MQTT_process(){
  if (!mqttclient.connected() && ((millis() - connectAttemptTime) > CONNECT_TIMEOUT)) {     
    // Not connected. Try again.
    mqtt_connect();
    connectAttemptTime = millis();
  }

  if(mqttclient.connected() && (msgBuffer[0] != 0)){
    mqttclient.publish(STATUS_TOPIC, msgBuffer);
    msgBuffer[0] = 0;
  }
  
  mqttclient.loop();  
}
