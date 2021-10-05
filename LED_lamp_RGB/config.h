#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define STATION_IP_ADDR         (SSID_EEPROM_ADDR + SSID_SIZE)
#define STATION_IP_SIZE         (4)
#define COLOR_ADDR              (STATION_IP_ADDR + STATION_IP_SIZE)
#define COLOR_SIZE              (4)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE + STATION_IP_SIZE + COLOR_SIZE)   

/* 19 characters maximum. When we append MAC addr, it will be 31. 
The device fails to create an AP if total AP name is longer than 31 character. */
#define AP_NAME_PREFIX          "OC_rgba_"    

#define LED_G_PIN                     (14)    
#define LED_R_PIN                     (12)   
#define LED_B_PIN                     (13)        

#define LED_STATUS_PIN                (2)   


#define DEBOUNCE_TIMEOUT              (100u)
#define LED_LEVEL_MAX                 (100u)

#endif
