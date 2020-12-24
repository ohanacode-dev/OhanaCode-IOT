# NTP clock lamp

This Arduino sketch is using ESP8266 WiFi module.
It provides a web interface, so you can connect to it using any web enabled device.
If it does not find the configured AP, it will provide its own open AP with SSID: "OC_ntp_clock_" followed by MAC address of the WiFi module.
The WiFi module uses NTP server to sync time. 
The web interface enables setting of the correct time zone.

# Additional dependencies
- ESP8266 Web Server
- ESP8266HTTPClient
- NTPClient

