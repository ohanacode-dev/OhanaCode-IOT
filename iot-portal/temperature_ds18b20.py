#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
from paho.mqtt import client as mqtt_client
import json
import time


DEVICES_DIR = "/sys/bus/w1/devices/"
broker = 'localhost'
port = 1883
topic = "iot"
client_id = "temp_ambient"


def send_to_mqtt(msg):
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    client.loop_start()

    result = client.publish(topic, msg)
    status = result[0]
    if status == 0:
        print("Sent")
    else:
        print("Failed")

    time.sleep(0.1)
    client.disconnect()


def get_probe_device_path():
    for folder in os.listdir(DEVICES_DIR):
        if not folder.startswith('w1'):
            device_path = os.path.join(DEVICES_DIR, folder, 'w1_slave')
            if os.path.isfile(device_path):
                return device_path

    return None


def parse_device_file(dev_file_path):
    with open(dev_file_path, 'r') as device:
        lines = device.readlines()

        for line in lines:
            if 't=' in line:
                temp_str = line.split('t=')[1]
                temp = int(int(temp_str) / 1000)
                return temp

    return None


if __name__ == '__main__':
    # Get temperature
    dev_file = get_probe_device_path()
    temperature = parse_device_file(dev_file)
    result = "{}{}C".format(temperature, u'\N{DEGREE SIGN}')
    if temperature >= 0:
        # Instead of adding a '+' sign just add a space to make it similar width as negative number
        result = " " + result

    msg = {"name": client_id, "msg": result}
    print(msg)

    # Send to mqtt server
    send_to_mqtt("Wake up")
    mqtt_msg = json.dumps(msg)
    send_to_mqtt(mqtt_msg)


