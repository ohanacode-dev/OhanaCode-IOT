#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include "web_socket.h"
#include "config.h"

char st_ssid[SSID_SIZE];    /* SSID to connect to */
char st_pass[WIFI_PASS_SIZE];    /* Password for the requested SSID */
String wifi_statusMessage = ""; 
bool connectedToAPFlag; 
unsigned long connectionTimeoutCheck = 0;
IPAddress stationIP;
IPAddress apIP(192, 168, 1, 1);

IPAddress WIFIC_getApIp(void){
  return apIP;
}

static void resetConnectivityTimeout(void){
  connectionTimeoutCheck = millis();
}

/* Performs a wifi scan to check if requested AP is available */
static bool checkApPresent(void){  
  ESP.wdtFeed();
  if(String(st_ssid).length() > 2){
    int n = WiFi.scanNetworks();
    ESP.wdtFeed();
    while(n > 0){
      n--;
      if(WiFi.SSID(n).equals(st_ssid)){
        return true;
      }
    }
  }
  return false; 
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
  if(String(st_ssid).length() > 2){   
    wifi_statusMessage = "Not connected to AP: ";
    wifi_statusMessage += st_ssid;
  }else{
    wifi_statusMessage = "No access point is configured to connect to";    
  }  
  
  Serial.println("\n" + wifi_statusMessage);

  Serial.println("\nStarting AP: Ujagaga_wifi_router");  
  WiFi.mode(WIFI_AP);  
  WiFi.begin();
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  boolean result = WiFi.softAP("Ujagaga_wifi_router");  
  if(result == true){
    Serial.println("Ready");
    Serial.println("IPAddress: 192.168.1.1");
  }else{
    Serial.println("Failed!");
  }


  WiFi.scanNetworks();
}


void WIFIC_stationtMode(void){ 
  if(connectedToAPFlag){
    WiFi.disconnect();
  }
  Serial.println("Trying STA mode.");  
  WiFi.mode(WIFI_STA);  
  WiFi.begin(st_ssid, st_pass);
 
  if((stationIP[0] < 255) && (stationIP[1] < 255) && (stationIP[2] < 255) && (stationIP[3] < 255)){
    IPAddress gateway(stationIP[0], stationIP[1],stationIP[2], 1);
    IPAddress dns(8,8,8,8);
    WiFi.config(stationIP, dns, gateway, IPAddress(255, 255, 255, 0));
  }
  
  /* set timeout in seconds*/
  int i = 1500;  
 
  while((i > 0) && (WiFi.status() != WL_CONNECTED)){
    delay(100);
    ESP.wdtFeed();       
    i--;
  } 
  
  if(WiFi.status() == WL_CONNECTED){
    connectedToAPFlag = true;
    wifi_statusMessage = "Connected to: ";  
    wifi_statusMessage += st_ssid;
    wifi_statusMessage += ". IP address: ";
    wifi_statusMessage += WiFi.localIP().toString();    
    Serial.println("WS2: {\"STATUS\":\"" + wifi_statusMessage + "\"}");  
    WS_ServerBroadcast("{\"STATUS\":\"" + wifi_statusMessage + "\"}");
  }else{    
    Serial.println("accessPointMode");
    WIFIC_APMode(); 
  } 
}



bool WIFIC_IPIsLocal(IPAddress clientIP){   
  IPAddress local_ip = WiFi.localIP();

  if(!connectedToAPFlag){  /* AP mode */
    return true;
  }
  
  if((local_ip[0] == clientIP[0]) && (local_ip[1] == clientIP[1]) && (local_ip[2] == clientIP[2])){   
    return true;
  }
  
  return false;
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



/* Timeout to check if configured AP exists and try to connect to it */
static bool connectivityTimeoutPassed(void){
  unsigned long timeSinceBoot = millis();

  unsigned long largeSecondsPassed = (timeSinceBoot - connectionTimeoutCheck)/1000;  
  
  if(largeSecondsPassed > LARGE_TIMEOUT){
    connectionTimeoutCheck = timeSinceBoot;
    return true;
  }
  return false;
}

void WIFIC_process(void){
  /* Handle AP and STA connecting */
  if((String(st_ssid).length() > 2)){
    if(connectedToAPFlag){
      /* We seem to be connected to AP but we should check */ 
      if(WiFi.status() != WL_CONNECTED){
        connectedToAPFlag = false;
        /* Lost connection to configured AP, so switch to AP mode */ 
        WIFIC_APMode();
      }
      resetConnectivityTimeout();      
    }

    if(connectivityTimeoutPassed()){
      //Serial.println("Timeout!");
      /* Try again */
      if(checkApPresent()){
        WIFIC_stationtMode();
      }         
    } 
  }  
}

void WIFIC_init(void){  
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

  WiFi.setAutoConnect(false);
  
  if(checkApPresent()){
    WIFIC_stationtMode();   
  }else{
    WIFIC_APMode(); 
  }
}
