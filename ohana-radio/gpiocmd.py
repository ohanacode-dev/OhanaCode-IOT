#!/usr/bin/env python3

'''
Orange PI GPIO module
sudo apt install python3-dev
pip3 install OrangePi.GPIO
'''

import OPi.GPIO as GPIO
import time
import subprocess

BTN_NEXT = 23
BTN_PREV = 19
BTN_PAUSE = 21

pause_timestamp = 0


def run_process(command_list):
    result = subprocess.run(command_list, stdout=subprocess.PIPE)
    print("Running: {}".format(command_list))

    return str(result.stdout, 'utf-8')


def setup():
    GPIO.setboard(GPIO.ZERO)
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(BTN_NEXT, GPIO.IN,
               pull_up_down=GPIO.PUD_UP)
    GPIO.setup(BTN_PREV, GPIO.IN,
               pull_up_down=GPIO.PUD_UP)
    GPIO.setup(BTN_PAUSE, GPIO.IN,
               pull_up_down=GPIO.PUD_UP)


def get_current():
    current = 0
    total = 1
    cmd = ['mpc']
    status = run_process(cmd).split('\n')

    try:
        for i in range(0, len(status)):
            if '[playing]' in status[i]:
                data = status[i].split('#')[1].split(' ')[0].split('/')
                current = int(data[0])
                total = int(data[1])
    except Exception as e:
        print('get_current() ERROR:', e)

    return current, total


def next_station():
    current, total = get_current()
    current += 1
    if current > total:
        current = 1

    cmd = ['mpc', 'play', str(current)]
    run_process(cmd)


def previous_station():
    current, total = get_current()
    current -= 1
    if current < 1:
        current = total

    cmd = ['mpc', 'play', str(current)]
    run_process(cmd)


def stop_mpc():
    cmd = ['mpc', 'stop']
    run_process(cmd)


def play_mpc():
    cmd = ['mpc', 'play']
    run_process(cmd)


def tgl_mpc():
    current, total = get_current()

    if current == 0:
        play_mpc()
    else:
        stop_mpc()


setup()
time.sleep(10)
play_mpc()

try:
    while True:
        if not GPIO.input(BTN_NEXT):
            next_station()
            while not GPIO.input(BTN_NEXT):
                time.sleep(0.1)

        elif not GPIO.input(BTN_PREV):
            previous_station()
            while not GPIO.input(BTN_PREV):
                time.sleep(0.1)

        elif not GPIO.input(BTN_PAUSE):
            tgl_mpc()
            pause_timestamp = time.time()

            while not GPIO.input(BTN_PAUSE):
                time.sleep(0.1)
                if time.time() - pause_timestamp > 3:
                    cmd = ['poweroff']
                    run_process(cmd)
                    exit(0)

        time.sleep(0.1)

finally:                        # this block will run no matter how the try block exits
    GPIO.cleanup()              # clean up after yourself
