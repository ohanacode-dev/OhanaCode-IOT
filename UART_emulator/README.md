# LED lamp

This Arduino sketch is using **ESP8266** WiFi module to controll an LED lamp. 
It can also be used for other purposes like driving a DC motor, or setting an analog level on the LED pin for a voltage source in which case you would also need a low frequency RC filter.
It provides a web interface, so you can connect to it using any web enabled device.
If it does not find the configured AP, it will provide its own open AP with SSID: **"OC_led_"** followed by MAC address of the WiFi module.
As a touch sensor, a pin is used connected to any metal surface of around 2 square cm. 
On **D1 mini**, for touch sensor pin GPIO5 is used. It is labeled D1. Just connect to any metal surface of around 2 square cm and controll by touch.
For best results isolate it using nail polish or plastic foil.
A short touch will toggle the LED and a long one will increase LED light level.

