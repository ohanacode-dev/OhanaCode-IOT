#include "config.h"
#include "pinctrl.h"

//#define USE_TOUCHSENSOR

static unsigned long userRequestTimestamp = 0;      

/* Processes the touch detection. The sensor is treated as a variable capacitor. 
 * When the user touches it, the capacity increases. We are measuring the charge time 
 * to determine if the capacity is higher than usual. If you are using a small conductive 
 * surface as a sensor (tipically up to 2 cm in diameter), if it is not touched, 
 * it should not take longer than 1ms to charge to a HIGH level.
 */
static bool touchDetected(){ 
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
    
  return (chargeTime > SENSOR_IDLE_CHARGE_MIN_TIME);
}

static bool buttonPushed(){
  pinMode(SENSPIN, INPUT_PULLUP);
  return (digitalRead(SENSPIN) == LOW);
}


void PUSHBUTTON_process(){

#ifdef USE_TOUCHSENSOR
  bool userRequestDetected = touchDetected();
#else
  bool userRequestDetected = buttonPushed();
#endif

  if(userRequestDetected){
    if((millis() - userRequestTimestamp) > DEBOUNCE_TIMEOUT){
      if(PINCTRL_getCurrentVal() == 0){
        PINCTRL_write(100);
      }else{
        PINCTRL_write(0);
      }      
    }
    
    userRequestTimestamp = millis();
  }
}
