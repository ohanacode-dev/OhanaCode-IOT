/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  Over The Air update module.
 *  
 *  Arduino Studio offers OTA update. You can choose the device like you choose the serial port.
 *  NOTE: After uploading code via UART, make sure to restart the device before atempting OTA.
 */
#include <ArduinoOTA.h>
#include "wifi_connection.h"
#include "config.h"


static bool updateStartedFlag = false;

bool OTA_updateInProgress(){
  return updateStartedFlag;
}

void OTA_init() {

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(WIFIC_getDeviceName());

  // No authentication by default
//  ArduinoOTA.setPassword((const char *)"pass123");
   
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_LittleFS
      type = "filesystem";

    // NOTE: if updating LittleFS this would be the place to unmount LittleFS using LittleFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    updateStartedFlag = false;
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.print("Progress:");
    Serial.println(progress / (total / 100));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.print("Error: ");
    Serial.println(error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  Serial.println("\nOTA initialized");
  updateStartedFlag = true;
}

void OTA_process(void){
  ArduinoOTA.handle();
}
