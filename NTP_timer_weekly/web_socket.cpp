#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "wifi_connection.h"
#include "udp_ping.h"
#include "http.h"
#include "NTP_timer_weekly.h"
#include "ntp.h"
#include "schedule.h"
#include "NTP_timer_weekly.h"

WebSocketsServer wsServer = WebSocketsServer(81);

void WS_process(){
  wsServer.loop();   
}

void WS_ServerBroadcast(String msg){ 
  wsServer.broadcastTXT(msg);  
}

static uint8_t charToNum(char* digits){
  uint8_t result = 0;

  if((digits[0] > 47) && (digits[0] < 58)){
    result = (digits[0] - 48) * 10;
  }

  if((digits[1] > 47) && (digits[1] < 58)){
    result += digits[1] - 48;
  }
  
  return result;
}

static void serverEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{ 
  ESP.wdtFeed();
  
  if(type == WStype_TEXT){
//    Serial.printf("[%u] get Text: %s\r\n", num, payload);
    char textMsg[length];
    for(int i = 0; i < length; i++){
      textMsg[i] = payload[i];          
    }
        
    DynamicJsonDocument doc(128);
    DeserializationError error = deserializeJson(doc, textMsg);    

    // Test if parsing succeeds.
    if (!error) {
      JsonObject root = doc.as<JsonObject>();   

      String broadcastResponse = "{";
      
      if(root.containsKey("ID")){
        String features = HTTP_getFeatures();
        wsServer.sendTXT(num, features);             
      }
      
      if(root.containsKey("APLIST")){  
        String APList = WIFIC_getApList();
        String msg = "{\"APLIST\":\"" + APList + "\"}";
        wsServer.sendTXT(num, msg);   
      }
      
      if(root.containsKey("STATUS")){  
        String statusMsg = MAIN_getStatusMsg(); 
        broadcastResponse += "\"STATUS\":\"" + statusMsg + "\",";
      }

      if(root.containsKey("TIMEZONE")){
        String zoneStr = root["TIMEZONE"];
        if(zoneStr.length() > 0){
          /* Setting time zone */
        
          int zone = zoneStr.toInt();
  
          NTP_setTimeZone(zone);
        }

        zoneStr = String(NTP_getTimeZone());
        broadcastResponse += "\"TIMEZONE\":\"" + zoneStr + "\",";           
      }   

      if(root.containsKey("DLSAVE")){
        String dlSave = root["DLSAVE"];
        if(dlSave.length() > 0){
          /* Setting daylight savings */    
          NTP_setDayLightSavings(dlSave);
        }

        dlSave = NTP_getDayLightSavings();
        broadcastResponse += "\"DLSAVE\":\"" + dlSave + "\"";               
      }

      if(root.containsKey("SCHEDULE")){
        String msg = root["SCHEDULE"];        
       
        uint8_t schedule[14] = {0};
       
        if(msg.length() == 42){ 
          char SchList[43] = {0};
          msg.toCharArray(SchList, 42);
          
          for(int i = 0; i < 7; i++){           
            int dataOffset = i * 6; 
            
            uint8_t fromTime = charToNum(&SchList[dataOffset]);  
            uint8_t toTime = charToNum(&SchList[dataOffset + 3]);
            
            schedule[i * 2] = fromTime;
            schedule[(i * 2) + 1] = toTime;
          }

          SCH_update(schedule);
        }

        SCH_get(schedule);
        String schMsg = "\"SCHEDULE\":\"";   

        for(int i = 0; i < 7; i++){
          String val = String(schedule[i * 2]);
          if(val.length() < 2){
            val = "0" + val;
          }
          schMsg += val + ",";

          val = String(schedule[i * 2 + 1]);
          if(val.length() < 2){
            val = "0" + val;
          }          
          schMsg += val + ",";
        }

        schMsg += "\",";
        
        broadcastResponse += schMsg;
        
      }

      if(root.containsKey("SCHENABLED")){
        String val = root["SCHENABLED"];
        if(val.length() > 0){ 
          int intVal = val.toInt();
          bool newVal = intVal > 0;
          SCH_setEnabled(newVal);
        }           

        broadcastResponse += "\"SCHENABLED\":\"";
        if(SCH_getEnabled()){
          broadcastResponse += "1";
        }else{
          broadcastResponse += "0";
        }

        broadcastResponse += "\",";
      }

      if(root.containsKey("ACTIVE")){
        String val = root["ACTIVE"];
//        if(val.length() > 0){ 
//          int intVal = val.toInt();
//          bool newVal = intVal > 0;
//          MAIN_setActivated(newVal);
//        }           

        broadcastResponse += "\"ACTIVE\":\"";
        if(MAIN_getActivated()){
          broadcastResponse += "1";
        }else{
          broadcastResponse += "0";
        }

        broadcastResponse += "\",";
      }

      // Remove last comma
      int bcMsgLen = broadcastResponse.length();
      broadcastResponse.remove(bcMsgLen - 1);
      broadcastResponse += "}";

      if(broadcastResponse.length() > 4){
        wsServer.broadcastTXT(broadcastResponse); 
      }
    }      
  }   
}

void WS_init(){
  wsServer.begin(); 
  wsServer.onEvent(serverEvent);  
}
