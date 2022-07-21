# LED RGB lamp

This Arduino sketch is using **ESP8266** WiFi module to controll an RGB LED lamp. 
It provides a web interface, so you can connect to it using any web enabled device.
If it does not find the configured AP, it will provide its own open AP with SSID: **"OC_rgba_"** followed by MAC address of the WiFi module.

It supports an MQTT server and looks for one at address: "http://iot-portal.local". This way I can have any IP address for my mqtt broker and all devices find it.
