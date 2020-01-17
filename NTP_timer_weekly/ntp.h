#ifndef MYNTP_H
#define MYNTP_H

/* NTP time is seconds since Jan 1 1900. Unix time starts on Jan 1 1970.
 Those 70 years difference in seconds is 2208988800 */
#define UNIX_UTC_DIFF       (2208988800UL)
#define SECONDS_PER_DAY     (86400UL)
#define SECONDS_PER_HOUR    (3600UL)
#define SECONDS_PER_MINUTE  (60UL)
#define MINUTES_PER_HOUR    (60UL)
#define MINUTES_PER_12H     (12*60UL)
#define ERROR_CORRECTION    (11 * SECONDS_PER_DAY) /* An error due to an unknown reason. Probably faulty NTP client */

extern void NTP_init();
extern void NTP_process();
extern String NTP_getTimeString(); 
extern int NTP_getTimeZone();
extern void NTP_setTimeZone(int zone);
extern void NTP_setDayLightSavings(String dls);
extern String NTP_getDayLightSavings(void);

#endif
