#include <OneWire.h>
#include <DallasTemperature.h>
#include "DS18B20.h"
#include "web_therm.h"
#include <ESP_EEPROM.h>

#define ONE_WIRE_BUS              (13)
#define TEMP_CHK_TIMEOUT          (60000)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);
// arrays to hold device address
DeviceAddress insideThermometer;

static float tempC = 0;
static uint32_t tempChkTimestamp = 0;
static int targetTemp = 29;

int DS_getTargetTemp(){
  return targetTemp;
}

void DS_setTargetTemp(int temp){
  if(targetTemp != temp){
    EEPROM.begin(1);
    EEPROM.put(0, (uint8_t)targetTemp);
    EEPROM.commit();
  }
  targetTemp = temp;  
}

float DS_getTemp(){
  return tempC;
}

void DS_process(){
  if((millis() - tempChkTimestamp) > TEMP_CHK_TIMEOUT){
    sensor.requestTemperatures();
    tempC = sensor.getTempC(insideThermometer);
    tempChkTimestamp = millis();
  }

  if(MAIN_getState()){
    MAIN_setRelayOn(tempC < targetTemp);
  }
}

void DS_init(){
  EEPROM.begin(1);
  targetTemp = EEPROM.read(0);
  EEPROM.end();
  
  sensor.begin();

  // Prepare the sensor
  int deviceCount = sensor.getDeviceCount();
  
  Serial.print("Found ");
  Serial.print(deviceCount, DEC);
  Serial.println(" DS18B20 devices.");  

  if(deviceCount > 0){
    if (!sensor.getAddress(insideThermometer, 0)) {
      Serial.println("Unable to find address for Device 0");
    }else{
      sensor.setResolution(insideThermometer, 9); 
      Serial.print("Device 0 Resolution: ");
      Serial.print(sensor.getResolution(insideThermometer), DEC); 
      Serial.println();
      
      sensor.requestTemperatures();
      tempC = sensor.getTempC(insideThermometer);
    }  
  }
}
