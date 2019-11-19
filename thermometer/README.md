# WiFi thermometer

This sketch is using ESP8266 WiFi module to communicate with a temperature sensor. It provides a web interface, so you can connect to it using any web enabled device.
If it does not find the configured AP, it will provide its own open AP with SSID: "ujagaga_thermometer_" followed by MAC address of the WiFi module.
This sketch works with ESP8266 general module and with Wemos D1 mini and supports DS18B20 (up to 120 celsius) and MAX6675 (thermopar probe IC for high temperatures).
To get the temperature readout out of DS18B20, I wrote my own C library and only one probe is supported. I also made it state machine driven so it does not block execution 
of main script on long delays during the read.
For MAX6675 I am using the arduino library from ladyada.

logo.h contains a 64-base encoded logo used in the web interface.
