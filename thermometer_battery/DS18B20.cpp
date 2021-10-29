#include <OneWire.h>
#include <DallasTemperature.h>
#include "DS18B20.h"

#define ONE_WIRE_BUS              (14)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);

float DS_getTemp(){   
  sensor.begin();

  sensor.requestTemperatures(); 
  float temperatureC = sensor.getTempCByIndex(0);
   
  Serial.print(temperatureC);
  Serial.println("ºC");
  return temperatureC;
}
