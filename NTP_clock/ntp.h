#ifndef NTP_H
#define NTP_H

/* NTP time is seconds since Jan 1 1900. Unix time starts on Jan 1 1970.
 Those 70 years difference in seconds is 2208988800 */
#define UNIX_UTC_DIFF       (2208988800UL)
#define SECONDS_PER_DAY     (86400UL)
#define SECONDS_PER_HOUR    (3600UL)
#define SECONDS_PER_MINUTE  (60UL)
#define MINUTES_PER_HOUR    (60UL)
#define MINUTES_PER_12H     (12*60UL)

typedef struct{
  uint8_t h;
  uint8_t m;
  uint8_t s;
}ntp_time_t;

extern void NTP_init(void);
extern void NTP_process(void);
extern bool NTP_getTime(ntp_time_t* result);
extern String NTP_getTimeString( void );
extern void NTP_setTimeZone(int zone);
extern int NTP_getTimeZone(void);
extern void NTP_setDayLightSavings(int dls);
extern int NTP_getDayLightSavings(void);

#endif
