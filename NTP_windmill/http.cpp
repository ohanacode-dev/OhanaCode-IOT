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
#include <FS.h>
#include "http.h"
#include "udp_ping.h"
#include "wifi_connection.h"
#include "web_socket.h"
#include "config.h"
#include "NTP_windmill.h"
#include "ntp.h"
#include "ota.h"


/* Declaring a web server object. */
static ESP8266WebServer webServer(80);

static String getContentType(String filename) { // convert the file extension to the MIME type
  String retType = "text/plain";
  
  if (filename.endsWith(".html")){
    retType = "text/html";
  }else if (filename.endsWith(".css")){
    retType = "text/css";
  }else if (filename.endsWith(".js")){
    retType = "application/javascript";
  }else if (filename.endsWith(".ico")){
    retType = "image/x-icon";
  }else if (filename.endsWith(".png")){
    retType = "image/png";
  }else if (filename.endsWith(".gif")){
    retType = "image/gif";
  }
  return retType;
}

static bool handleFileRead(String path) { // send the right file to the client (if it exists)
  bool retVal = false;
  //Serial.println("handleFileRead: " + path);
  String originalPath = path;
  
  if (path.endsWith("/")){
    // If a folder is requested, send the index file
    path += "index.html";         
  }else if(path.equals("/generate_204") || path.equals("/gen_204")){
    // Sign in request from smartphone
    path = "/index.html";     
  }
  
  String contentType = getContentType(path);            // Get the MIME type
  
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = webServer.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    
    retVal = true;
  }else{
//    Serial.println("FS ERROR: " + originalPath + " not found!");
    
    retVal = false;
  }
  
  return retVal;                                        
}

static void showNotFound(void){
  if (!handleFileRead("/not_found.html")){
    webServer.send(404, "text/plain", "Page not found!");      
  }
}

String HTTP_getFeatures( void ){
//  Serial.print("returnFeatures");
  String response = "{\"MAC\":\"";
  response += WiFi.macAddress();
  response += ":";
  response += DEV_ID;
  response += "\",\"MODEL\":\"OC NTP clock\"";
  response += ",\"CURRENT\":";
  response += NTP_getTimeString();  
  response += "}"; 
//  Serial.println(response); 
  return response;     
}

void showID( void ) {    
//  Serial.println("show ID");
  webServer.send(200, "text/plain", HTTP_getFeatures());  
}

static void showStatusPage() {    
//  Serial.println("showStatusPage");
  String response = "Connection Status:" + MAIN_getStatusMsg();
  webServer.send(200, "text/plain", response);   
}

static void startOtaUpdate(void){
  String statusMsg = "{\"STATUS\":\"Starting OTA update...\"}";               
  WS_ServerBroadcast(statusMsg); 
   
  OTA_init();    
}

static void showRedirectPage(void){
  if (!handleFileRead("/redirect.html")){
     showNotFound();        
  }   
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
    
    if(WIFIC_checkValidIp(newStationIP)){      
      http_statusMessage += ipaddr;
    }else{
      http_statusMessage += "dynamically assigned by DHCP";
    }
    
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
  webServer.on("/wifisave", saveWiFi);
  webServer.on("/start_ota_update", startOtaUpdate);
  
  webServer.onNotFound([]() {                             
    if (!handleFileRead(webServer.uri())){
       showNotFound();        
    }      
  });
  
  webServer.begin();

  if(!SPIFFS.begin()){
//    Serial.println("SPIFFS Initialization failed. Did you enable SPIFFS in \"Tools/Flash size\"?");/
  }

//  Serial.println("\tListing files...");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
//      Serial.println(dir.fileName());      
  }
//  Serial.println("\tEnd of file list...");
}
