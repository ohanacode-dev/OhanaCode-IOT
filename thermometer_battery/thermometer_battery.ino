#include "config.h"
#include "wifi_connection.h"
#include "mqtt.h"

void setup(void) {
	/* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
  Serial.begin(115200);   
  WIFIC_init();
}


void loop(void) {
  if(MQTT_connect()){
    ESP.deepSleep(DEEP_SLEEP_TIME);
  }
}
