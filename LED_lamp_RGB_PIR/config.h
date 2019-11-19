#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define STATION_IP_ADDR         (SSID_EEPROM_ADDR + SSID_SIZE)
#define STATION_IP_SIZE         (4)
#define COLOR_ADDR              (STATION_IP_ADDR + STATION_IP_SIZE)
#define COLOR_SIZE              (6)
#define LED_VAL_ADDR            (COLOR_ADDR + COLOR_SIZE)
#define LED_VAL_SIZE            (1)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE + STATION_IP_SIZE + COLOR_SIZE + LED_VAL_SIZE)   

#define LARGE_TIMEOUT           (60) /* Used to check if configured AP appeared and connect to it if possible */ 
#define PING_RX_PORT            (4210u)  /* local port to listen on */
#define PING_RESPONSE_PORT      (4211u)
#define DEV_ID                  "05"
#define DNS_PORT                53

/* 19 characters maximum. When we append MAC addr, it will be 31. 
The device fails to create an AP if total AP name is longer than 31 character. */
#define AP_NAME_PREFIX          "ohana_rgb_pir"    

#define LED_G_PIN                     (14)    /* D3 */  
#define LED_R_PIN                     (16)    /* D2 */  
#define LED_B_PIN                     (12)    /* D1 */           
#define SENSPIN                       (13)   /* D5 */


#define LED_LEVEL_MAX                 (100u)

#endif
