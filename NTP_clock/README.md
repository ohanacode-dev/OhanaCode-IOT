# NTP clock lamp

This Arduino sketch is using ESP8266 WiFi module, intended for D1 Mini, but any other with a few GPIO pins will do.
It provides a web interface, so you can connect to it using any web enabled device.
If it does not find the configured AP, it will provide its own open AP with SSID: "ujagaga_ntp_clock_" followed by MAC address of the WiFi module.
The WiFi module uses NTP server to sync time. This is used to drive two stepper motors to show current time.
The web interface enables setting of the correct time zone.

logo.h contains a 64-base encoded logo used in the web interface.
