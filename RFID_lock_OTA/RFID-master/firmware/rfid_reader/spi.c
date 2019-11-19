/*
 * spi.c
 *
 *  Created on: Dec 31, 2016
 *      Author: ujagaga
 */

#include "main.h"
#include "spi.h"

uint8_t spi(uint8_t val)
{
    USIDR = val;
    USISR = (1<<USIOIF);
    do {
        USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
    } while ((USISR & (1<<USIOIF)) == 0);
    return USIDR;
}

void spi_enable(bool enable)
{
    if (enable) {
    	OUT_REG |= (1<<USI_DATAIN_PIN)|(1<<MFRC_CS);       // pull-up i.e. disabled
        DIR_REG |= (1<<USI_CLOCK_PIN)|(1<<USI_DATAOUT_PIN)|(1<<MFRC_CS);
        USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK);
        OUT_REG &= ~(1<<MFRC_CS);    // enable Slave
    } else {
    	OUT_REG |= (1<<MFRC_CS);     // pull-up i.e. disabled
        DIR_REG &= ~((1<<USI_CLOCK_PIN)|(1<<USI_DATAIN_PIN)|(1<<USI_DATAOUT_PIN)|(1<<MFRC_CS));
        USICR = 0;
    }
}
