RFID reader project for AtTiny2313 using MIFARE RFID-RC522 spi module

The RFID library is based on Addicore arduino library. I re-wrote it as pure C library and removed 
everything not esential to bare reading of an RFID TAG id to reduce the code size.

The initial commit contains a basic example of reading a tag and outputting the 4 bytes of id to UART.
The latest commit contains:
- full RFID example which can record up to 32 tag ids and unlock an electromagnetic lock
- EEPROM reader device which can be used to record the RFID readers memory and write it back on command
so that re-distributing codes between multiple locks can be simpified
- Linux and Windows console applications for administrating tag ids using either the EEPROM copy device 
or the RFID reader directly.

more details will be in the doc folder.  