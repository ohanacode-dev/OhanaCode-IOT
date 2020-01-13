#ifndef UDP_H
#define UDP_H

#include <stdbool.h>
#include <stdint.h>
#include <WiFiUdp.h>

extern void UDP_init(void);
extern void UDP_process(void);
extern bool UDP_getNtpMessage(uint8_t* ntp_buffer);
extern void UDP_sendNtpPackage(IPAddress ipAddr, uint8_t* data);

#endif
