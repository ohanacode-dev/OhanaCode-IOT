#!/usr/bin/env python3

# -*- coding: utf-8 -*-
'''
Written for python3
'''

from socket import socket, AF_INET, SOCK_DGRAM, SOL_SOCKET, SO_BROADCAST, SOCK_STREAM
from uuid import getnode as get_mac
import threading
from time import sleep, time
import sys
from subprocess import Popen, CalledProcessError, PIPE
from json import dumps as strEscape
from command_executor import Cmd, CmdMod, execute_cmd


# if 'linux' in sys.platform:

def cleanup_string(msg):
    esc_seq = [['%20', ' '], ['%9E', 'ž'], ['%8E', 'Ž'], ['%9A', 'š'], ['%8A', 'Š'], ['%26%23269%3B', 'č'],
               ['%26%23268%3B', 'Č'], ['%26%23263%3B', 'ć'], ['%26%23273%3B', 'đ'], ['%26%23272%3B', 'Đ'],
               ['%26%23262%3B', 'Ć'], ['%27', "'"], ['%22', '"']]

    for c in esc_seq:
        while c[0] in msg:
            msg = msg.replace(c[0], c[1])

    return msg


DEV_ID = "92"
BROADCAST_PORT = 4210
RX_PORT = 4213
IP = None
port = 0
stop_flag = False
MAC = ""
current_window_title = ""
APP_TITLE = "Lazy Control"
MSG_PING = "ujagaga ping"
last_shutdown_timestamp = 0
last_cmd_timestamp = 0
last_ping_timestamp = 0
FEATURES = '"MODEL":"Multimedia Control"'


if 'linux' in sys.platform:
    def get_active_window_name():
        try:
            return Popen(["xdotool", "getwindowfocus", "getwindowname"], stdout=PIPE).communicate()[0].decode()
        except CalledProcessError as e:
            return ""

else:
    from win32gui import GetWindowText, GetForegroundWindow

    def get_active_window_name():
        return GetWindowText(GetForegroundWindow())


def thread_window_title():
    global current_window_title
    global stop_flag

    print("Starting window title thread\n")

    while not stop_flag:
        current_window_title = strEscape(get_active_window_name(), ensure_ascii=True)
        sleep(1)

    print("Ending window title thread\n")


def thread_countdown():
    global flag_shutdown_init
    global last_shutdown_timestamp

    print("Starting countdown monitor thread\n")

    while not stop_flag:
        if flag_shutdown_init:

            try:
                if OS_PLATFORM == 'linux':
                    # system('play ' + current_path + '/theend.wav')
                    run_async_process(['play', current_path + '/theend.wav'])
                else:
                    PlaySound(current_path + "\\theend.wav", SND_FILENAME)

                box = CountDownBox()
                box.mainloop()

                if OS_PLATFORM == 'linux':
                    run_async_process(['play', current_path + '/startup.wav'])
                    # system('play ' + current_path + '/startup.wav')
                else:
                    PlaySound(current_path + "\\startup.wav", SND_FILENAME)
            except:
                pass

            flag_shutdown_init = False

        sleep(1)
    print("Stopping countdown monitor thread\n")


def parse_cmd(cmdVal):
    global last_mute_timestamp
    global current_window_title

    result = "OK"

    if cmdVal == 1:
        print("p window close")
        execute_cmd(Cmd.F4, CmdMod.ALT)

    elif cmdVal == 2:
        print("Lp Shutdown")
        execute_cmd(Cmd.POWEROFF)

    elif cmdVal == 3:
        print("p stop")
        execute_cmd(Cmd.STOP)

    elif cmdVal == 4:
        print("lp stop")
        execute_cmd(Cmd.STOP)

    elif cmdVal == 5:
        print("p vol up")
        execute_cmd(Cmd.VOL_UP)

    elif cmdVal == 6:
        print("lp vol up")
        execute_cmd(Cmd.VOL_UP)

    elif cmdVal == 7:
        print("p prew")
        execute_cmd(Cmd.PREVIOUS, current_window_title.lower())

    elif cmdVal == 8:
        print("lp rew")
        execute_cmd(Cmd.REWIND, current_window_title.lower())

    elif cmdVal == 9:
        print("p play/pause")
        execute_cmd(Cmd.PLAY, current_window_title.lower())

    elif cmdVal == 10:
        print("lp play/pause")
        execute_cmd(Cmd.SPACEBAR)

    elif cmdVal == 11:
        print("p next")
        execute_cmd(Cmd.NEXT, current_window_title.lower())

    elif cmdVal == 12:
        print("lp ffwd")
        execute_cmd(Cmd.FORWARD, current_window_title.lower())

    elif cmdVal == 13:
        print("p vol down")
        execute_cmd(Cmd.VOL_DOWN)

    elif cmdVal == 14:
        print("lp vol down")
        execute_cmd(Cmd.MUTE)

    elif cmdVal == 15:
        print("p media")
        execute_cmd(Cmd.MEDIA)

    elif cmdVal == 16:
        print("lp media")
        execute_cmd(Cmd.MEDIA)

    elif cmdVal == 17:
        print("Window title request")
        result = current_window_title

    else:
        print("ERROR: TCP API, unknown command: ", cmdVal)
        result = "ERROR: TCP API, unknown command"

    return result


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


def thread_beacon():
    global IP
    global BROADCAST_PORT
    global MSG_PING
    global stop_flag
    global MAC
    global last_ping_timestamp

    print("Starting ping responder thread\n")

    mac = "%012X" % get_mac()
    MAC = ""
    for i in range(0, 10, 2):
        MAC += mac[i: i+2] + ":"

    MAC += mac[10: 12]

    while not stop_flag:
        try:
            (msg, (sender_addr, sender_port)) = broadcast_receiver.recvfrom(24)

            print("Received:", msg, " From:", sender_addr, "Port:", sender_port)

            if (MSG_PING in msg.decode("utf-8")) and ((time() - last_ping_timestamp) > 0.100):
                if not send_tcp_response(sender_addr, MAC.encode()):
                    # No TCP server available. Maybe an older app expecting UDP response.
                    print("ERROR: No TCP connection!")
                else:
                    print("Responding via TCP with: ", MAC + ":" + DEV_ID)

                last_ping_timestamp = time()
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

    print("Ending ping responder thread\n")


def tcp_server():
    global RX_PORT

    server = socket(AF_INET, SOCK_STREAM)

    try:
        server.bind((IP, RX_PORT))
        server.listen(1)

        while True:
            # Wait for a connection
            connection, client_address = server.accept()
            try:
                # Receive the data in small chunks and retransmit it
                while True:
                    data = connection.recv(16)
                    print('received {}'.format(data))
                    if data:
                        connection.sendall(parse_cmd(data).encode('utf-8'))
                    else:
                        break

            finally:
                # Clean up the connection
                connection.close()
    except Exception as e:
        print("ERROR: {}".format(e))


def init():
    global stop_flag

    IP = get_ip()
    while IP is None:
        sleep(5)
        IP = get_ip()

    print("Running on IP: ", IP, ":", BROADCAST_PORT)

    t_beacon = threading.Thread(target=thread_beacon)
    t_beacon.start()

    t_window_title = threading.Thread(target=thread_window_title)
    t_window_title.start()

    t_server = threading.Thread(target=tcp_server)
    t_server.start()

    # tcp_server()
    stop_flag = True
    print("Closed tcp server")
