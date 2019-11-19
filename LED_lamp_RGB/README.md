# LED RGB lamp

This Arduino sketch is using **ESP8266** WiFi module to controll an RGB LED lamp. 
It provides a web interface, so you can connect to it using any web enabled device.
If it does not find the configured AP, it will provide its own open AP with SSID: **"ujagaga_led_rgb_"** followed by MAC address of the WiFi module.
For **ESP-1** module, as a touch sensor, UART RX pin is used connected to any metal surface of around 2 square cm, so after programming, be sure to disconnect UART RX from the programmer as it will be used as an output pin to empty the virtual capacitor of the touch sensor. 
On **D1 mini**, for touch sensor pin GPIO5 is used. It is labeled D1. Also, just connect to a small metal surface and controll by touch.
For best results isolate it using nail polish or plastic foil.
A short touch will toggle the LED and a long one will increase LED light level.
