**OhanaCode-IOT**

This is my personal Internet Of Things set of projects. It is intended as a low budget home automation system. 
I am building my own WiFi controlled devices and controlling them through their own web interface. 
Most devices can act as an access point or a client device on a WiFi network. After it connects to your home network, 
it gets difficult to find its IP address if it is dynamically alocated, so I also provided an Android app to list all devices on the same network. 
Every device responds to an UDP broadcast string "ujagaga ping" via TCP socket. 
There is also an implemented Over The Air update. 
Most projects are build using Arduino studio and an **ESP8266** device. I am using **Wemos D1 mini** and **ESP-12** devices. ESP-01 is possible to use for most projects, but without the OTA update due to insufficient flash size.

NOTE: this is still work in progress. I have refactored some of the projects to use a filesystem on ESP8266 flash memory for storing web interface files. Most of the projects are functional, but I still want to improove them.
Most projects support "Over The Air" update, but it does sometimes crash, so you will still ocasionally need the wired connection.

---

## Using the code

Every device has an Arduino sketch in it's own folder, so open the sketch in **Arduino studio** and set the proper board and parameters for your board, compile and program. 
For the web interface some projects use SPIFFS library to upload HTML, CSS, JS files to virtual file system in the SPI flash. To upload these files, you need **Arduino ESP8266 filesystem uploader**. It integrates in your Arduino studio and adds an entry in the **Tools** menu. To install it, follow instructions from: https://github.com/esp8266/arduino-esp8266fs-plugin
After that you might want to open the **serial monitor** to see the debug messages for more info. 
You can then connect to devices access point, use your browser to control and configure it. 
Project is still in development. I believe that the switch, LED_lamp, LED_lamp_RGB, lazy_control and OC-IOT-Browser have reached their final state and are ready for use. Other projects are usable, but will be refactored a lot. Reffer to each projects **readme** for current status. 

---

## lazy_control

A python multimedial server for your PC. Should work on windows and linux.

---

## LED_lamp

Just a single color LED light controll device. 

---

## LED_lamp_RGB

An RGB LED lamp controll.

---

## NTP_clock

A stepper motor control using an NTP server for time sync. This will drive two clock hands to show time.

---

## NTP_timer

An NTP synchronized timer to activate a relay.

---

## router

A device intended to enable external access to your home networks **Ohana-IOT** devices.

---

## thermometer

A thermometer using either low temperature probe **DS18B20** or a high temperature thermopar probe module with **max6675**.

---

## thermostat

A thermometer project expanded with a relay driving capability.

---

## IOTBrowser

An Android application to list and provide a quick access to all **Ohana-IOT** devices on the same network.

---


