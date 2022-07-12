/*
 * DS18B20.c
 *
 *  Created on: jan 15, 2017
 *      Author: ujagaga
 */

#include "DS18B20.h"

#define SKIP_ROM_MSG 		    (0xCC)
#define CONVERT_T_MSG		    (0x44)
#define READ_SCRATCHPAD_MSG (0xBE)

unsigned int DSpin;
typedef enum{
  state_init = 0, 
  state_init_conversion,
  state_wait_for_conversion,  
  state_read_temp,
  state_finish
}state_t;

state_t ds_state = state_init;
int lastRead = DS_ERR;
unsigned long lastStateTime = 0;

/* Find the probe to see if it is operational */
bool checkPresence( void )
{
	bool isPresent;

	digitalWrite(DSpin, LOW);  
	pinMode(DSpin, OUTPUT);
	delayMicroseconds(480);
	
	pinMode(DSpin, INPUT);
	delayMicroseconds(64);

	isPresent = digitalRead(DSpin);
	
	delayMicroseconds(480);

	return !isPresent;
}

void writeBit( bool value )
{	
		digitalWrite(DSpin, LOW);  
		pinMode(DSpin, OUTPUT);

		delayMicroseconds(2);

		if(value){
			pinMode(DSpin, INPUT);
			delayMicroseconds(60);
		}else{
			delayMicroseconds(60);
			pinMode(DSpin, INPUT);
		}
}

void sendByte(uint8_t data)
{
	uint8_t i;

	for(i = 0; i < 8; i++){
		writeBit((((data >> i) & 1) == 1));
	}

}

bool readBit( void )
{
	bool result;
	
	digitalWrite(DSpin, LOW);  
	pinMode(DSpin, OUTPUT);

	delayMicroseconds(2);
	pinMode(DSpin, INPUT);
	delayMicroseconds(10);
	result = digitalRead(DSpin);
	

	delayMicroseconds(60-10);
	return result;
}

uint8_t readByte( void )
{
	uint8_t i, result = 0;

	for(i = 0; i < 8; i++)
	{
		if(readBit())
		{
			result |= 1 << i;
		}
	}

	return result;
}

/* State machine based probe read routine to prevent blocking for longer periods. 
*   reads the probe and returns raw integer. Divide it by 16 to get temperature in celsius.
*/
int DS_readTemp( unsigned int pin )
{   
  switch(ds_state){
    case state_init:
    {
      DSpin = pin;
      
      if(lastStateTime == 0){
        lastStateTime = millis();
      }else{
        /* Wait for 100ms */
        if((millis() - lastStateTime) > 100){   
          lastStateTime = 0;         
          ds_state = state_init_conversion;   
        }              
      }      
      break;
    }

    case state_init_conversion:
    {
      if(!checkPresence())
      {
        lastRead = DS_ERR;
        ds_state = state_finish;
      }else{
        sendByte(SKIP_ROM_MSG);
        sendByte(CONVERT_T_MSG);
      
        /* Extra power for temp conversion */
        digitalWrite(DSpin, HIGH);  
        pinMode(DSpin, OUTPUT);
        
        ds_state = state_wait_for_conversion;   
      }
      break;
    }

    case state_wait_for_conversion:
    {
      if(lastStateTime == 0){
        lastStateTime = millis();
      }else{
        /* Wait for 1500ms */
        if((millis() - lastStateTime) > 1500){   
          lastStateTime = 0;           
          ds_state = state_read_temp;   
        }              
      }   
      break;
    }

    case state_read_temp:
    {
      pinMode(DSpin, INPUT);

      if(!checkPresence())
      {
        lastRead = DS_ERR;        
      }else{      
        sendByte(SKIP_ROM_MSG);
        sendByte(READ_SCRATCHPAD_MSG);
      
        int result = readByte();
      
        result |= (readByte() << 8);
        result = (result & 0x7FF);
      
        if(result > DS_MAX){
          lastRead = DS_ERR;
        }else{
          lastRead = result;
        }
      }

      ds_state = state_finish;
      break;
    }

    case state_finish:
    {
      if(lastStateTime == 0){
        lastStateTime = millis();
      }else{
        /* Wait for another 1000ms to try again */
        if((millis() - lastStateTime) > 1000){   
          lastStateTime = 0;           
          ds_state = state_init;   
        }              
      }   
      break;
    }

    default:
      ds_state = state_init;
  }
  
  return lastRead;
}






