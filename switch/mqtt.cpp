#include <PubSubClient.h>
#include <mDNSResolver.h>
#include <ESP8266WiFi.h>
#include "pinctrl.h"
#include "mqtt.h"


#define MQTT_SERVER_HOSTNAME  "iot-portal"
#define STATUS_TOPIC          "device_status"
#define PORT                  1883
#define CONNECT_TIMEOUT       (10000ul) 

WiFiClient ethClient;
PubSubClient client(ethClient);
String macAddr = WiFi.macAddress();
char msgBuffer[256] = {0};
uint32_t connectAttemptTime = 0;
char clientName[32] = {0};

static IPAddress resolve_mDNS(){
  WiFiUDP udp;
  mDNSResolver::Resolver resolver(udp);

  IPAddress answer = resolver.search(MQTT_SERVER_HOSTNAME);
  
  return answer;
}

static void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

static void setCurrentStatus(){
  String device_status = "{\"mac\":\"" + macAddr + "\", \"type\":\"switch\", \"CURRENT\":[" + String(PINCTRL_getCurrentVal()) + "]}";
  device_status.toCharArray(msgBuffer, sizeof(msgBuffer));
}

static void mqtt_connect() {
  
  if(!client.connected()){
    // Attempt to connect
    if (client.connect(clientName)) {
      Serial.println("connected");
      // Once connected, publish an announcement...

      setCurrentStatus();
      
      client.publish(STATUS_TOPIC, msgBuffer);

      // Subscribe to topic named after our MAC address
      
      
      client.subscribe(clientName);
    } else {
      Serial.println("MQTT failed");   
    }
  } 
}

void MQTT_publishStatus(){
  if(client.connected()){
    setCurrentStatus();
    client.publish(STATUS_TOPIC, msgBuffer);
  }
}

void MQTT_init(){
  macAddr.toCharArray(clientName, sizeof(clientName));
  IPAddress serverIp = resolve_mDNS();
  client.setServer(serverIp, PORT);
  client.setCallback(callback);
}

void MQTT_process(){
  if (!client.connected() && ((millis() - connectAttemptTime) > CONNECT_TIMEOUT)) {     
    // Not connected. Try again.
    mqtt_connect();
    connectAttemptTime = millis();
  }
  client.loop();
}
