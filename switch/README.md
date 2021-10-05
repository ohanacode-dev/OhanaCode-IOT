# LED lamp

This Arduino sketch is using **ESP8266** WiFi module to controll a lamp. 
It can also be used for other purposes like driving a motor or any other device. I am using a relay to turn the lights on/off. I tried using a triac, 
but it sometimes activates the light when there is an instabillity in the AC network, like my refridgerator switching on/off. This created a flash effect so had to get rid of it. 
It provides a web interface, so you can connect to it using any web enabled device.
If it does not find the configured AP at startup, it will provide its own open AP with SSID: **"OC_Switch_"** followed by MAC address of the WiFi module.

It supports an MQTT server and looks for one at address: "http://iot-portal.local". This way I can have any IP address for my mqtt broker and all devices find it.
