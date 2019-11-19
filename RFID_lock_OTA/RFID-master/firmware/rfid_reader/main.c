#include "main.h"
#include "RFID.h"
#include "spi.h"
#include "uart.h"
#include <string.h>


//Maximum length of the array
#define MAX_TAG_COUNT	(32)

#define DoorUnlock()	do{ PORTD |= (1 << DOOR_PIN); }while(0)
#define DoorLock()		do{ PORTD &= ~(1 << DOOR_PIN); }while(0)
#define LED_on()		do{ PORTB |= (1 << LED_PIN); }while(0)
#define LED_off()		do{ PORTB &= ~(1 << LED_PIN); }while(0)
#define LED_tgl()		do{ PORTB ^= (1 << LED_PIN); }while(0)

typedef enum{
	start,
	waitMasterId,
	program,
	normal
}state_t;

uint32_t EEMEM savedIds[MAX_TAG_COUNT];
uint32_t masterId;

void setup() {
	// Configure port directions.
	// Set the chipSelectPin as digital output, do not select the slave yet
	// Set the resetPowerDownPin as digital output, do not reset or power down.
	DIR_REG = (1<<USI_DATAOUT_PIN) | (1<<USI_CLOCK_PIN) | (1 << MFRC_CS) | (1 << MFRC_RESET);

	OUT_REG = (1<<USI_DATAIN_PIN)  // Pull-ups.
				| (1 << MFRC_CS);	// Slave release for now

	DDRB |= (1 << LED_PIN);
	// Configure USI to 3-wire master mode with overflow interrupt.

	USICR = (1<<USIOIE) | (1<<USIWM0) | (1<<USICS1) | (USICS0) | (1<<USICLK);

	/* Uart init */
	DDRD |= (1 << PD1) | (1 << DOOR_PIN);
	UBRRL = (unsigned char)BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
	UCSRB = (1<<TXEN) | (1<<RXEN) | (1 << RXCIE);

	RFID_Init();

	sei();

	masterId = eeprom_read_dword(&savedIds[0]);

	LED_on();
	DoorLock();
	wrIdx = 0;
	rdIdx = 0;

	_delay_ms(100);
}

/* Delays with the LED blinking.
 * Duration is in units of 100ms
 */
void LED_delay(uint8_t duration)
{
	uint8_t i;

	for(i = 0; i<duration; i++){
		LED_tgl();
		_delay_ms(100);
	}
}


/* Reads an RFID tag if present
 * returns 0 if none is present, 32-bit ID if one is read
 */
uint32_t readTagId( void )
{
	uint8_t calcChecksum;
	uint8_t status;
	uint8_t str[RFID_MSG_MAX_LEN];


	str[0] = 0;
	str[1] = 0x44;
	//Find tags, return tag type
	status = RFID_Request(str);
	//Anti-collision, return tag serial number 4 bytes
	status = RFID_Anticoll(str);

	calcChecksum = str[0] ^ str[1] ^ str[2] ^ str[3];

	uint32_t* cardId = (uint32_t*)str;

	if ((status != MI_OK) || (calcChecksum != str[4]))
	{
		*cardId = 0;
	}

	return *cardId;
}


/*
 * Checks if a tag is already saved.
 * returns true if found in EEPROM, false otherwise
 * If "add" is set, and it is not found, it will be saved
 */
bool checkTag(uint32_t ID, bool add)
{
	uint8_t i = 0;
	uint32_t temp;
	uint8_t emptySpot = 0;

	for(i = 0; i < MAX_TAG_COUNT; i++){
		temp = eeprom_read_dword(&savedIds[i]);

		if(temp == ID){
			/* Already saved */
			return true;

		}else if((temp == 0xFFFFFFFF) || (temp == 0)){
			/* found empty space, so remember this location */
			if(emptySpot == 0){
				emptySpot = i;
			}
		}
	}

	if(add){
		/* The tag was not found but should be saved */
		if(emptySpot != 0){
			/* An empty spot was found */
			eeprom_update_dword(&savedIds[emptySpot], ID);
		}/* else no more space to save a new tag */
	}

	return false;
}


void processMsg( void )
{
	uint8_t command, response;

	uart_readMsg(&command, 1);

	if(command == 'r'){
		/* respond to detected command */
		uart_print(&command, 1);

		/* start transmitting tags */
		uint8_t i;

		for(i = 0; i < MAX_TAG_COUNT; i ++){

			uint32_t tagId = eeprom_read_dword(&savedIds[i]);

			uart_print((uint8_t*)&tagId, 4);

			if(!uart_readMsg(&response, 1)){
				/* No response, so communication failed */
				break;
			}

			if(response != 1){
				/* Bad response */
				break;
			}
		}
	}else if(command == 'w'){
		/* respond to detected command */
		uart_print(&command, 1);

		/* start writing tags */
		uint8_t i;
		uint32_t msgBuffer;

		response = 1;
		for(i = 0; i < MAX_TAG_COUNT; i++){

			if(!uart_readMsg((uint8_t*)&msgBuffer, 4)){
				/* communication failed and tag id-s are not OK*/
				uart_queueClear();
				return;
			}

			eeprom_update_dword(&savedIds[i], msgBuffer);
			/* send acknowledge to confirm writing */
			uart_print(&response, 1);
		}
	}else{
		command = 0;
		uart_print(&command, 1);
		uart_queueClear();
	}
}


int main( void )
{
	uint32_t tagId;
	state_t state = start;

	setup();

	while(1){

		tagId = readTagId();

		switch(state){
			case start:
			{
				if(masterId == 0xFFFFFFFF){
					/* Not set */
					state = waitMasterId;
				}else{
					state = normal;
				}
				break;
			}

			case waitMasterId:
			{
				LED_tgl();
				if(tagId != 0){
					masterId = tagId;
					eeprom_update_dword(&savedIds[0], tagId);
					state = normal;
					LED_delay(30);	/* delay for 3 seconds so you have time to remove the master tag */
				}
				break;
			}

			case program:
			{
				LED_tgl();
				if(tagId != 0){
					checkTag(tagId, true);
					state = normal;
					LED_delay(30);
				}
				break;
			}


			default:
			{	/* normal state */
				LED_on();

				if(tagId != 0){
					if(tagId == masterId){
						state = program;
					}else if(checkTag(tagId, false)){
						DoorUnlock();
						LED_delay(50);	/* keep door unlocked for 5 seconds */
						DoorLock();
					}
				}

				if(uart_queueHasMsg()){
					processMsg();
				}

				break;
			}
		}

		_delay_ms(300);
	}
}

