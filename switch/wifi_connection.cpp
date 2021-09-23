/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  WiFi connection module. At startup connects to a user configured access point. If none is configured, creates its own access point to provide a way to adjust settings.
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include "web_socket.h"
#include "config.h"


static char myApName[32] = {0};    /* Array to form AP name based on read MAC */
static char st_ssid[SSID_SIZE];    /* SSID to connect to */
static char st_pass[WIFI_PASS_SIZE];    /* Password for the requested SSID */
static unsigned long connectionTimeoutCheck = 0;
static IPAddress stationIP;
static IPAddress apIP(192, 168, 1, 1);

static bool checkValidIp(IPAddress IP){
  /* check if they are all zero value */
  if((IP[0] == 0) && (IP[1] == 0) && (IP[2] == 0) && (IP[3] == 0)){
      return false;
  }

  /* Check that they are not all 0xff (defaut empty flash value) */
  if((IP[0] == 0xff) && (IP[1] == 0xff) && (IP[2] == 0xff) && (IP[3] == 0xff)){
      return false;
  }
    
  return true;
}

char* WIFIC_getDeviceName(void){
  return myApName;
}

IPAddress WIFIC_getApIp(void){
  return apIP;
}

/* Returns wifi scan results */
String WIFIC_getApList(void){
  String result = "";
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks(); 
  if (n >0){ 
    result = WiFi.SSID(0); 
    for (int i = 1; i < n; ++i)
    {      
      result += "|" + WiFi.SSID(i);  
    }
  }
  return result;
}

/* Initiates a local AP */
void WIFIC_APMode(void){ 
  String wifi_statusMessage;
  
  if(String(st_ssid).length() > 2){   
    wifi_statusMessage = "Not connected to AP: ";
    wifi_statusMessage += st_ssid;
  }else{
    wifi_statusMessage = "No access point is configured to connect to";    
  }  
     
  Serial.println("\n" + wifi_statusMessage);

  Serial.println("\nStarting AP");  
  WiFi.mode(WIFI_AP);  
  WiFi.begin();
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  
  if(WiFi.softAP(myApName)){
    wifi_statusMessage = "Running in AP mode. SSID: " + String(myApName) + ", IP:" + apIP.toString();  
  }else{
    wifi_statusMessage = "Failed to switch to AP mode.";
  }
  Serial.println(wifi_statusMessage);  

  /* Once you setup an AP for the device to connect to, and it is not found at startup, so AP mode is atempted,
  * the device remains unconnectable without the original AP. I have not been able to find why, 
  * but the following seems to fix the problem. */
  WiFi.scanNetworks();
}


void WIFIC_stationMode(void){   
  Serial.println("Trying STA mode."); 
  WiFi.mode(WIFI_STA);  
  WiFi.begin(st_ssid, st_pass);

  if( checkValidIp(stationIP)){
    IPAddress gateway(stationIP[0], stationIP[1],stationIP[2], 1);
    IPAddress dns(8,8,8,8);
    WiFi.config(stationIP, dns, gateway, IPAddress(255, 255, 255, 0));
  }
  
  /* set timeout to 30 seconds*/
  int i = 300;  
 
  while((i > 0) && (WiFi.status() != WL_CONNECTED)){
    delay(100);
    ESP.wdtFeed();       
    i--;
  } 
  
  if(WiFi.status() == WL_CONNECTED){
    stationIP = WiFi.localIP();
    
    String wifi_statusMessage = "Connected to: ";  
    wifi_statusMessage += st_ssid;
    wifi_statusMessage += ". IP address: " + stationIP.toString();  
    
    Serial.println(wifi_statusMessage);
  }else{    
    Serial.println("accessPointMode");
    WIFIC_APMode(); 
  } 
}

String WIFIC_getStSSID(void){
  return String(st_ssid);
}

void WIFIC_setStSSID(String new_ssid){
  EEPROM.begin(EEPROM_SIZE);
  
  uint16_t addr;
  
  for(addr = 0; addr < new_ssid.length(); addr++){
    EEPROM.write(addr + SSID_EEPROM_ADDR, new_ssid[addr]);
    st_ssid[addr] = new_ssid[addr];
  }
  EEPROM.write(addr + SSID_EEPROM_ADDR, 0);  
  st_ssid[addr] = 0;
  
  EEPROM.commit();
  EEPROM.end();
}

String WIFIC_getStPass(void){
  return String(st_pass);
}

void WIFIC_setStPass(String new_pass){
  EEPROM.begin(EEPROM_SIZE);
  
  uint16_t addr;
  for(addr = 0; addr < new_pass.length(); addr++){
    EEPROM.write(addr + WIFI_PASS_EEPROM_ADDR, new_pass[addr]);
    st_pass[addr] = new_pass[addr];
  }
  EEPROM.write(addr + WIFI_PASS_EEPROM_ADDR, 0);
  st_pass[addr] = 0;
    
  EEPROM.commit();
  EEPROM.end();
}

IPAddress WIFIC_getStIP(void){
  return stationIP;
}

void WIFIC_setStIP(IPAddress newStationIP){
  stationIP = newStationIP;
  
  EEPROM.begin(EEPROM_SIZE);
    
  EEPROM.write(STATION_IP_ADDR + 0, newStationIP[0]);
  EEPROM.write(STATION_IP_ADDR + 1, newStationIP[1]);
  EEPROM.write(STATION_IP_ADDR + 2, newStationIP[2]);
  EEPROM.write(STATION_IP_ADDR + 3, newStationIP[3]);
  
  EEPROM.commit();
  EEPROM.end();
}


void WIFIC_init(void){  
  ESP.wdtFeed();
   /* Read settings from EEPROM */
  EEPROM.begin(EEPROM_SIZE);
  uint16_t i = 0;
  
  do{
    st_pass[i] = EEPROM.read(i + WIFI_PASS_EEPROM_ADDR);
    if((st_pass[i] < 32) || (st_pass[i] > 126)){
      /* Non printable character */
      break;
    }
    i++;
  }while(i < WIFI_PASS_SIZE);
  st_pass[i] = 0;

  i = 0;
  do{
    st_ssid[i] = EEPROM.read(i + SSID_EEPROM_ADDR);
    if((st_ssid[i] < 32) || (st_ssid[i] > 126)){
      /* Non printable character */
      break;
    }
    i++;
  }while(i < SSID_SIZE);
  st_ssid[i] = 0;

  stationIP[0] = EEPROM.read(STATION_IP_ADDR + 0);
  stationIP[1] = EEPROM.read(STATION_IP_ADDR + 1);
  stationIP[2] = EEPROM.read(STATION_IP_ADDR + 2);
  stationIP[3] = EEPROM.read(STATION_IP_ADDR + 3);
   
  EEPROM.end();

  String mac = WiFi.macAddress();
  mac.replace(":", "");
  String ApName = AP_NAME_PREFIX + mac;
  ApName.toCharArray(myApName, ApName.length() + 1);
  
  WIFIC_stationMode();   
  
}
