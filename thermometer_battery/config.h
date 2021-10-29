#ifndef CONFIG_H
#define CONFIG_H

/* 19 characters maximum. When we append MAC addr, it will be 31. 
The device fails to create an AP if total AP name is longer than 31 character. */
#define DEVICE_NAME                  "temp_external"
#define DEEP_SLEEP_TIME               (15 * 60 * 1000 * 1000)
  

/* Define 2 WiFi AP to connect to in case one is not available. 
This is usefull when developing at home and then using elsewhere. */
#define SSID_1                        "HUAWEI-B315-05B6"
#define PASS_1                        "J0HYJGN547T"

#define SSID_2                        "Rada_i_Slavica"
#define PASS_2                        "ohana130315"

#endif
