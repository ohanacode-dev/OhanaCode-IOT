/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  HTTP server which generates the web browser pages. 
 */
 
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <pgmspace.h>
#include "http.h"
#include "wifi_connection.h"
#include "web_socket.h"
#include "config.h"
#include "LED_lamp_RGB_Neopixel.h"
#include "led.h"
#include "ota.h"
#include "html_ui.h"


/* Declaring a web server object. */
static ESP8266WebServer webServer(80);

static void showNotFound(void){
  webServer.send(404, "text/plain", "Sorry, no such item here!"); 
}

String HTTP_getFeatures( void ){
  uint8_t val[4];
  LED_getCurrentRGBA( val );
  
  Serial.print("returnFeatures");
  String response = "{\"MAC\":\"";
  response += WiFi.macAddress();
  response += "\",\"MODEL\":\"ujagaga WiFi RGBA LED\"";
  response += ",\"CURRENT\":[" + String(val[0]) + "," + String(val[1]) + "," + String(val[2])+ "," + String(val[3]) + "]}"; 
  Serial.println(response); 
  return response;     
}

void showID( void ) {    
  Serial.println("show ID");
  webServer.send(200, "text/plain", HTTP_getFeatures());  
}

static void showStatusPage() {    
  Serial.println("showStatusPage");
  String response = "Connection Status:" + MAIN_getStatusMsg();
  webServer.send(200, "text/plain", response);   
}

static void startOtaUpdate(void){  
  OTA_init();    
}

static void showRedirectPage(void){
  webServer.send(200, "text/html", F(redirect_html));   
}

static void showHome(void){
  webServer.send(200, "text/html", F(index_html));   
}

static void showSelectAP(void){
  webServer.send(200, "text/html", F(select_ap_html));   
}

/* Saves wifi settings to EEPROM */
static void saveWiFi(void){
  String ssid = webServer.arg("s");
  String pass = webServer.arg("p");
  String ipaddr = webServer.arg("a");
  
  if((ssid.length() > 63) || (pass.length() > 63)){
      MAIN_setStatusMsg("Sorry, this module can only remember SSID and a PASSWORD up to 63 bytes long.");
      showRedirectPage(); 
      return;
  } 

  IPAddress newStationIP;
  newStationIP.fromString(ipaddr);

  String st_ssid = "";
  String st_pass = "";
  IPAddress stationIP;

  if(ssid.length() > 0){
    bool cmpFlag = true;

    st_ssid = WIFIC_getStSSID();
    st_pass = WIFIC_getStPass();
    stationIP = WIFIC_getStIP();

    if(!st_ssid.equals(ssid) || !st_pass.equals(pass)){
      cmpFlag = false;
    }
      
    if(cmpFlag){
       if((newStationIP[0] != stationIP[0]) || (newStationIP[1] != stationIP[1]) || (newStationIP[2] != stationIP[2]) || (newStationIP[3] != stationIP[3])){
          cmpFlag = false;
       }
    }
  
    if(cmpFlag){
      MAIN_setStatusMsg("All parameters are already set as requested.");
      showRedirectPage();      
      return;
    }   
  }
  
  WIFIC_setStSSID(ssid);
  WIFIC_setStPass(pass);
  WIFIC_setStIP(newStationIP);

  String http_statusMessage;
  
  if(ssid.length() > 3){    
    http_statusMessage = "Saving settings and connecting to SSID: ";
    http_statusMessage += ssid; 
    http_statusMessage += " ,IP: ";
    
    http_statusMessage += ipaddr;   
    
  }else{       
    http_statusMessage = "Saving settings and switching to AP mode only.";    
  }
  http_statusMessage += "<br>If you can not connect to this device 20 seconds from now, please, reset it.";

  MAIN_setStatusMsg(http_statusMessage);
  showStatusPage();

  volatile int i;

  /* Keep serving http to display the status page*/
  for(i = 0; i < 100000; i++){
    webServer.handleClient(); 
    ESP.wdtFeed();
  } 

  /* WiFI config changed. Restart to apply. 
   Note: ESP.restart is buggy after programming the chip. 
   Just reset once after programming to get stable results. */
  ESP.restart();
}

void HTTP_process(void){
  webServer.handleClient(); 
}

void HTTP_init(void){ 
  webServer.on("/id", showID);
  webServer.on("/favicon.ico", showNotFound);
  webServer.on("/wifisave", saveWiFi);
  webServer.on("/start_ota_update", startOtaUpdate);
  webServer.on("/select_ap", showSelectAP);
  webServer.on("/", showHome);
  
  webServer.onNotFound([]() {                             
    showNotFound();      
  });
  
  webServer.begin();
 
}
