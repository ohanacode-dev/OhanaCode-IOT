#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define USR_PASS_EEPROM_ADDR    (SSID_EEPROM_ADDR + SSID_SIZE)
#define USR_PASS_SIZE           (32)
#define USR_NAME_EEPROM_ADDR    (USR_PASS_EEPROM_ADDR + USR_PASS_SIZE)
#define USR_NAME_SIZE           (32)
#define STATION_IP_ADDR         (USR_NAME_EEPROM_ADDR + USR_NAME_SIZE)
#define STATION_IP_SIZE         (4)
#define SMTP_LOGIN_EEPROM_ADDR  (STATION_IP_ADDR + STATION_IP_SIZE)
#define SMTP_LOGIN_SIZE         (32)
#define SMTP_PASS_EEPROM_ADDR   (SMTP_LOGIN_EEPROM_ADDR + SMTP_LOGIN_SIZE)
#define SMTP_PASS_SIZE          (32)
#define SMTP_PORT_EEPROM_ADDR   (SMTP_PASS_EEPROM_ADDR + SMTP_PASS_SIZE)
#define SMTP_PORT_SIZE          (2)
#define SMTP_SERVER_EEPROM_ADDR (SMTP_PORT_EEPROM_ADDR + SMTP_PORT_SIZE)
#define SMTP_SERVER_SIZE        (32)
#define SMTP_TO_EEPROM_ADDR     (SMTP_SERVER_EEPROM_ADDR + SMTP_SERVER_SIZE)
#define SMTP_TO_SIZE            (32)
#define SMTP_SUBJECT_EEPROM_ADDR     (SMTP_TO_EEPROM_ADDR + SMTP_TO_SIZE)
#define SMTP_SUBJECT_SIZE            (32)
#define IPCHK_URL_EEPROM_ADDR   (SMTP_SUBJECT_EEPROM_ADDR + SMTP_SUBJECT_SIZE)
#define IPCHK_URL_SIZE          (64)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE + USR_PASS_SIZE + USR_NAME_SIZE + STATION_IP_SIZE + \
                                  SMTP_LOGIN_SIZE + SMTP_PASS_SIZE + SMTP_PORT_SIZE + SMTP_PORT_SIZE + SMTP_TO_SIZE + \
                                  SMTP_SUBJECT_SIZE + IPCHK_URL_SIZE)    
#define LARGE_TIMEOUT           (120) /* Used to check if configured AP appeared and connect to it if possible */ 
#define MAX_DEV_COUNT           (10)  /**/
/* Device types we can remotelly control */
#define DEVT_LED_DIMMER_LAMP    (1)
#define DEVT_THERMOMETER        (2)
#define DEVT_THERMOSTAT03       (3)
#define DEVT_THERMOSTAT04       (4)

#define UDP_PORT                (4210)  /* local port to listen on */
#define EXT_IP_CHK_INTERVAL     (120)   /* How often to check external IP in seconds */
/* default email setup */
#define SMTP_PORT               (465)
#define SMTP_SERVER             "smtp.gmail.com"
#define SMTP_LOGIN              "ujagaga@gmail.com"
#define SMTP_PASSWORD           ""
#define SMTP_RECEPIENT          "ujagaga@gmail.com"
#define SMTP_SUBJECT            "External IP address change"
#define SMTP_TIMEOUT            (10000)

#endif
