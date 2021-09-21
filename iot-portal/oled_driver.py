#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
Script to display weather data. Uses www.weatherbit.io.
Dependencies: pip3 install requests
City ID can be acquired from city.list.json.gz at: http://bulk.openweathermap.org/sample/city.list.json.gz

To use the OLED screen you will need to enable I2C interface. On Armbian, you can use armbian-config to enable it.
Then append the driver to /etc/modules-load.d/modules.conf:
     12c-dev
     
'''

from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import sh1106
from PIL import ImageFont
from paho.mqtt import client as mqtt_client
import time
import json
import fontawesome as fa


FONT_WEATHER = ImageFont.truetype("DejaVuSans.ttf", 12)
FONT_TITLE = ImageFont.truetype("DejaVuSans.ttf", 10)
FONT_TEMP = ImageFont.truetype("DejaVuSans.ttf", 20)
FONT_ICON = ImageFont.truetype("fontawesome-webfont.ttf", 14)
serial = i2c(port=0, address=0x3C)
device = sh1106(serial)
WEATHER_ROW_1 = device.height - 1 - FONT_WEATHER.size * 2
WEATHER_ROW_2 = device.height - 1 - FONT_WEATHER.size
TITLE_ROW = 2
TEMPERATURE_ROW = TITLE_ROW + 12
MIDDLE_X = int(device.width / 2)

topic = "iot"
broker = 'localhost'
port = 1883

SENDER_WEATHER = "weather"
SENDER_TEMP_AMBIENT = "temp_ambient"
SENDER_TEMP_EXTERNAL = "temp_external"

weather_text = ""
temp_ambient = "----- "
temp_external = "----- "


class MqttReceiver:
    def __init__(self):
        self.host = broker
        self.port = port
        self.topic = topic
        self.rx_msg = None

        self.client = mqtt_client.Client('Rcv')
        try:
            self.client.connect(self.host, port=port)
            self.client.on_connect = self.on_connect
            self.client.on_message = self.on_message
            self.client.loop_start()
        except:
            pass

    def on_connect(self, client, userdata, flags, rc):
        client.subscribe(self.topic)

    def on_message(self, client, userdata, msg):
        global weather_text
        global temp_ambient
        global temp_external

        try:
            self.rx_msg = json.loads(msg.payload.decode())
            sender = self.rx_msg.get("name", "")
            message = self.rx_msg.get("msg", "")
            if sender == SENDER_WEATHER:
                weather_text = message
            elif sender == SENDER_TEMP_AMBIENT:
                temp_ambient = message
            elif sender == SENDER_TEMP_EXTERNAL:
                temp_external = message

        except Exception as ex:
            print("ERROR: ", ex)
            self.rx_msg = msg.payload.decode()

        # print('MQTT RX: {}'.format(self.rx_msg))
        output_to_display()

    def stop(self):
        self.client.disconnect()
        self.client.loop_stop()


def split_text_2_rows_by_word(in_text):
    with canvas(device) as draw:
        pieces = in_text.split(' ')
        text_0 = pieces[0]
        i = 1
        while i < len(pieces):
            test = text_0 + ' ' + pieces[i]
            test_len = draw.textsize(test, FONT_WEATHER)[0]
            if test_len < device.width:
                text_0 = test
                i += 1
            else:
                break

        if len(pieces) > i:
            text_1 = pieces[i]
            i += 1

            while i < len(pieces):
                test = text_1 + ' ' + pieces[i]
                test_len = draw.textsize(test, FONT_WEATHER)[0]
                if test_len < device.width:
                    text_1 = test
                    i += 1
                else:
                    break
        else:
            text_1 = ""

    return [text_0 + " ", text_1 + " "]


def output_to_display():
    display_weather_text = split_text_2_rows_by_word(weather_text)
    with canvas(device) as draw:
        draw.rectangle((0, 0, MIDDLE_X, WEATHER_ROW_1 - 1), outline="white", fill="black")
        draw.text((4, TITLE_ROW), fa.icons['home'], fill="white", font=FONT_ICON)
        draw.text((4, TEMPERATURE_ROW), temp_ambient, fill="white", font=FONT_TEMP)

        draw.rectangle((MIDDLE_X, 0, device.width - 1, WEATHER_ROW_1 - 1), outline="white", fill="black")
        draw.text((MIDDLE_X + 4, TITLE_ROW), fa.icons['tree'], fill="white", font=FONT_ICON)
        draw.text((MIDDLE_X + 4, TEMPERATURE_ROW), temp_external, fill="white", font=FONT_TEMP)

        draw.text((0, WEATHER_ROW_1), display_weather_text[0], fill="white", font=FONT_WEATHER)
        draw.text((0, WEATHER_ROW_2), display_weather_text[1], fill="white", font=FONT_WEATHER)


if __name__ == '__main__':
    receiver = MqttReceiver()
    output_to_display()
    try:
        while True:
            time.sleep(5)
    except Exception as e:
        receiver.stop()
