#include <stdint.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "stepper_motor_driver.h"
#include "ntp.h"


#define TICK_TIMEOUT      (100)

typedef enum{
  state_init_min,
  state_init_hour,
  state_get_time,
  state_check_min,
  state_check_hours,
  state_tick_min,
  state_tick_hour,
  state_wait_min_tick_end,
  state_wait_hour_tick_end
}state_t;

static state_t stmdrvState;
static uint16_t minTickCount;
static uint16_t hourTickCount;
static uint16_t targetTickCount;
static unsigned long lastTickTime;
static bool minHandZeroPosSet;
static bool hourHandZeroPosSet;
static ntp_time_t currentTime;   

static void M_M_tickStop(void){
  digitalWrite(M_M_PIN1, LOW);
  digitalWrite(M_M_PIN2, LOW);
  digitalWrite(M_M_PIN3, LOW);
  digitalWrite(M_M_PIN4, LOW);
}

static void M_H_tickStop(void){
  digitalWrite(M_H_PIN1, LOW);
  digitalWrite(M_H_PIN2, LOW);
  digitalWrite(M_H_PIN3, LOW);
  digitalWrite(M_H_PIN4, LOW);
}


static void M_M_tick(void){
  switch(minTickCount & 0b11){
    case 0:
      digitalWrite(M_M_PIN1, HIGH);
      digitalWrite(M_M_PIN2, LOW);
      digitalWrite(M_M_PIN3, LOW);
      digitalWrite(M_M_PIN4, LOW);
      break;
     case 1:
      digitalWrite(M_M_PIN1, LOW);
      digitalWrite(M_M_PIN2, HIGH);
      digitalWrite(M_M_PIN3, LOW);
      digitalWrite(M_M_PIN4, LOW);
      break;
     case 2:
      digitalWrite(M_M_PIN1, LOW);
      digitalWrite(M_M_PIN2, LOW);
      digitalWrite(M_M_PIN3, HIGH);
      digitalWrite(M_M_PIN4, LOW);
      break;
     case 3:
      digitalWrite(M_M_PIN1, LOW);
      digitalWrite(M_M_PIN2, LOW);
      digitalWrite(M_M_PIN3, LOW);
      digitalWrite(M_M_PIN4, HIGH);
      break;
  } 

  minTickCount++; 
  if(minTickCount == M_M_STEPS){
    minTickCount = 0;
  }
  
  lastTickTime = millis();
}


static void M_H_tick(void){
  switch(hourTickCount & 0b11){
    case 0:
      digitalWrite(M_H_PIN1, HIGH);
      digitalWrite(M_H_PIN2, LOW);
      digitalWrite(M_H_PIN3, LOW);
      digitalWrite(M_H_PIN4, LOW);
      break;
     case 1:
      digitalWrite(M_H_PIN1, LOW);
      digitalWrite(M_H_PIN2, HIGH);
      digitalWrite(M_H_PIN3, LOW);
      digitalWrite(M_H_PIN4, LOW);
      break;
     case 2:
      digitalWrite(M_H_PIN1, LOW);
      digitalWrite(M_H_PIN2, LOW);
      digitalWrite(M_H_PIN3, HIGH);
      digitalWrite(M_H_PIN4, LOW);
      break;
     case 3:
      digitalWrite(M_H_PIN1, LOW);
      digitalWrite(M_H_PIN2, LOW);
      digitalWrite(M_H_PIN3, LOW);
      digitalWrite(M_H_PIN4, HIGH);
      break;
  }
 
  hourTickCount++; 
  if(hourTickCount == M_H_STEPS){
    hourTickCount = 0;
  }
  lastTickTime = millis();
}

void STMDRV_process(void)
{
  /* Minute hand state machine */
  switch(stmdrvState){
    case state_init_min:
    {
      /* Turn the minute hand to full hour position. */
      
      if(!minHandZeroPosSet && (minTickCount < (M_M_STEPS - 1))){
        if((millis() - lastTickTime) > M_M_TICK_TIMEOUT){        
          M_M_tick();     
          // TODO: implement full hour position detection to set minHandZeroPosSet to true and minTickCount to 0                           
        }         
      }else{
        minHandZeroPosSet = true;
        M_M_tickStop();
        minTickCount = 0;        
        stmdrvState = state_init_hour;
      }     

    }break;

    case state_init_hour:
    {
      /* Turn the hour hand to 12 o clock position. */
      if(!hourHandZeroPosSet && (hourTickCount < (M_H_STEPS - 1))){
        if((millis() - lastTickTime) > M_H_TICK_TIMEOUT){        
          M_H_tick();  
          // TODO: implement zero hour position detection to set hourHandZeroPosSet to true and minHourCount to 0                              
        }         
      }else{
        hourHandZeroPosSet = true;
        M_H_tickStop();
        hourTickCount = 0;        
        stmdrvState = state_check_min;
      }     
      
    }break;

    case state_get_time:
    {
      if(NTP_getTime(&currentTime)){
        if(currentTime.h > 11){
          currentTime.h -= 12;
        }
        
        stmdrvState = state_check_min;
      }      
    }break;
    
    case state_check_min:
    {        
      uint16_t totalSeconds = (currentTime.m * SECONDS_PER_MINUTE) + currentTime.s;
      /* Calculating minute hand position with 1s precision
       * M_M_STEPS/targetTickCount = SECONDS_PER_HOUR/totalSeconds */
      targetTickCount = (M_M_STEPS * totalSeconds) / SECONDS_PER_HOUR;      

      if(targetTickCount != minTickCount){
        stmdrvState = state_tick_min;
      }else{
        stmdrvState = state_check_hours;
      }
    }break;
    
    case state_check_hours:
    {
      
      uint32_t totalMinutes = (currentTime.h * MINUTES_PER_HOUR) + currentTime.m;
    
      /* Calculating hour hand position with 1m precision
       * M_H_STEPS/targetTickCount = MINUTES_PER_12H/totalMinutes */
      targetTickCount = (M_H_STEPS * totalMinutes) / MINUTES_PER_12H;

      if(targetTickCount != hourTickCount){
        stmdrvState = state_tick_hour;

//        Serial.print(" H:");
//        Serial.print(currentTime.h);
//        Serial.print(" M:");
//        Serial.print(currentTime.m);
//        Serial.print(" S:");
//        Serial.println(currentTime.s);
//        Serial.print("Hour targetTickCount:");
//        Serial.println(targetTickCount);
//        Serial.print("hourTickCount:");
//        Serial.println(hourTickCount);
      }else{
        stmdrvState = state_get_time;
      }
     
    }break;

    case state_tick_min:
    {
      if((millis() - lastTickTime) > M_M_TICK_TIMEOUT){  
        M_M_tick();  
        stmdrvState = state_wait_min_tick_end;      
      }
    }break;

    case state_wait_min_tick_end:
    {
      if((millis() - lastTickTime) > M_M_TICK_TIMEOUT){        
        M_M_tickStop();
        stmdrvState = state_check_min;         
      }      
    }break;

    case state_tick_hour:
    {
      if((millis() - lastTickTime) > M_H_TICK_TIMEOUT){  
        M_H_tick();  
        stmdrvState = state_wait_hour_tick_end;      
      }
    }break;
    
    case state_wait_hour_tick_end:
    {
      if((millis() - lastTickTime) > M_H_TICK_TIMEOUT){        
        M_H_tickStop();
        stmdrvState = state_check_hours;         
      }      
    }break;
    
    default:
      stmdrvState = state_init_min;
      break;
  }
}

void STMDRV_init(void){
  pinMode(M_M_PIN1, OUTPUT);
  pinMode(M_M_PIN2, OUTPUT);
  pinMode(M_M_PIN3, OUTPUT);
  pinMode(M_M_PIN4, OUTPUT);

  pinMode(M_H_PIN1, OUTPUT);
  pinMode(M_H_PIN2, OUTPUT);
  pinMode(M_H_PIN3, OUTPUT);
  pinMode(M_H_PIN4, OUTPUT);

  M_M_tickStop();
  M_H_tickStop();
  minTickCount = 0;
  hourTickCount = 0;
  lastTickTime = 0;
  stmdrvState = state_init_min;
  minHandZeroPosSet = false;
  hourHandZeroPosSet = false;
 
}


