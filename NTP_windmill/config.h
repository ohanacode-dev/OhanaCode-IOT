#ifndef CONFIG_H
#define CONFIG_H

#include <ESP8266WiFi.h>

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define STATION_IP_ADDR         (SSID_EEPROM_ADDR + SSID_SIZE)
#define STATION_IP_SIZE         (4)
#define TIMEZONE_ADDR           (STATION_IP_ADDR + STATION_IP_SIZE)
#define TIMEZONE_SIZE           (1)
#define DAYSAVE_ADDR            (TIMEZONE_ADDR + TIMEZONE_SIZE)
#define DAYSAVE_SIZE            (1)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE + STATION_IP_SIZE + TIMEZONE_SIZE + DAYSAVE_SIZE)   

#define LARGE_TIMEOUT           (190)   /* Used to check if configured AP appeared and connect to it if possible */ 
#define PING_RX_PORT            (4210u)  /* local port to listen on */
#define PING_RESPONSE_PORT      (4211u)
#define UDP_NTP_PORT            (2390)  /* local port to listen for ntp messages */
#define DEV_ID                  "05"
#define DNS_PORT                53

#define NTP_SYNC_INTERVAL       (60000)  /* Interval in ms to update time */
#define NTP_PACKET_SIZE         (48)    /* NTP time stamp is in the first 48 bytes of the message */
#define NTP_SERVER_NAME         "europe.pool.ntp.org"

/* 19 characters maximum. When we append MAC addr, it will be 31. 
The device fails to create an AP if total AP name is longer than 31 character. */
#define AP_NAME_PREFIX          "OC_ntp_clk_"

/* We are using unipolar motor to drive the hour hand. It has 200 steps per revolution. */
#define M_H_STEPS               (200)
/* Motor minute driver pins */
#define M_H_PIN1                 16 
#define M_H_PIN2                 14 
#define M_H_PIN3                 12 
#define M_H_PIN4                 13 

#define LED_PIN                  4

#endif
