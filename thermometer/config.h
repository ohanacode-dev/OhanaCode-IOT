#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define STATION_IP_ADDR         (SSID_EEPROM_ADDR + SSID_SIZE)
#define STATION_IP_SIZE         (4)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE + STATION_IP_SIZE)   

#define LARGE_TIMEOUT           (60) /* Used to check if configured AP appeared and connect to it if possible */ 
#define PING_RX_PORT            (4210)  /* local port to listen on */
#define PING_RESPONSE_PORT      (4211u)
#define DEV_ID                  "02"
#define DNS_PORT                53

/* 19 characters maximum. When we append MAC addr, it will be 31. 
The device fails to create an AP if total AP name is longer than 31 character. */
#define AP_NAME_PREFIX                  "ujagaga_therm_"

#define USE_D1_MINI                     /* on wemos d1 mini we use a different pin for the temperature probe */
//#define USE_MAX6675                   /* Comment to use DS18B20 */
//#define DISPLAY_INTEGER_TEMPERATURE   /* Comment this to use one decimal for temperature display. */
  


#ifdef USE_MAX6675
  #ifdef USE_D1_MINI
  #define thermoSO              (5)   /* D1 */
  #define thermoCS              (4)   /* D2 */
  #define thermoCLK             (0)   /* D3 */
  #else
  #define thermoSO              (1)
  #define thermoCS              (0)
  #define thermoCLK             (3)
  #endif
#else
  #define USE_DS18B20
  
  #ifdef USE_D1_MINI
  #define SENSPIN               (5) /* D1 */          
  #else
  #define SENSPIN               (0)          
#endif

#endif


#endif
