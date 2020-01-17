#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "web_socket.h"
#include "config.h"
#include <EEPROM.h>
#include "ntp.h"
#include <Time.h>
#include "NTP_timer_weekly.h"

int timezone = 0;
String broadcasted = "";
static bool useDaylightSavings;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

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

void NTP_init()
{
  timeClient.begin();
  /* Read timezone settings from EEPROM */
  EEPROM.begin(EEPROM_SIZE);
  timezone = EEPROM.read(TIMEZONE_ADDR);
  int dlsVal = EEPROM.read(DAYSAVE_ADDR);
  EEPROM.end();

  useDaylightSavings = dlsVal > 0;

  if((useDaylightSavings > 0) && IsDayLightSaving()){
    timeClient.setTimeOffset((timezone + 1) * SECONDS_PER_HOUR);
  }else{
    timeClient.setTimeOffset(timezone * SECONDS_PER_HOUR);
  }
}

void NTP_process()
{
  timeClient.update();
  if(broadcasted != timeClient.getFormattedTime()){
    broadcasted = timeClient.getFormattedTime();

    unsigned long localTime = timeClient.getEpochTime();

    setTime(localTime);
  
    String timeStatus = "{ \"CURRENT\":\"" + broadcasted + "|" + String(daysOfTheWeek[timeClient.getDay()]) + 
    "," + String(day()) + "."  + String(month()) + "."  + String(year()) +
    "\", \"DLSAVE\":\"" + String(useDaylightSavings) + 
    "\", \"ACTIVE\":\"";

    if( MAIN_getActivated()){
      timeStatus += "1";
    }else{
      timeStatus += "0";
    }

    timeStatus += "\"}";
    
    WS_ServerBroadcast(timeStatus); 
  } 
}

String NTP_getTimeString(){
  return timeClient.getFormattedTime();
}

void NTP_setTimeZone(int zone){
  if(zone > 13){
    zone = 13;
  }else if(zone < -12){
    zone = -12;
  }
  
  timezone = zone;
  timeClient.setTimeOffset(timezone * SECONDS_PER_HOUR);

  /* Save to EEPROM */
  EEPROM.begin(EEPROM_SIZE);  
  EEPROM.write(TIMEZONE_ADDR, timezone); 
  EEPROM.commit();
  EEPROM.end();
}

int NTP_getTimeZone(){
  return timezone;
}

void NTP_setDayLightSavings(String dls)
{ 
  /* Adjust time zone */
  int dlsVal = 0;
  useDaylightSavings = dls.length() > 0;

  if(useDaylightSavings){
    dlsVal = 1;
  }

  if(useDaylightSavings && IsDayLightSaving()){
    timeClient.setTimeOffset((timezone + 1) * SECONDS_PER_HOUR);
  }else{
    timeClient.setTimeOffset(timezone * SECONDS_PER_HOUR);
  }

  /* Save to EEPROM */
  EEPROM.begin(EEPROM_SIZE);  
  EEPROM.write(DAYSAVE_ADDR, dlsVal); 
  EEPROM.commit();
  EEPROM.end();

}

String NTP_getDayLightSavings(void)
{
  if(useDaylightSavings){
    return "1";
  }
  return "0";
}
