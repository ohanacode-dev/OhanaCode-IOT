/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  Ping receive and response module. 
 *  Any device trying to get a list of devices on the network must send a UDP broadcast packet containing the string "ujagaga ping".
 *  The UDP broacast was selected because it is the only network package type that can be broadcasted and not target a specific IP, 
 *  but it is not guaranteed to arrive because it does not have any handshaking protocols, so we are sending several UDP ping packets, 
 *  hoping that every device on the network will receive at least one. 
 *  After receiving it, this device should respond via TCP socket. Unlike UDP, TCP does implement a handshaking protocol and is guaranteed 
 *  to arrive, so it is enough to send only one packet as a response.
 *  We define a timeout here and do not reppeat a response before the timeout expires. This is to reduce unnecessary network trafick.
 */
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "config.h"

#define PING_REPPEAT_TIMEOUT    (100u)          /* Timeout in ms before which not to reppeat a ping response to prevent network overload. */

static WiFiUDP Udp;                             /* UDP object used for receiving the ping message. */
static char incomingPacket[255];                /* buffer for incoming packets */
static char replyMsg[21] = {0};                 /* Setup prepares the reply here. The content is only the chips MAC. Different for each chip, so must be populated at startup */
static uint16_t replyLength = 0;                /* Length of the reply package to be used in TCP communication. */
static const char ping_msg[] = "ujagaga ping";  /* String to respond to */
static unsigned long pingTimestamp = 0;         /* Timestamp of the last reply, so we can implement the reply timeout. */

void UDPPING_init(){
  Udp.begin(PING_RX_PORT);
  String UdpRreplyStr = WiFi.macAddress();
  UdpRreplyStr += ":";
  UdpRreplyStr += DEV_ID;
  UdpRreplyStr += "\n";

  replyLength = UdpRreplyStr.length() + 1;
  
  UdpRreplyStr.toCharArray(replyMsg, replyLength);
  
}

void UDPPING_process(){
  int packetSize = Udp.parsePacket();
  
  if (packetSize == String(ping_msg).length())
  {
    // receive incoming UDP packets
    int len = Udp.read(incomingPacket, String(ping_msg).length());
    incomingPacket[len] = 0;
    
    if(String(incomingPacket).equals(ping_msg)){

      if((millis() - pingTimestamp) > PING_REPPEAT_TIMEOUT){
        /* send back a reply, to the IP address and port + 1 that we got the packet from */
        WiFiClient tcpClient;
    
        if (tcpClient.connect(Udp.remoteIP(), PING_RESPONSE_PORT)){ 
          tcpClient.write(replyMsg, replyLength);
          tcpClient.stop();
  
          pingTimestamp = millis();
        }
      } 
    }    
  }
}
