#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
Script to display weather data. Uses www.weatherbit.io.
Dependencies: pip3 install requests
City ID can be acquired from city.list.json.gz at: http://bulk.openweathermap.org/sample/city.list.json.gz
'''

import requests
import json
from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import sh1106
import time
from PIL import ImageFont


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

FONT_WEATHER = ImageFont.truetype("./DejaVuSans.ttf", 12)
serial = i2c(port=0, address=0x3C)
device = sh1106(serial)
MARGIN = 10


def query_server():
    url_params = {'key': API_KEY, 'city_id': CITY_ID}
    r = requests.get(API_URL, params=url_params)
    json_ret_val = json.loads(r.text)

    return json_ret_val


def split_text_2_rows_by_letter(in_text):
    with canvas(device) as draw:
        text_0 = in_text
        text_len = draw.textsize(text_0, FONT_WEATHER)[0]

        while text_len > (device.width - 6):
            # Remove symbols from the end
            text_0 = text_0[:-1]
            text_len = draw.textsize(text_0, FONT_WEATHER)[0]

        text_1 = in_text[len(text_0) - 1:]
        text_len = draw.textsize(text_1, FONT_WEATHER)[0]

        while text_len > (device.width - 6):
            # Remove symbols from the end
            text_1 = text_1[:-1]
            text_len = draw.textsize(text_1, FONT_WEATHER)[0]

    return [text_0, text_1]


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


def output_weather_to_display(text):
    out_text = split_text_2_rows_by_word(text)
    with canvas(device) as draw:
        draw.text((0, device.height - 1 - FONT_WEATHER.size * 2), out_text[0], fill="white", font=FONT_WEATHER)
        draw.text((0, device.height - 1 - FONT_WEATHER.size), out_text[1], fill="white", font=FONT_WEATHER)

    time.sleep(1)


if __name__ == '__main__':
    while True:
        print("Getting weather")
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

        msg = '{}{}C {} {}'.format(temp, u'\N{DEGREE SIGN}', description, icon)

        output_weather_to_display(msg)
        time.sleep(60)