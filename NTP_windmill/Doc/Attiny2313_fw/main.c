/*
 * File:   main.c
 * Author: ujagaga@gmail.com
 *
 * Created on 12/23/2020 7:07:54 PM UTC
 * "Created in MPLAB Xpress"
 * After you compile this in MPLABX for Attiny2313, you will get a hex file, like clk_7_seg_display.hex
 * attached in this folder. It can be flashed to MCU using avrdude command:
 *         avrdude -c usbtiny -p attiny2313 -U flash:w:clk_7_seg_display.hex
 */
 
#define F_CPU   1000000ul
#define BAUD 	        (300)
#define BAUD_PRESCALE 	((F_CPU / (BAUD * 16L)) - 1)
#define NUM_OFF()       do{ PORTD &= ~((1 << PD6) | (1 << PD4));}while(0)
#define NUM_0()		    do{ PORTD |= (1 << PD6);}while(0)
#define NUM_1()		    do{ PORTD |= (1 << PD4);}while(0)

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>


uint8_t num;

/* 0b abcdefg0  */
uint8_t out_num[] = {
    0b00000011,     //0
    0b10011111,     //1
    0b00100101,     //2
    0b00001101,     //3
    0b10011001,     //4
    0b01001001,     //5
    0b01000001,     //6    
    0b00011111,     //7
    0b00000001,     //8
    0b00001001,     //9
    0b11111101      //-
};

void send_uart(uint8_t c) {
  // wait for empty data register
  while (!(UCSRA & (1<<UDRE)));
  // set data into data register
  UDR = c;
}

ISR(USART_RX_vect) {
	num = UDR;
	send_uart(num);
}

void setup() {	
	/* Uart init */
	UBRRL = (unsigned char)BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
	UCSRB = (1<<RXEN) | (1 << TXEN) | (1 << RXCIE);
	
	// Setup pin direction
	DDRB = 0xff;
	DDRD |= (1 << PD6) | (1 << PD4) | (1 << PD1);
	
	sei();
}

int main(){ 
    
    setup();    
    
    uint8_t seg_id = 0;
    uint8_t val;
    
    while(1){    
        _delay_ms(4); 
        
        NUM_OFF();
        
        seg_id++;   // switch displayed digit
        
        if(seg_id & 1){    
            val = num / 10;          
        }else{
            val = num % 10;
        }   
        
        if(val < 10){
            PORTB = out_num[val];   
        }else{
            PORTB = out_num[10];   
        }        
        
        if(seg_id & 1){
            NUM_1();  
        }else{            
            NUM_0();
        }                
                
    }
    return 0;   
}