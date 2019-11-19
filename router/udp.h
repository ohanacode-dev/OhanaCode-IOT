#ifndef UDP_H
#define UDP_H

extern void UDP_init(void);
extern void UDP_process(void);
extern void UDP_pingDevices(void);
extern void UDP_devCountClear(void);
extern String UDP_devListToJson(void);
extern String UDP_getDeviceIP(uint8_t devId);
extern int UDP_getDevListId(String deviceMAC);

#endif
