#ifndef CONFIG_H
#define CONFIG_H


#define COLOR_ADDR              (0)
#define COLOR_SIZE              (4)
#define EEPROM_SIZE             (COLOR_SIZE) 

/* 19 characters maximum. When we append MAC addr, it will be 31. 
The device fails to create an AP if total AP name is longer than 31 character. */
#define AP_NAME_PREFIX          "Ap_name"    
#define AP_PASS                 "My_ap_pass"

#define DATA_PIN      (1)
#define NUM_LEDS      (60)
#define BRIGHTNESS    (100) // Set BRIGHTNESS (max = 255)
       
#define DEBOUNCE_TIMEOUT              (100u)
#define LED_LEVEL_MAX                 (100u)


#if NUM_LEDS > 255
#error "ERROR: Maximum supported number of pixels is 255."
#endif

#endif
