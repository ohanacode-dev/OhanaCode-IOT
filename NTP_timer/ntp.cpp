#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include "config.h"
#include "udp.h"
#include "ntp.h"
#include "web_socket.h"
#include <Time.h>

int timeZone;
static unsigned long lastSyncTime;
static uint8_t NTP_TxBuffer[ NTP_PACKET_SIZE]; //buffer to hold outgoing packets
static uint8_t NTP_RxBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming packets
static unsigned long ntp_localTimestamp;
static unsigned long localTime;
static String broadcasted;
static bool timeValidFlag;
static int useDaylightSavings;

static void sendNtpPackage(void)
{
  /* Don't hardwire the IP address or we won't get the benefits of the pool.
   *  Lookup the IP address for the host name instead */
  IPAddress timeServerIP; 
  
  /* get a random server IP from the pool */
  WiFi.hostByName(NTP_SERVER_NAME, timeServerIP); 
  
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  UDP_sendNtpPackage(timeServerIP, NTP_TxBuffer); 
  
  lastSyncTime = millis();
}

void NTP_init(void)
{
  /* set all bytes in the buffer to 0 */
  memset(NTP_TxBuffer, 0, NTP_PACKET_SIZE);
  /* Initialize values needed to form NTP request (see URL above for details on the packets) */
  NTP_TxBuffer[0] = 0b11100011;   // LI, Version, Mode
  NTP_TxBuffer[1] = 0;     // Stratum, or type of clock
  NTP_TxBuffer[2] = 6;     // Polling Interval
  NTP_TxBuffer[3] = 0xEC;  // Peer Clock Precision
  /* 8 bytes of zero for Root Delay & Root Dispersion */
  NTP_TxBuffer[12]  = 49;
  NTP_TxBuffer[13]  = 0x4E;
  NTP_TxBuffer[14]  = 49;
  NTP_TxBuffer[15]  = 52;

  ntp_localTimestamp = 0;
  broadcasted = "";

   /* Read timezone settings from EEPROM */
  EEPROM.begin(EEPROM_SIZE);
  timeZone = EEPROM.read(TIMEZONE_ADDR);
  useDaylightSavings = EEPROM.read(DAYSAVE_ADDR);
  EEPROM.end();
 
  localTime = 0;
  timeValidFlag = false;

  sendNtpPackage();
}

void NTP_process(void)
{
  if((millis() - lastSyncTime) > NTP_SYNC_INTERVAL){
    sendNtpPackage();
  }
  
  if (UDP_getNtpMessage(NTP_RxBuffer)) {   
    /* the timestamp starts at byte 40 of the received packet and is four bytes, or two words, long. 
     * First, esxtract the two words: */
    ntp_localTimestamp = millis();
    
    unsigned long highWord = word(NTP_RxBuffer[40], NTP_RxBuffer[41]);
    unsigned long lowWord = word(NTP_RxBuffer[42], NTP_RxBuffer[43]);
    /* combine the four bytes (two words) into a long integer. This is NTP time. */
    unsigned long secsSince1900 = highWord << 16 | lowWord;
   
    /* now convert NTP time into UNIX time by substracting 70 years: */       
    unsigned long epoch = secsSince1900 - UNIX_UTC_DIFF;

    localTime = epoch + (timeZone * SECONDS_PER_HOUR);

    setTime(localTime);
    
    timeValidFlag = true;   
  }

  if(broadcasted != NTP_getTimeString()){
    broadcasted = NTP_getTimeString();
    String timeStatus = "{\"CURRENT\":\"" + broadcasted + "\"}";  
    WS_ServerBroadcast(timeStatus); 
  }  
}

static bool IsDayLightSaving(void)
{
    //January, february, and december are out.
    if (month() < 3 || month() > 11) { 
      return false; 
    }
    //April to October are in
    if (month() > 3 && month() < 11) { 
      return true; 
    }
    int previousSunday = day() - weekday();
    //In march, we are DST if our previous sunday was on or after the 8th.
    if (month() == 3) { 
      return previousSunday >= 8; 
    }
    //In november we must be before the first sunday to be dst.
    //That means the previous sunday must be before the 1st.
    return previousSunday <= 0;
}

bool NTP_getTime(ntp_time_t* result)
{
  if(timeValidFlag){  
    result->h = hour();

    if((useDaylightSavings > 0) && IsDayLightSaving()){
      result->h ++;
      if(result->h > 23){
        result->h -= 24;
      }
    }
    
    result->m = minute();
    result->s = second();
  }

  return timeValidFlag;
}

String NTP_getTimeString( void )
{
  String retVal = "";
  
  ntp_time_t current;
  if(NTP_getTime(&current)){
    String H = String(current.h);
    if(H.length() == 1){
      H = "0" + H;
    }
    String M = String(current.m);
    if(M.length() == 1){
      M = "0" + M;
    }
    String S = String(current.s);
    if(S.length() == 1){
      S = "0" + S;
    }
    retVal = H + ":" + M + ":" + S;
  }

  return retVal;
}

void NTP_setTimeZone(int zone)
{ 
  /* Adjust time zone */
  timeZone = zone;

  /* Save to EEPROM */
  EEPROM.begin(EEPROM_SIZE);  
  EEPROM.write(TIMEZONE_ADDR, timeZone); 
  EEPROM.commit();
  EEPROM.end();
  
  /* Force time update */
  sendNtpPackage();

  timeValidFlag = false;

}

int NTP_getTimeZone(void)
{
  return timeZone;
}

void NTP_setDayLightSavings(int dls)
{ 
  /* Adjust time zone */
  useDaylightSavings = dls;

  /* Save to EEPROM */
  EEPROM.begin(EEPROM_SIZE);  
  EEPROM.write(DAYSAVE_ADDR, useDaylightSavings); 
  EEPROM.commit();
  EEPROM.end();
  
  /* Force time update */
  sendNtpPackage();  

}

int NTP_getDayLightSavings(void)
{
  return useDaylightSavings;
}




