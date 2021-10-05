/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  LED control module.
 */
 #include <EEPROM.h>
#include "http.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "led.h"
#include "mqtt.h"


static unsigned long LedWriteTimestamp = 0;
static uint8_t currentRVal = 0;
static uint8_t currentGVal = 0;
static uint8_t currentBVal = 0;
static uint8_t currentAVal = 0;

void LED_saveColor(void){
  EEPROM.begin(EEPROM_SIZE);
   
  EEPROM.write(COLOR_ADDR, currentRVal);
  EEPROM.write(COLOR_ADDR + 1, currentGVal);
  EEPROM.write(COLOR_ADDR + 2, currentBVal);
  EEPROM.write(COLOR_ADDR + 3, currentAVal);
  
  EEPROM.commit();
  EEPROM.end();

  Serial.println("\nSaving RGBA:");
  Serial.print(currentRVal);
  Serial.print(",");
  Serial.print(currentGVal);
  Serial.print(",");
  Serial.print(currentBVal);
  Serial.print(",");
  Serial.print(currentAVal);
}

static void readColor(void){
  EEPROM.begin(EEPROM_SIZE);  
  currentRVal = EEPROM.read(COLOR_ADDR);
  currentGVal = EEPROM.read(COLOR_ADDR + 1);
  currentBVal = EEPROM.read(COLOR_ADDR + 2);
  currentAVal = EEPROM.read(COLOR_ADDR + 3);
  EEPROM.end();

  Serial.println("Reading RGBA:");
  Serial.print(currentRVal);
  Serial.print(",");
  Serial.print(currentGVal);
  Serial.print(",");
  Serial.print(currentBVal);
  Serial.print(",");
  Serial.print(currentAVal);
}

/* Set LED level in percentage */
static void LED_write()
{
  if((millis() - LedWriteTimestamp) < DEBOUNCE_TIMEOUT){
    return;
  }  

  int R = 255 * currentRVal / 100;
  int G = 255 * currentGVal / 100;
  int B = 255 * currentBVal / 100;
  
  R = R * currentAVal/100; 
  G = G * currentAVal/100; 
  B = B * currentAVal/100; 
 
  analogWrite(LED_R_PIN, R);
  analogWrite(LED_G_PIN, G);
  analogWrite(LED_B_PIN, B);

//  Serial.print("LED:");
//  Serial.print(R);
//  Serial.print(",");
//  Serial.print(G);
//  Serial.print(",");
//  Serial.print(B);
//  Serial.println(".");

  /* Status pin is usually the onboard LED which is connected to Vcc and Pin2, so invert the value. */
  int opacity = 255 - (255 * currentAVal / 100);  
  analogWrite(LED_STATUS_PIN, opacity);

  LedWriteTimestamp = millis();  

  MQTT_setCurrentStatus();

  String statusMsg = "{\"CURRENT\":[" + String(currentRVal) + "," + String(currentGVal) + "," + String(currentBVal)+ "," + String(currentAVal) + "]}";  
  WS_broadcast(statusMsg);  
}

void LED_getCurrentRGBA( uint8_t* val ){
  
  val[0] = currentRVal;
  val[1] = currentGVal;
  val[2] = currentBVal;
  val[3] = currentAVal;
}

void LED_writeRGBA(uint8_t* val){ 
  currentRVal = val[0];
  currentGVal = val[1];
  currentBVal = val[2];
  currentAVal = val[3];

  if(currentRVal > 100){
    currentRVal = 100;
  }
  if(currentGVal > 100){
    currentGVal = 100;
  }
  if(currentBVal > 100){
    currentBVal = 100;
  }
  if(currentAVal > 100){
    currentAVal = 100;
  }
  
  LED_write();  
}

void LED_init(void){  
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
   
  readColor();
  LED_write();
}
