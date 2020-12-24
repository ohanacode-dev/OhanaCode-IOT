#ifndef CONFIG_H
#define CONFIG_H

#include <ESP8266WiFi.h>

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define STATION_IP_ADDR         (SSID_EEPROM_ADDR + SSID_SIZE)
#define STATION_IP_SIZE         (4)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE + STATION_IP_SIZE)   

#define LARGE_TIMEOUT           (60) /* Used to check if configured AP appeared and connect to it if possible */ 
#define PING_RX_PORT            (4210u)  /* local port to listen on */
#define PING_RESPONSE_PORT      (4211u)
#define DEV_ID                  "13"
#define DNS_PORT                53

/* 19 characters maximum. When we append MAC addr, it will be 31. 
The device fails to create an AP if total AP name is longer than 31 character. */
#define AP_NAME_PREFIX          "OC_uart_"

#endif
