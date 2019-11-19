#ifndef CONFIG_H
#define CONFIG_H

#define USE_MAX6675   /* Comment to use DS18B20 */

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define STATION_IP_ADDR         (SSID_EEPROM_ADDR + SSID_SIZE)
#define STATION_IP_SIZE         (4)
#define TARGET_TEMP_ADDR        (STATION_IP_ADDR + STATION_IP_SIZE)
#define TARGET_TEMP_SIZE        (8)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE + STATION_IP_SIZE + TARGET_TEMP_SIZE)   

#define LARGE_TIMEOUT           (120) /* Used to check if configured AP appeared and connect to it if possible */ 
#define UDP_PORT                (4210)  /* local port to listen on */
#define DNS_PORT                53

/* 19 characters maximum. When we append MAC addr, it will be 31. 
The device fails to create an AP if total AP name is longer than 31 character. */
#define AP_NAME_PREFIX          "ujagaga_TStat_"

#define USE_D1_MINI   /* on wemos d1 mini we use a different pin for the temperature probe */

#define DISPLAY_INTEGER_TEMPERATURE   /* Comment this to use one decimal for temperature display. */

#ifdef USE_MAX6675
#define DEV_ID  "04"
#else
#define USE_DS18B20
#define DEV_ID  "03"
#endif

#ifdef USE_DS18B20
  #ifdef USE_D1_MINI
    #define SENSPIN       5  
    #define RELAY_PIN     2         /* Tx Pin used for relay activating  */        
  #else
    #define SENSPIN       0       
    #define RELAY_PIN     2         /* Tx Pin used for relay activating  */   
  #endif
#else
  #ifdef USE_D1_MINI
    #define thermoSO      5
    #define thermoCS      4
    #define thermoCLK     0
    #define RELAY_PIN     2         /* Tx Pin used for relay activating  */
  #else
    #define thermoSO      1    
    #define thermoCS      0
    #define thermoCLK     3
    #define RELAY_PIN     2         /* Tx Pin used for relay activating  */
  #endif
#endif

#endif
