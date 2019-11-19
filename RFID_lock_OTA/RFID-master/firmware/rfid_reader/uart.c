/*
 * uart.c
 *
 *  Created on: Dec 30, 2016
 *      Author: rada
 */

#include "main.h"
#include "uart.h"


#define RX_SIZE 		(16)
#define IDX_MASK		(RX_SIZE - 1)

uint8_t rxQueue[RX_SIZE];
volatile uint8_t wrIdx, rdIdx;

#define getRxCount()		((wrIdx - rdIdx) & IDX_MASK)
#define queueIsNotFull()	(getRxCount() < RX_SIZE)


ISR(USART_RX_vect) {
	uint8_t temp = UDR;

	if(queueIsNotFull()){
		rxQueue[wrIdx & IDX_MASK] = temp;
		wrIdx++;
	}
}


void uart_print(uint8_t* data, uint8_t len)
{
	uint8_t i = 0;

	if(len > 32){
		len = 32;
	}

	while(i < len){
		uart_printByte(data[i]);
		i++;
	}
}

bool uart_readMsg( uint8_t* data, uint8_t len)
{
	uint8_t i = 0;
	volatile uint16_t timeout = 500;

	while(getRxCount() < len){
		_delay_ms(1);
		timeout--;
		if(timeout == 0){
			/* communication failed */
			return false;
		}
	}

	for(i = 0; i < len; i++){
		data[i] = rxQueue[rdIdx & IDX_MASK];
		rdIdx++;
	}

	return true;
}
