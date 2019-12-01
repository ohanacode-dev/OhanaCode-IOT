#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include <WebSocketsClient.h>
#include "wifi_connection.h"
#include "scan.h"
#include "http.h"
#include "hub.h"

WebSocketsServer wsServer = WebSocketsServer(81);
WebSocketsClient wsClient;
static String pendingMessage = "";
static bool responseReceivedFlag = false;
static uint32_t msgSentTimestamp = 0;

void WS_sendToDevice(String devIp, String msg){
  /* Connect client. */
  wsClient.begin(devIp, 81, "/");
  wsClient.onEvent(wsClientEvent);
  wsClient.setReconnectInterval(5000);
  
  /* Prepare the message in the queue. */
  responseReceivedFlag = false;
  pendingMessage = msg; 
}

bool WS_getClientState(String devIp){
  bool retVal = false;

  if(pendingMessage.length() < 2){
    
  }
}

void WS_ServerBroadcast(String msg){
  wsServer.broadcastTXT(msg);
}

static void wsClientEvent(WStype_t type, uint8_t * payload, size_t length) {
  if(type == WStype_TEXT){
    //Serial.printf("[WSc] get text: %s\n", payload);
    char textMsg[length + 1];
    for(int i = 0; i < length; i++){
      textMsg[i] = payload[i];          
    }
    textMsg[length] = 0; /* Finish the string */

    DynamicJsonDocument doc(128);
    DeserializationError error = deserializeJson(doc, textMsg); 

    if (!error) {
      JsonObject root = doc.as<JsonObject>();
      if(root.containsKey("ID")){    
       
      }else{
        // send message to the connected client
        //wsServer.broadcastTXT(payload);
      }
    }     
  }
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

void WS_process(){
  wsServer.loop();   
  wsClient.loop();
}
