#include "http.h"
#include "udp.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "email_sender.h"

static unsigned long externalIPTimeout = 0;
static String externalIP = "";

static String statusMessage = "";         /* This is set and requested from other modules. */

void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

void checkExternalIP( void ){
  if(((externalIPTimeout == 0) && (millis() > 10000)) ||
      ((millis() - externalIPTimeout) > (EXT_IP_CHK_INTERVAL * 1000))){
    externalIPTimeout = millis();
    String currentExternalIP = HTTP_getExternalIP();
    
    if(!currentExternalIP.equals(externalIP)){
      externalIP = currentExternalIP;
      Serial.println("New external IP: " + externalIP);    

      String emailMsg;
      
      if(externalIP == ""){
        emailMsg = "External IP service is not available.";
      }else{
        emailMsg = "New external IP: http://" + externalIP;
      }
      
      if(!EMAIL_send(emailMsg)){
        String email_status = "Error sending email: " + EMAIL_getError();
        WS_ServerBroadcast(email_status);
      }
    }    
  }
}

void setup(void) {
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY); /* Use only tx, so rx can be used as GPIO */   
  //ESP.eraseConfig();
  WIFIC_init(); 
  ESP.wdtFeed();
  HTTP_init();
  ESP.wdtFeed();
  WS_init();  
  ESP.wdtFeed();
  UDP_init();   
  ESP.wdtFeed();
  EMAIL_init();
}


void loop(void) {
  ESP.wdtFeed();
  HTTP_process();
  ESP.wdtFeed();
  WS_process();
  ESP.wdtFeed();
  UDP_process();
  ESP.wdtFeed();
  WIFIC_process();  
  ESP.wdtFeed();
  checkExternalIP();
}
