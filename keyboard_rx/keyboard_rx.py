#!/usr/bin/env python3

# -*- coding: utf-8 -*-
'''
Written for python3
'''

from socket import socket, AF_INET, SOCK_DGRAM, SOL_SOCKET, SO_BROADCAST, SOCK_STREAM
from uuid import getnode as get_mac
from time import sleep, time
import sys
from subprocess import Popen, CalledProcessError, PIPE
from os import path


if 'linux' in sys.platform:
    OS_PLATFORM = 'linux'
else:
    OS_PLATFORM = 'windows'

DEV_ID = "99"
BROADCAST_PORT = 4321
IP = None
port = 0
stop_flag = False
MAC = ""
flag_shutdown_init = False
CMD_PING = "wifikbd"
CMD_KEY = "cmd:"
CMD_MAC = "MAC:"
messages = []

# determine if application is a script file or frozen exe
if getattr(sys, 'frozen', False):
    current_path = path.dirname(sys.executable)
    if "windows" in OS_PLATFORM:
        extension = ".exe"
    else:
        extension = ""
elif __file__:
    current_path = path.dirname(path.realpath(__file__))
    extension = ".py"

cfg_path = path.expanduser("~") + "/.wifikbd"
cfg_name = "config.txt"


def run_async_process(command_list):
    print("Execute: ", command_list)
    try:
        Popen(command_list)
    except:
        pass

def get_ip():

    ipaddr = ""
    s = socket(AF_INET, SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        s.connect(('10.255.255.255', 0))
        ipaddr = s.getsockname()[0]
    except:
        s.close()
    finally:
        s.close()

    if len(ipaddr.split('.')) != 4:
        return None

    return ipaddr


def send_tcp_response(sender_address, msg):

    success_status = True

    try:
        responder = socket(AF_INET, SOCK_STREAM)
        responder.connect((sender_address, BROADCAST_PORT + 1))
        responder.send(msg)
        responder.close()
    except:
        success_status = False

    return success_status


IP = get_ip()
while IP is None:
    sleep(5)
    IP = get_ip()

print("Running on IP: ", IP, ":", BROADCAST_PORT)


print("Starting ping responder thread\n")

mac = "%012X" % get_mac()
MAC = ""
for i in range(0, 10, 2):
    MAC += mac[i: i+2] + ":"

MAC += mac[10: 12]

while True:
    try:
        (msg, (sender_addr, sender_port)) = broadcast_receiver.recvfrom(24)

        print("Received:", msg, " From:", sender_addr, "Port:", sender_port)
        rx = msg.decode("utf-8")

        if CMD_PING in rx:
            if (time() - last_ping_timestamp) > 0.100:
                if send_tcp_response(sender_addr, MAC.encode()):
                    print("Responding via TCP with: ", MAC + ":" + DEV_ID)

                last_ping_timestamp = time()
        elif CMD_MAC in rx:
            # Expecting message in format: mac:<MAC ADDR>,cmd:<key code>
            msg = rx.split(',')
            if (msg[0].split[':'][1] == MAC):
                key_code = msg[1].split[':'][1]
                print("Simulating key code: {}".format(key_code))
                # TODO key simulation
    except:
        try:
            broadcast_receiver.close()
        except:
            pass

        try:
            broadcast_receiver = socket(AF_INET, SOCK_DGRAM)
            broadcast_receiver.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)
            broadcast_receiver.settimeout(20)
            broadcast_receiver.bind(("", BROADCAST_PORT))
        except:
            pass

