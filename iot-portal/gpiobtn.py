#!/usr/bin/env python3

'''
Orange PI GPIO module
sudo apt install python3-dev
pip3 install OrangePi.GPIO
'''

import OPi.GPIO as GPIO
import time
from paho.mqtt import client as mqtt_client

BTN_PIN = 12
broker = 'localhost'
port = 1883
device_mac = "CC50E32E88F3"
client_id = "btn_" + device_mac


def send_to_mqtt(msg_to_send):
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    client.loop_start()
    result = client.publish(device_mac, msg_to_send)
    status = result[0]
    if status == 0:
        print("Sent")
    else:
        print("Failed")

    time.sleep(0.1)
    client.disconnect()


def setup():
    GPIO.setboard(GPIO.ZERO)
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(BTN_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)


setup()

try:
    while True:
        if not GPIO.input(BTN_PIN):
            # Send -1 to toggle
            msg = '{"current": [-1]}'
            send_to_mqtt(msg)

            # Wait for button release
            while not GPIO.input(BTN_PIN):
                time.sleep(0.1)

finally:
    GPIO.cleanup()

