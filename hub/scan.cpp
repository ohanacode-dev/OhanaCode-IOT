/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *   
 */
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "config.h"


struct devProfile {
  String devMac;
  String devIp;  
  int devId;
};

static WiFiUDP Udp;                             /* UDP object used for sending the ping message. */
WiFiServer server(PING_RX_PORT);                /* tcp server for receiving the response */
devProfile deviceList[MAX_DEV_COUNT];
static uint8_t devCount = 0;
static char incomingPacket[255];                /* buffer for incoming packets */
static unsigned long pingTimestamp = 0;         /* Timestamp of the last reply, so we can implement the reply timeout. */
static const char ping_msg[] = "ujagaga ping"; 

void SCAN_init(){
  server.begin();
  Serial.printf("Listenning UDP on port %d\n\r", PING_RX_PORT);
}

void devCountClear(void){
  devCount = 0;
  deviceList[devCount].devMac = "";
  deviceList[devCount].devIp = "";
  deviceList[devCount].devId = 0;
}

int SCAN_getDevListId(String deviceMAC){
  for(int i = 0; i< devCount; i++){
    if(deviceList[i].devMac.equals(deviceMAC)){
      return i;
    }
  } 
  return -1;
}
String SCAN_getDeviceIPByIndex(uint8_t index){  
  return deviceList[index].devIp;
}

void devListAppend(String deviceMAC, String deviceIP, int deviceId){
  if(SCAN_getDevListId(deviceMAC) < 0){
    if(devCount < MAX_DEV_COUNT){
      deviceList[devCount].devMac = deviceMAC;
      deviceList[devCount].devIp = deviceIP;
      deviceList[devCount].devId = deviceId;
      devCount++;
    }
  }
}

void pingDevices(void){  
  if((millis() - pingTimestamp) > PING_REPPEAT_TIMEOUT){
    Serial.println("UDP ping");
    
    pingTimestamp = millis();
    
    devCountClear();
    
    IPAddress broadcastIP = WiFi.localIP();
    broadcastIP[3] = 255;
  
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
        client.stop();  
        
          /* Check if in MAC format */
        if((deviceMAC.length() > 17) && (deviceMAC.charAt(2) == ':') && (deviceMAC.charAt(5) == ':') && (deviceMAC.charAt(8) == ':') 
              && (deviceMAC.charAt(11) == ':') && (deviceMAC.charAt(14) == ':') && (deviceMAC.charAt(17) == ':')){
            deviceMAC.replace(":", "");
           
            int deviceId = deviceMAC.substring(12).toInt();
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
