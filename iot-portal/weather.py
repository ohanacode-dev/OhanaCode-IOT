#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
Script to display weather data. Uses www.weatherbit.io.
Dependencies: pip3 install requests
City ID can be acquired from city.list.json.gz at: http://bulk.openweathermap.org/sample/city.list.json.gz
'''

import requests
import json
from paho.mqtt import client as mqtt_client
import time


API_KEY = "6b15e16bb3dc4ba1b063d0836d56ce44"
API_URL = "http://api.weatherbit.io/v2.0/current"
CITY_ID = '3194360'     # Novi Sad
# CITY_ID = '789518'    # Kikinda
# CITY_ID = '783814'    # Zrenjanin
# CITY_ID = '792680'    # Beograd
ASCII_ICON_DAY = {
    '200': '\u2633', '201': '\u2633', '202': '\u2633', '230': '\u2633', '231': '\u2633', '232': '\u2633',
    '233': '\u2633', '300': '\u1F328', '301': '\u1F328', '302': '\u1F328', '500': '\u2614', '501': '\u2614',
    '502': '\u2614', '511': '\u2614', '520': '\u2614', '521': '\u2614', '522': '\u2614', '600': '\u2603',
    '601': '\u2603', '602': '\u2603', '610': '\u2603', '611': '\u2603', '612': '\u2603', '621': '\u2603',
    '622': '\u2603', '623': '\u2603', '700': '\u2601', '711': '\u2601', '721': '\u2601', '731': '\u2601',
    '741': '\u2601', '751': '\u2601', '800': '\u263C', '801': '\u263C', '802': '\u2601', '803': '\u2601',
    '804': '\u2601', '900': '\u2614'
}

ASCII_ICON_NIGHT = {
    '200': '\u2633', '201': '\u2633', '202': '\u2633', '230': '\u2633', '231': '\u2633', '232': '\u2633',
    '233': '\u2633', '300': '\u1F328', '301': '\u1F328', '302': '\u1F328', '500': '\u2614', '501': '\u2614',
    '502': '\u2614', '511': '\u2614', '520': '\u2614', '521': '\u2614', '522': '\u2614', '600': '\u2603',
    '601': '\u2603', '602': '\u2603', '610': '\u2603', '611': '\u2603', '612': '\u2603', '621': '\u2603',
    '622': '\u2603', '623': '\u2603', '700': '\u2601', '711': '\u2601', '721': '\u2601', '731': '\u2601',
    '741': '\u2601', '751': '\u2601', '800': '\u263D', '801': '\u263D', '802': '\u2601', '803': '\u2601',
    '804': '\u2601', '900': '\u2614'
}

broker = 'localhost'
port = 1883
topic = "iot"
client_id_w = "weather"
client_id_t = "temp_external"


def send_to_mqtt(msg, client_id):
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


def query_server():
    url_params = {'key': API_KEY, 'city_id': CITY_ID}
    r = requests.get(API_URL, params=url_params)
    json_ret_val = json.loads(r.text)

    return json_ret_val


def get_weather():
    response = query_server()
    data = response['data'][0]
    temp = data['temp']
    weather = data['weather']
    description = weather['description']
    weather_code = '{}'.format(weather['code'])
    weather_icon = weather['icon']

    if weather_icon.endswith('n'):
        icon = ASCII_ICON_NIGHT[weather_code]
    else:
        icon = ASCII_ICON_DAY[weather_code]

    t_msg = '{}{}C'.format(temp, u'\N{DEGREE SIGN}')
    w_msg = '{} {}'.format(description, icon)

    return t_msg, w_msg


if __name__ == '__main__':
    # Get weather data
    external_temp, weather_msg = get_weather()

    # Send to mqtt server.
    send_to_mqtt("Wake up")

    msg = {"name": client_id_w, "msg": weather_msg}
    print(msg)
    mqtt_msg = json.dumps(msg)
    send_to_mqtt(mqtt_msg, client_id_w)

    msg = {"name": client_id_t, "msg": external_temp}
    print(msg)
    mqtt_msg = json.dumps(msg)
    send_to_mqtt(mqtt_msg, client_id_t)
