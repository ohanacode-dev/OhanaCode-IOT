/*
 * spi.h
 *
 *  Created on: Dec 31, 2016
 *      Author: ujagaga
 */

#ifndef SPI_H_
#define SPI_H_

/* USI port and pin definitions.
 */
#define OUT_REG			PORTB	//USI port output register.
#define IN_REG			PINB	//USI port input register.
#define DIR_REG			DDRB	//USI port direction register.
#define USI_CLOCK_PIN	PB7		//USI clock I/O pin.		(SCK)
#define USI_DATAOUT_PIN	PB6		//USI data output pin. 		(MOSI)
#define USI_DATAIN_PIN	PB5		//USI data input pin. 		(MISO

#define MFRC_CS 		PB4		//							(SDA)
#define MFRC_RESET 		PB2		//							(RST)


void spi_enable( bool enable );
uint8_t spi( uint8_t val );


#endif /* SPI_H_ */
