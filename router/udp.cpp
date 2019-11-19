#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "config.h"

static WiFiUDP Udp;
static char incomingPacket[255];         /* buffer for incoming packets */
static String deviceList[MAX_DEV_COUNT];
static String deviceIPList[MAX_DEV_COUNT];
static uint8_t devCount = 0;

void UDP_init(){
  Udp.begin(UDP_PORT);
}

void UDP_devCountClear(void){
  devCount = 0;
}

String UDP_devListToJson(void){
  String result = "{\"DEVICES\":\"";

  if(devCount > 0){
    result += deviceList[0];
    for(int i = 1; i< devCount; i++){
      result += "|" + deviceList[i];
    }
  }
  
  result += "\"}";
  return result;
}

int UDP_getDevListId(String deviceMAC){
  for(int i = 0; i< devCount; i++){
    if(deviceList[i].equals(deviceMAC)){
      return i;
    }
  } 
  return -1;
}

String UDP_getDeviceIP(uint8_t devId){
  return deviceIPList[devId];
}

void devListAppend(String deviceMAC, String deviceIP){
  if(UDP_getDevListId(deviceMAC) < 0){
    if(devCount < MAX_DEV_COUNT){
      deviceList[devCount] = deviceMAC;
      deviceIPList[devCount] = deviceIP;
      devCount++;
    }
  }
}

void UDP_pingDevices(void){  

  IPAddress broadcastIP = WiFi.localIP();
  broadcastIP[3] = 255;
  
  Udp.beginPacket(broadcastIP, UDP_PORT);
  Udp.write("ujagaga ping");
  Udp.endPacket(); 
}

void UDP_process(){
  int packetSize = Udp.parsePacket();
  if (packetSize > 16)
  {
    //Serial.print("UDP rcv:");
    //Serial.println(packetSize);
    /* receive incoming UDP packets */
    int len = Udp.read(incomingPacket, 20);
    incomingPacket[len] = 0;

    String deviceMAC = String(incomingPacket);  

    /* Check if in MAC format */
    if(deviceMAC.length() > 17){
      if((deviceMAC.charAt(2) == ':') && (deviceMAC.charAt(5) == ':') && (deviceMAC.charAt(8) == ':') 
          && (deviceMAC.charAt(11) == ':') && (deviceMAC.charAt(14) == ':') && (deviceMAC.charAt(17) == ':')){
        deviceMAC.replace(":", "");
        String deviceIp = Udp.remoteIP().toString();        
        devListAppend(deviceMAC, deviceIp);                 
      }      
    }   
  }
}
