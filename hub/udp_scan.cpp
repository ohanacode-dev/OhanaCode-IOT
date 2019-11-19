/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *   
 */
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "config.h"


struct devProfile {
  String devId;
  String devIp;  
};

static WiFiUDP Udp;                             /* UDP object used for receiving the ping message. */
devProfile deviceList[MAX_DEV_COUNT];
static uint8_t devCount = 0;


static char incomingPacket[255];                /* buffer for incoming packets */
static char replyMsg[21] = {0};                 /* Setup prepares the reply here. The content is only the chips MAC. Different for each chip, so must be populated at startup */
static uint16_t replyLength = 0;                /* Length of the reply package to be used in TCP communication. */
static const char ping_msg[] = "ujagaga ping";  /* String to respond to */
static unsigned long pingTimestamp = 0;         /* Timestamp of the last reply, so we can implement the reply timeout. */

void UDPSCAN_init(){
  Udp.begin(UDP_PORT);
}

void devCountClear(void){
  devCount = 0;
}

int UDPSCAN_getDevListId(String deviceMAC){
  for(int i = 0; i< devCount; i++){
    if(deviceList[i].devId.equals(deviceMAC)){
      return i;
    }
  } 
  return -1;
}
String UDPSCAN_getDeviceIPByIndex(uint8_t index){  
  return deviceList[index].devIp;
}

void devListAppend(String deviceMAC, String deviceIP){
  if(UDPSCAN_getDevListId(deviceMAC) < 0){
    if(devCount < MAX_DEV_COUNT){
      deviceList[devCount].devId = deviceMAC;
      deviceList[devCount].devIp = deviceIP;
      devCount++;
    }
  }
}

void UDPSCAN_pingDevices(void){  
  devCountClear();
  IPAddress broadcastIP = WiFi.localIP();
  broadcastIP[3] = 255;
  
  Udp.beginPacket(broadcastIP, UDP_PORT);
  Udp.write("ujagaga ping");
  Udp.endPacket(); 
}

void UDPSCAN_process(){
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
