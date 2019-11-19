#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "wifi_connection.h"
#include "udp_ping.h"
#include "http.h"
#include "LED_lamp_RGB_PIR.h"
#include "led.h"

WebSocketsServer wsServer = WebSocketsServer(81);

static rgbValue_t strHexToRgb(String hexStr){
    rgbValue_t retRgbVal;

    char hexChrArr[8] = {0};
    hexStr.toCharArray(hexChrArr, hexStr.length() + 1);    
    long rgb;

    if(hexStr[0] == '#'){
      rgb=strtol(&hexChrArr[1], NULL, 16);
    }else{
      rgb=strtol(&hexChrArr[0], NULL, 16);
    }
    
    retRgbVal.R=(uint8_t)(rgb>>16);
    retRgbVal.G=(uint8_t)(rgb>>8);
    retRgbVal.B=(uint8_t)rgb;

    return retRgbVal;
}

static String rgbToHexStr(rgbValue_t Rgb){
    char hexout[8];
    sprintf(hexout,"#%02x%02x%02x",Rgb.R,Rgb.G,Rgb.B);

    return String(hexout);
}

void WS_process(){
  wsServer.loop();   
}

void WS_ServerBroadcast(String msg){
  wsServer.broadcastTXT(msg);
}

static void serverEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{ 
  if(type == WStype_TEXT){
    //Serial.printf("[%u] get Text: %s\r\n", num, payload);
    char textMsg[length];
    for(int i = 0; i < length; i++){
      textMsg[i] = payload[i];          
    }
        
    DynamicJsonDocument doc(128);
    DeserializationError error = deserializeJson(doc, textMsg);    

    // Test if parsing succeeds.
    if (!error) {
      JsonObject root = doc.as<JsonObject>();
      
      if(root.containsKey("CURRENT")){
        String cmd = root["CURRENT"];    
        rgbValue_t rgbClr;
        
        if(cmd.length() > 0){
           
          JsonObject curr = root["CURRENT"];
          if(curr.containsKey("OPA")){              
            int curVal = curr["OPA"]; 
            LED_write(curVal);  
            LED_saveColor();      
          }
          if(curr.containsKey("CLR")){  
            rgbClr = strHexToRgb(curr["CLR"]);            
            LED_writeRGB(rgbClr);   
            LED_saveColor();    
          }
          
        } 
        rgbClr = LED_getCurrentRGB();
        wsServer.broadcastTXT("{\"CURRENT\":{\"OPA\":" + String(LED_getCurrentVal()) + ",\"CLR\":\"" + rgbToHexStr(rgbClr) + "\"}}"); 
      }
      
      if(root.containsKey("ID")){
        String features = HTTP_getFeatures();
        wsServer.sendTXT(num, features);             
      }
      
      if(root.containsKey("APLIST")){  
        String APList = "{\"APLIST\":\"" + WIFIC_getApList() + "\"}";
        wsServer.sendTXT(num, APList);   
      }
      
      if(root.containsKey("STATUS")){
        String statusMsg = "{\"STATUS\":\"" + MAIN_getStatusMsg() + "\"}";               
        wsServer.broadcastTXT(statusMsg);   
      }
    }      
  }   
}

void WS_init(){
  wsServer.begin(); 
  wsServer.onEvent(serverEvent);  
}
