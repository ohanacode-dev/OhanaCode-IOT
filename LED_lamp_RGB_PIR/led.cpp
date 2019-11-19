/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  LED control module.
 */
#include "http.h"
#include "udp_ping.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "led.h"
#include <EEPROM.h>

static uint8_t currentOpacityVal = 0;            /* Current LED level. used in forming HTML and id */
static uint16_t currentRVal = 0;
static uint16_t currentGVal = 0;
static uint16_t currentBVal = 0;

void LED_saveColor(void){
  EEPROM.begin(EEPROM_SIZE);
   
  EEPROM.write(COLOR_ADDR, (uint8_t)(currentRVal >> 8));
  EEPROM.write(COLOR_ADDR + 1, (uint8_t)(currentRVal));
  EEPROM.write(COLOR_ADDR + 2, (uint8_t)(currentGVal >> 8));
  EEPROM.write(COLOR_ADDR + 3, (uint8_t)(currentGVal));
  EEPROM.write(COLOR_ADDR + 4, (uint8_t)(currentBVal >> 8));
  EEPROM.write(COLOR_ADDR + 5, (uint8_t)(currentBVal));
  EEPROM.write(LED_VAL_ADDR, currentOpacityVal);
  
  EEPROM.commit();
  EEPROM.end();

  Serial.println("\nSaving RGBO:");
  Serial.print(currentRVal);
  Serial.print(",");
  Serial.print(currentGVal);
  Serial.print(",");
  Serial.print(currentBVal);
  Serial.print(",");
  Serial.print(currentOpacityVal);
}

static void readColor(void){
  EEPROM.begin(EEPROM_SIZE);  
  currentRVal = (EEPROM.read(COLOR_ADDR) << 8) + EEPROM.read(COLOR_ADDR + 1);
  currentGVal = (EEPROM.read(COLOR_ADDR + 2) << 8) + EEPROM.read(COLOR_ADDR + 3);
  currentBVal = (EEPROM.read(COLOR_ADDR + 4) << 8) + EEPROM.read(COLOR_ADDR + 5);
  currentOpacityVal = EEPROM.read(LED_VAL_ADDR);
  EEPROM.end();

  Serial.println("Reading RGB):");
  Serial.print(currentRVal);
  Serial.print(",");
  Serial.print(currentGVal);
  Serial.print(",");
  Serial.print(currentBVal);
  Serial.print(",");
  Serial.print(currentOpacityVal);
}

int LED_getCurrentVal( void ){
  return currentOpacityVal;
}

rgbValue_t LED_getCurrentRGB( void ){
  rgbValue_t retVal;
  
  retVal.R = currentRVal / 4;
  retVal.G = currentGVal / 4;
  retVal.B = currentBVal / 4; 

  return retVal;
}

void LED_writeRGB(rgbValue_t val){ 
  currentRVal = val.R * 4;
  currentGVal = val.G * 4;
  currentBVal = val.B * 4;

  /* Refresh */
  LED_write(currentOpacityVal);
}

/* Set LED level in percentage */
void LED_write(int val)
{
  
  if(val < 0){
    val = 0;
  }

  if(val > 100){
    val = 100;
  }

  currentOpacityVal = val;
  
  int R = (currentRVal * val)/100; 
  int G = (currentGVal * val)/100; 
  int B = (currentBVal * val)/100; 
  
  analogWrite(LED_R_PIN, R);
  analogWrite(LED_G_PIN, G);
  analogWrite(LED_B_PIN, B);
  
  int opacity = 1024 - (val * 10);  

}

void LED_init(void){  
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
   
  readColor();
  LED_write(currentOpacityVal);
}
