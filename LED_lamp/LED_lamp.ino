/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  This is the main sketch file. 
 *  It provides a touch sensor functionality and a periodic pooling of other services.
 */

#include "http.h"
#include "udp_ping.h"
#include "web_socket.h"
#include "wifi_connection.h"
#include "config.h"
#include "led.h"
#include "ota.h"


static String statusMessage = "";         /* This is set and requested from other modules. */
static unsigned long touchTime = 0;       /* Duration of a touch */
static bool touchFlag = false;            /* Flag is set when a touch is detected. */
static unsigned long touchStartTime = 0;  /* First touch detection timestamp */
static unsigned long executeTime = 0;     /* On touch LED level change timestamp */
static int lastLedVal = 0;                /* History to be used with touch sensor function (Value to restore to) */

/* Processes the touch detection. The sensor is treated as a variable capacitor. 
 * When the user touches it, the capacity increases. We are measuring the charge time 
 * to determine if the capacity is higher than usual. If you are using a small conductive 
 * surface as a sensor (tipically up to 2 cm in diameter), if it is not touched, 
 * it should not take longer than 1ms to charge to a HIGH level.
 */
static void updateTouchSensor(){ 
  /* Discharge the capacitor. */
  pinMode(SENSPIN, OUTPUT);
  digitalWrite(SENSPIN, LOW);  
  delay(1); 

  /* Start charging and measure the time it takes to detect a HIGH level. */
  volatile uint8_t chargeTime = 0;
  pinMode(SENSPIN, INPUT_PULLUP);
  while(digitalRead(SENSPIN) == LOW){
    chargeTime++;
  }

  /* Get the currently set LED PWM value. */
  int currentLedVal = LED_getCurrentVal();
  
  if(chargeTime > SENSOR_IDLE_CHARGE_MIN_TIME){ 
    /* It took longer to charge the capacitor. Considering the sensor touched.  */
    touchTime = millis();
    
    if(!touchFlag){
      /* Just pressed. Set touch timestamp so we can detect how long the user is holding it. */
      touchStartTime = touchTime;
      executeTime = touchTime;      
    }

    if((touchTime - executeTime) > LONG_TOUCH_TIMEOUT){
      /* Long press, so increment LED */  
      LED_write(currentLedVal + LONG_TOUCH_LED_INCREMENT);
      executeTime = touchTime;
    } 

    touchFlag = true;   
  }else{
    /* Debounce */  
    if((millis() - touchTime) > DEBOUNCE_TIMEOUT){ 
      if(touchFlag){
        /* just released */        
        if((touchTime - touchStartTime) < LONG_TOUCH_TIMEOUT){  
          /* Short touch */          
          if( currentLedVal > 0){
            /* LED is on. Turn it off. */
            lastLedVal = currentLedVal;
            LED_write(0);
          }else{
            /* LED is off. Turn it on. */
            if(lastLedVal > 0){
              LED_write(lastLedVal);
            }else{
              LED_write(LED_LEVEL_MAX);
            }
          }
        }else{
          /* Do nothing becouse we already changed LED level by long touch */ 
        }
      }
      touchFlag = false; 
    }
  }  
}

void MAIN_setStatusMsg(String msg){
  statusMessage = msg;
}

String MAIN_getStatusMsg(void){
  return statusMessage;
}

void setup(void) {
  /* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);   
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY); /* Use only tx, so rx can be used as GPIO */   
  //ESP.eraseConfig();
  pinMode(LEDPIN, OUTPUT);
  LED_write(0);  
  WIFIC_init();
  HTTP_init();
  WS_init();  
  UDPPING_init(); 
}


void loop(void) {  
  if(OTA_updateInProgress()){
    OTA_process();
  }else{
    HTTP_process();  
    WS_process();
    UDPPING_process();  
    WIFIC_process();     
    updateTouchSensor(); 
  }
}
