# NTP clock lamp

This Arduino sketch is using ESP8266 WiFi module, intended for D1 Mini, but any other will do.
It provides a web interface, so you can connect to it using any web enabled device.
If it does not find the configured AP, it will provide its own open AP with SSID: "ujagaga_ntp_timer_" followed by MAC address of the WiFi module.
The WiFi module uses NTP server to sync time. This is used to show current time.
You can set a time to set a pin high and when to set it low, so you can controll a device based on current time.
The web interface enables setting of the correct time zone.

logo.h contains a 64-base encoded logo used in the web interface.


NOT DONE YET
