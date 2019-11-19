#ifndef UDP_SCAN_H
#define UDP_SCAN_H

extern void SCAN_init(void);
extern void SCAN_process(void);
extern String SCAN_getDeviceIP(uint8_t devId);
extern int SCAN_getDevListId(String deviceMAC);

#endif
