#include "main.h"
#include "uart.h"

#define LED_grnOn()			do{PORTB |= (1 << LED_GRN_PIN);}while(0)
#define LED_grnOff()		do{PORTB &= ~(1 << LED_GRN_PIN);}while(0)
#define LED_redOn()			do{PORTB |= (1 << LED_RED_PIN);}while(0)
#define LED_redOff()		do{PORTB &= ~(1 << LED_RED_PIN);}while(0)
#define BtnReadPressed()	((PINA & (1 << BTN_READ_PIN)) == 0)
#define BtnWritePressed()	((PINA & (1 << BTN_WRITE_PIN)) == 0)

#define MAX_TAG_COUNT	(32)
uint32_t EEMEM savedIds[MAX_TAG_COUNT];


void setup() {

	/* set LED pins as output */
	DDRB = (1 << LED_RED_PIN) | (1 << LED_GRN_PIN);

	DDRA = 0;

	/* activate pushbutton pins pull-up resistors */
	PORTA = (1 << BTN_READ_PIN) | (1 << BTN_WRITE_PIN);

	/* Uart init */
	DDRD |= (1 << PD1);
	UBRRL = (unsigned char)BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
	UCSRB = (1<<TXEN) | (1<<RXEN) | (1 << RXCIE);

	sei();

	wrIdx = 0;
	rdIdx = 0;
}

/* Reads RFID readers saved tags.
 * if verifyOnly flag is set, then it compares with EEPROM content without writing.
 * */
bool readRfidMem( bool verifyOnly ){
	uint8_t command = 'r', response;
	uint8_t i;
	uint32_t tagId;

	/* clear queue in case of garbage due to previous unsuccessful operation */
	uart_queueClear();

	/* Send read command */
	uart_print(&command, 1);

	response = 0;
	/* wait for response */
	if(!uart_readMsg(&response, 1)){
		/* communication failed */
		return false;
	}

	if(response != 'r'){
		uart_print(&response, 1);
		return false;
	}

	response = 1;
	/* wait for data for 32 tags and write them to EEPROM */
	for(i = 0; i < MAX_TAG_COUNT; i++){

		if(!uart_readMsg((uint8_t*)&tagId, 4)){
			/* communication failed */
			return false;
		}

		if(verifyOnly){
			/* verify written tags */
			uint32_t storedId = eeprom_read_dword(&savedIds[i]);
			if(storedId != tagId){
				return false;
			}
		}else{
			eeprom_update_dword(&savedIds[i], tagId);
		}

		/* Send acknowledge response  */
		uart_print(&response, 1);
	}

	return true;

}

bool writeRfidMem( void ){
	uint8_t command = 'w', response;
	uint8_t i;
	uint32_t tagId;

	/* clear queue in case of garbage due to previous unsuccessful operation */
	uart_queueClear();

	uart_print(&command, 1);

	/* wait for response */
	if(!uart_readMsg(&response, 1)){
		/* communication failed */
		return false;
	}

	if(response != 'w'){
		return false;
	}

	/* send data */
	for(i = 0; i < MAX_TAG_COUNT; i++){
		tagId = eeprom_read_dword(&savedIds[i]);

		uart_print((uint8_t*)&tagId, 4);

		/* wait for acknowledge */
		if(!uart_readMsg(&response, 1)){
			/* communication failed */
			return false;
		}

		if(response != 1){
			/* Bad response */
			return false;
		}
	}

	return true;

}

int main( void )
{
	setup();

	/* Just a signal to the user that
	 * everything is ready and power is on*/
	LED_grnOn();
	_delay_ms(300);
	LED_redOn();
	_delay_ms(300);
	LED_grnOff();
	_delay_ms(300);
	LED_redOff();


	while(1){

		if(BtnReadPressed() || BtnWritePressed()){
			_delay_ms(100);	/* de-bounce */

			if(BtnReadPressed()){

				LED_grnOn();
				LED_redOn();

				/* wait for the user to release both pushbuttons to prevent repeated operation*/
				while(BtnReadPressed());

				if(readRfidMem(false) && readRfidMem(true)){
					LED_redOff();
					LED_grnOn();
				}else{
					LED_grnOff();
					LED_redOn();
				}
			}else if(BtnWritePressed()){

				LED_grnOn();
				LED_redOn();

				/* wait for the user to release both pushbuttons to prevent repeated operation*/
				while(BtnWritePressed());

				if(writeRfidMem() && readRfidMem(true)){
					LED_redOff();
				}else{
					LED_grnOff();
				}
			}

			_delay_ms(1000);
		}
	}
}

