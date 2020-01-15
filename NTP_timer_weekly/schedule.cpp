#include "config.h"
#include <EEPROM.h>

static uint8_t schedule[SCHEDULE_SIZE] = {0};
static bool scheduleChangedFlag = false;

void SCH_init(){
  
  EEPROM.begin(EEPROM_SIZE);
  
  for(int i = 0; i < SCHEDULE_SIZE; i++){
    schedule[i] = EEPROM.read(SCHEDULE_ADDR + i);
    if(schedule[i] > 24){
      schedule[i] = 24;
    }
  }  
  
  EEPROM.end();
}

void SCH_update(uint8_t* source){
  for(int i = 0; i < SCHEDULE_SIZE; i++){
    schedule[i] = source[i];
  } 
  scheduleChangedFlag = true;
}

void SCH_get(uint8_t* dst){
  for(int i = 0; i < SCHEDULE_SIZE; i++){
    dst[i] = schedule[i];
  }  
}

void SCH_process(){
  if(scheduleChangedFlag){
    EEPROM.begin(EEPROM_SIZE);
  
    for(int i = 0; i < SCHEDULE_SIZE; i++){
      EEPROM.write(SCHEDULE_ADDR + i, schedule[i]); 
    }  
    EEPROM.commit();
    EEPROM.end();
  }

  scheduleChangedFlag = false;
}
