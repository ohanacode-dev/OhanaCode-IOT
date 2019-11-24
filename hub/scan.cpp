/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *   
 */
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "config.h"

#define NO_RESPONSE_MAX_COUNT       (5u)    /* Number of consecutive pings for which we did not get device response, to consider device offline */
#define MAX_IP_LEN                  (15u)


struct devProfile {
  char mac[MAX_MAC_LEN + 1];
  char ip[MAX_IP_LEN + 1];  
  uint8_t id;
  uint8_t noResponseCount;
  uint8_t state;
};


static WiFiUDP Udp;                             /* UDP object used for sending the ping message. */
WiFiServer server(PING_RX_PORT);                /* tcp server for receiving the response */
static devProfile deviceList[MAX_DEV_COUNT] = {{"", "", 0, 0}};

static char incomingPacket[255];                /* buffer for incoming packets */
static unsigned long pingTimestamp = 0;         /* Timestamp of the last reply, so we can implement the reply timeout. */
static const char ping_msg[] = "ujagaga ping"; 


void SCAN_init(){
  server.begin();
  Serial.printf("Listenning UDP on port %d\n\r", PING_RX_PORT);
}

void devListClear(void){
  for(int i = 0; i< MAX_DEV_COUNT; i++){
    if(deviceList[i].noResponseCount > NO_RESPONSE_MAX_COUNT){
      // This device was not heard from for too long. Probably offline.
      deviceList[i].mac[0] = 0;      
    }
  } 
}

int SCAN_getDevId(String deviceMAC){
  for(int i = 0; i < MAX_DEV_COUNT; i++){
    if(deviceList[i].mac[0] != 0){
        String mac = String(deviceList[i].mac);

        if(mac.equals(deviceMAC)){
            return i;
        }
    }
    
  } 
  return -1;
}

String SCAN_getDeviceIPByIndex(uint8_t index){  
  return String(deviceList[index].ip);
}

void devListAppend(String deviceMAC, String deviceIP, int deviceId){
  Serial.print("Apend:");
  Serial.print(deviceMAC);
  Serial.print(":");
  Serial.print(deviceId);
  Serial.print(":");
  Serial.println(deviceIP);
          
  int firsFreeId = -1;
  
  for(int i = 0; i< MAX_DEV_COUNT; i++){
    if((deviceList[i].mac[0] == 0) && (firsFreeId < 0)){
      firsFreeId = i;
    }
    
    if(String(deviceList[i].mac).equals(deviceMAC)){
      deviceList[i].noResponseCount = 0;
      return;
    }   
  } 
  
  // This device was not previously recorded
  if(firsFreeId >= 0){
    // There is some free space;
    deviceMAC.toCharArray( deviceList[firsFreeId].mac, MAX_MAC_LEN);
    deviceList[firsFreeId].mac[MAX_MAC_LEN] = 0;
    deviceIP.toCharArray(deviceList[firsFreeId].ip, MAX_IP_LEN);
    deviceList[firsFreeId].ip[MAX_IP_LEN] = 0;
    deviceList[firsFreeId].id = deviceId;
    deviceList[firsFreeId].noResponseCount = 0;
  }
}

void pingDevices(void){  
  if((millis() - pingTimestamp) > PING_REPPEAT_TIMEOUT){
    Serial.println("UDP ping");
    
    pingTimestamp = millis();
    
    devListClear();
    
    IPAddress broadcastIP = WiFi.localIP();
    broadcastIP[3] = 255;

    // In a noisy environment, UDP packages get lost, so reppeat it few times
    Udp.beginPacket(broadcastIP, PING_TX_PORT);
    Udp.write(ping_msg);
    Udp.endPacket(); 

    delay(1);

    Udp.beginPacket(broadcastIP, PING_TX_PORT);
    Udp.write(ping_msg);
    Udp.endPacket(); 

    delay(1);
    
    Udp.beginPacket(broadcastIP, PING_TX_PORT);
    Udp.write(ping_msg);
    Udp.endPacket(); 
        
    delay(1);
    
    Udp.beginPacket(broadcastIP, PING_TX_PORT);
    Udp.write(ping_msg);
    Udp.endPacket(); 

    delay(1);
    
    Udp.beginPacket(broadcastIP, PING_TX_PORT);
    Udp.write(ping_msg);
    Udp.endPacket(); 

  }
}

void SCAN_process(){
  pingDevices();

  WiFiClient client = server.available();
  if (client) {
    while (!client.connected()){ESP.wdtFeed();};
    while (client.connected())
    {
      if (client.available())
      {
        String deviceMAC = client.readStringUntil('\n');
        String deviceIp = client.remoteIP().toString();  
        int deviceId = 0;
        client.stop();  
        
          /* Check if in MAC format */
        if((deviceMAC.length() > 17) && (deviceMAC.charAt(2) == ':') && (deviceMAC.charAt(5) == ':') && (deviceMAC.charAt(8) == ':') 
              && (deviceMAC.charAt(11) == ':') && (deviceMAC.charAt(14) == ':') && (deviceMAC.charAt(17) == ':')){
            deviceMAC.replace(":", "");
           
            deviceId = deviceMAC.substring(12).toInt();
            deviceMAC = deviceMAC.substring(0, 12);            
                  
            devListAppend(deviceMAC, deviceIp, deviceId);                      
        }else{
          Serial.print("FAIL:");
          Serial.println(deviceMAC);
        }
      }
    }
  }  
}



String SCAN_getDevList(void){
  String response = "{devs:";  

  for(int i = 0; i < MAX_DEV_COUNT; i++){
    devProfile dev = deviceList[i];

    if(dev.mac[0] != 0){
        response += "{ip:" + String(dev.ip) + ", label:" + getLabel(String(dev.mac)) + ", state:" + String(dev.state) + "},";
    }
    response += "}";
    
  }

  return response;
}
