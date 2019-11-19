#include <ArduinoOTA.h>
#include "wifi_connection.h"
#include "config.h"

static bool updateStartedFlag = false;

bool OTAA_updateInProgress(){
  return updateStartedFlag;
}

void OTAA_init() {

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(myApName);

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"ohana13");
   
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    SERIAL_PRINTLN("Start updating " + type);

    updateStartedFlag = true;
  });
  ArduinoOTA.onEnd([]() {
    updateStartedFlag = false;
    SERIAL_PRINTLN("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    SERIAL_PRINT("Progress:");
    SERIAL_PRINTLN(progress / (total / 100));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    SERIAL_PRINT("Error: ");
    SERIAL_PRINTLN(error);
    if (error == OTA_AUTH_ERROR) SERIAL_PRINTLN("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) SERIAL_PRINTLN("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) SERIAL_PRINTLN("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) SERIAL_PRINTLN("Receive Failed");
    else if (error == OTA_END_ERROR) SERIAL_PRINTLN("End Failed");
  });
  ArduinoOTA.begin();
}

void OTAA_process(void){
  ArduinoOTA.handle();
}
