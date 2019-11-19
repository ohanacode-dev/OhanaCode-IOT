#ifndef UDP_SCAN_H
#define UDP_SCAN_H

extern void UDPSCAN_init(void);
extern void UDPSCAN_process(void);
extern void UDPSCAN_pingDevices(void);
extern String UDPSCAN_getDeviceIP(uint8_t devId);
extern int UDPSCAN_getDevListId(String deviceMAC);

#endif
