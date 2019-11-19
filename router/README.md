# Ujagaga IOT Router

This Arduino sketch is using ESP8266 WiFi module as a router to enable access to ujagaga IOT devices from outside local network. 
To use it, you have to set this devices IP address to static and then forward port 80 and port 81 in your router to the IP address 
of this device. The Ujagaga IOT Router will use UDP broadcast to list all devices on the LAN and form its web interface with the 
list of recognized devices, so you can access any of them. 

This sketch works with ESP8266 general module and with Wemos D1 mini.
logo.h contains a 64-base encoded logo used in the web interface.
