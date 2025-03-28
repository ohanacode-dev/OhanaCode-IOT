# IOT personal local network portal

This is my implementation of a local network portal to access all my WiFi IOT devices 
in a central location. The structure of the portal is as follows:
1. An mqtt server as an intermediate between all my wifi devices and this web server.
2. This web server to display current device status, provide a UI and subscribe to topics 
that devices post to so it can write data to an SQLITE database for the UI to use.
3. A temporary SQLITE database /tmp/.iot-portal/database.db to store current list of devices and states
4. A persistent SQLITE database $HOME/.iot-portal/database.db to store device labels for a more user friendly UI

## Dependencies

    sudo apt install mosquitto mosquitto-clients
    pip3 install paho-mqtt fastapi uvicorn jinja2 aiofiles websocket-client websockets
    
## Expected mqtt message from a device

    mosquitto_pub -h localhost -t device_status -m '{"mac":"982CBCD5FCB4", "type":"switch", "addr":"192.168.0.153", "current":[0]}'    
    mosquitto_pub -h localhost -t device_status -m '{"mac":"982CBCD5FCB6", "type":"dimmer", "addr":"192.168.0.155", "current":[5, 100, 255]}'    
    mosquitto_pub -h localhost -t device_status -m '{"mac":"982CBCD5FCB7", "type":"thermometerc", "addr":"192.168.0.156", "current":[25]}'
    mosquitto_pub -h localhost -t device_status -m '{"mac":"982CBCD5FCBB", "type":"thermometerf", "addr":"192.168.0.160", "current":[25]}'
    mosquitto_pub -h localhost -t device_status -m '{"mac":"982CBCD5FCB8", "type":"thermostatc", "addr":"192.168.0.157", "current":[25], "target":[35], "state":[0]}'
    mosquitto_pub -h localhost -t device_status -m '{"mac":"982CBCD5FCBC", "type":"thermostatf", "addr":"192.168.0.161", "current":[100], "target":[105], "state":[0]}'
    mosquitto_pub -h localhost -t device_status -m '{"mac":"982CBCD5FCB9", "type":"clock", "addr":"192.168.0.158", "current":["10:35:45", "13:00:05"]}'
    mosquitto_pub -h localhost -t device_status -m '{"mac":"982CBCD5FCBA", "type":"timer", "addr":"192.168.0.159", "current":["10:35:45"], "target":["10:40:00"], "state":[0]}'
    
## Hardware

I am using an Orange Pi Zero board with Armbian linux. Also connected an OLED screen based on sh1106 driver and a DS18B20 temperature probe. They are both used by the oled_driver.py.
To use I2C for OLED, I had to enable it in armbian-config along with w1-gpio driver for temperature probe.
Also needed to append to /etc/modules-load.d/modules.conf:

    12c-dev
    w1-gpio
    w1-therm

The probe is connected to pin A14. This required appending to /boot/armbianEnv.txt:

    param_w1_pin=PA14
    param_w1_pin_int_pullup=1

Now reboot and you should have a folder with the sensor ID under /sys/bus/w1/devices/. More datails available on https://blog.ja-ke.tech/2019/01/21/DS18B20-armbian.html.
