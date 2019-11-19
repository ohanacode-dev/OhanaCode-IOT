/*
 * DS18B20.h
 *
 *  Created on: Oct 23, 2016
 *      Author: ujagaga
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include <inttypes.h>
#include "Arduino.h"

#define DS_MAX		(84 * 16)
#define DS_ERR		(0xFFFF)

#ifdef __cplusplus
extern "C" {
#endif

int DS_readTemp( unsigned int pin );

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* DS18B20_H_ */
