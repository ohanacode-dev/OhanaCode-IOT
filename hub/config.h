#ifndef CONFIG_H
#define CONFIG_H

#include <ESP8266WiFi.h>

#define MAX_DEV_COUNT           (20)  /* Maximum number of devices supported */
#define MAX_LABEL_LEN           (32u)
#define MAX_MAC_LEN             (19u)

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define STATION_IP_ADDR         (SSID_EEPROM_ADDR + SSID_SIZE)
#define STATION_IP_SIZE         (4)
#define LABELS_ADDR             (STATION_IP_ADDR + STATION_IP_SIZE)
#define LABELS_SIZE             ((MAX_MAC_LEN + MAX_LABEL_LEN) * MAX_DEV_COUNT)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE + STATION_IP_SIZE + LABELS_SIZE)   

#define LARGE_TIMEOUT           (60) /* Used to check if configured AP appeared and connect to it if possible */ 
#define PING_RX_PORT            (4211u)  /* local port to listen on */
#define PING_TX_PORT            (4210u)
#define DEV_ID                  "99"
#define DNS_PORT                53
#define PING_REPPEAT_TIMEOUT    (10000u)         

/* 19 characters maximum. When we append MAC addr, it will be 31. 
The device fails to create an AP if total AP name is longer than 31 character. */
#define AP_NAME_PREFIX          "OC_hub_"


/* Device types we can remotelly control */
#define DEVT_LED_DIMMER_LAMP    (1)
#define DEVT_THERMOMETER        (2)
#define DEVT_THERMOSTAT03       (3)
#define DEVT_THERMOSTAT04       (4)
#define DEVT_LED_RGB_LAMP       (5)

#endif
