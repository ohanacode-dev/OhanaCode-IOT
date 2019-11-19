#ifndef OTAU_H
#define OTAU_H

#define OTA_TIMEOUT                   (60000u)  /* This is how long the OTA server will be processed (in ms). After this timeout the device will work as usual. */

extern void OTA_init();
extern void OTA_process(void);
extern bool OTA_updateInProgress();

#endif
