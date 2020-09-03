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
import command_executor


# Client messages
CODE_NONE = 0
CODE_ASCII = 1
CODE_SPECIAL = 2
CODE_MOUSE = 3
CODE_MSG_START1 = 127
CODE_MSG_START2 = 126

KEY_LEFT = 1
KEY_RIGHT = 2
KEY_UP = 3
KEY_DOWN = 4
KEY_PLAY = 5
KEY_CAPS = 6
KEY_VOLUP = 7
KEY_VOLDN = 8
KEY_PGDN = 9
KEY_PGUP = 10
KEY_ENTER = 11
KEY_MOUSE_LEFT = 12
KEY_MOUSE_RIGHT = 13
KEY_MOUSE_LEFT_DOWN = 14
KEY_MOUSE_RIGHT_DOWN = 15
KEY_MOUSE_LEFT_UP = 16
KEY_MOUSE_RIGHT_UP = 17
KEY_SPACE = 18
KEY_BACKSPACE = 19
KEY_MONKEY = 20
KEY_PLUS = 21
KEY_MINUS = 22
KEY_SLASH = 23
KEY_COLON = 24
KEY_SEMICOLON = 25
KEY_EQUAL = 26
KEY_PERCENT = 27
KEY_LESS = 28
KEY_GREATER = 29
KEY_QUESTION = 30
KEY_TILDE = 31
KEY_UNDERSCORE = 32
KEY_EXCLAMATION = 33
KEY_COMMA = 34
KEY_PERIOD = 35
KEY_QUOTE = 36
KEY_MOUSE_LEFT_DOUBLECLICK = 37
KEY_CLOSE = 38

DEV_ID = "50"
UDP_RX_PORT = 4210
TCP_RX_PORT = 4213
IP = None
stop_flag = False
MAC = ""
current_window_title = ""
MSG_PING = "ujagaga ping"
last_shutdown_timestamp = 0
last_cmd_timestamp = 0


def get_active_window_name():
    try:
        return Popen(["xdotool", "getwindowfocus", "getwindowname"], stdout=PIPE).communicate()[0].decode()
    except CalledProcessError as e:
        return ""


def thread_window_title():
    global current_window_title
    global stop_flag

    print("Starting window title reader\n")

    while not stop_flag:
        current_window_title = strEscape(get_active_window_name(), ensure_ascii=True)
        sleep(1)

    print("Ending window title reader\n")


def parse_cmd(cmdVal):
    global last_mute_timestamp
    global current_window_title

    if cmdVal == 1:
        print("p window close")
        command_executor.execute_cmd(command_executor.Cmd.F4, command_executor.CmdMod.ALT)

    elif cmdVal == 2:
        print("Lp Shutdown")
        command_executor.execute_cmd(command_executor.Cmd.POWEROFF)

    elif cmdVal == 3:
        print("p stop")
        command_executor.execute_cmd(command_executor.Cmd.STOP)

    elif cmdVal == 4:
        print("lp stop")
        command_executor.execute_cmd(command_executor.Cmd.STOP)

    elif cmdVal == 5:
        print("p vol up")
        command_executor.execute_cmd(command_executor.Cmd.VOL_UP)

    elif cmdVal == 6:
        print("lp vol up")
        command_executor.execute_cmd(command_executor.Cmd.VOL_UP)

    elif cmdVal == 7:
        print("p prew")
        command_executor.execute_cmd(command_executor.Cmd.PREVIOUS, current_window_title.lower())

    elif cmdVal == 8:
        print("lp rew")
        command_executor.execute_cmd(command_executor.Cmd.REWIND, current_window_title.lower())

    elif cmdVal == 9:
        print("p play/pause")
        command_executor.execute_cmd(command_executor.Cmd.PLAY, current_window_title.lower())

    elif cmdVal == 10:
        print("lp play/pause")
        command_executor.execute_cmd(command_executor.Cmd.SPACEBAR)

    elif cmdVal == 11:
        print("p next")
        command_executor.execute_cmd(command_executor.Cmd.NEXT, current_window_title.lower())

    elif cmdVal == 12:
        print("lp ffwd")
        command_executor.execute_cmd(command_executor.Cmd.FORWARD, current_window_title.lower())

    elif cmdVal == 13:
        print("p vol down")
        command_executor.execute_cmd(command_executor.Cmd.VOL_DOWN)

    elif cmdVal == 14:
        print("lp vol down")
        command_executor.execute_cmd(command_executor.Cmd.MUTE)

    elif cmdVal == 15:
        print("p media")
        command_executor.execute_cmd(command_executor.Cmd.MEDIA)

    elif cmdVal == 16:
        print("lp media")
        command_executor.execute_cmd(command_executor.Cmd.MEDIA)

    else:
        print("ERROR: TCP API, unknown command: ", cmdVal)


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
        responder.connect((sender_address, UDP_RX_PORT + 1))
        responder.send(msg)
        responder.close()
    except:
        success_status = False

    return success_status


def process_rx(rxbuf):

    if len(rxbuf) > 2:
        # print('rxbuf {}'.format(rxbuf), flush=True)

        try:
            cmd_code = rxbuf[0]
            if cmd_code == CODE_ASCII:
                #Send ASCII key
                command_executor.send_key(chr(rxbuf[1]))
            elif cmd_code == CODE_SPECIAL:
                # Send a special key:
                key_val = rxbuf[1]
                if key_val == KEY_LEFT:
                    command_executor.send_key(command_executor.KEYPRESS.ARROW_LEFT.value)
                elif key_val == KEY_RIGHT:
                    command_executor.send_key(command_executor.KEYPRESS.ARROW_RIGHT.value)
                elif key_val == KEY_UP:
                    command_executor.send_key(command_executor.KEYPRESS.ARROW_UP.value)
                elif key_val == KEY_DOWN:
                    command_executor.send_key(command_executor.KEYPRESS.ARROW_DOWN.value)
                elif key_val == KEY_PLAY:
                    command_executor.execute_cmd(command_executor.Cmd.PLAY, current_window_title.lower())
                elif key_val == KEY_CAPS:
                    command_executor.send_key(command_executor.KEYPRESS.CAPS_LOCK.value)
                elif key_val == KEY_VOLUP:
                    command_executor.send_key(command_executor.KEYPRESS.VOLUME_UP.value)
                elif key_val == KEY_VOLDN:
                    command_executor.send_key(command_executor.KEYPRESS.VOLUME_DOWN.value)
                elif key_val == KEY_PGDN:
                    command_executor.send_key(command_executor.KEYPRESS.PAGE_DOWN.value)
                elif key_val == KEY_PGUP:
                    command_executor.send_key(command_executor.KEYPRESS.PAGE_UP.value)
                elif key_val == KEY_ENTER:
                    command_executor.send_key(command_executor.KEYPRESS.ENTER.value)
                elif key_val == KEY_MOUSE_LEFT:
                    command_executor.mouse_click(command_executor.Cmd.MOUSE_CLICK_LEFT)
                elif key_val == KEY_MOUSE_RIGHT:
                    command_executor.mouse_click(command_executor.Cmd.MOUSE_CLICK_RIGHT)
                elif key_val == KEY_MOUSE_LEFT_DOWN:
                    command_executor.mouse_click(command_executor.Cmd.MOUSE_DOWN_LEFT)
                elif key_val == KEY_MOUSE_LEFT_DOUBLECLICK:
                    command_executor.mouse_click(command_executor.Cmd.MOUSE_DOUBLECLICK_LEFT)
                elif key_val == KEY_MOUSE_RIGHT_DOWN:
                    command_executor.mouse_click(command_executor.Cmd.MOUSE_DOWN_RIGHT)
                elif key_val == KEY_MOUSE_LEFT_UP:
                    command_executor.mouse_click(command_executor.Cmd.MOUSE_UP_LEFT)
                elif key_val == KEY_MOUSE_RIGHT_UP:
                    command_executor.mouse_click(command_executor.Cmd.MOUSE_UP_RIGHT)
                elif key_val == KEY_SPACE:
                    command_executor.send_key(command_executor.KEYPRESS.SPACEBAR.value)
                elif key_val == KEY_BACKSPACE:
                    command_executor.send_key(command_executor.KEYPRESS.BACKSPACE.value)
                elif key_val == KEY_MONKEY:
                    command_executor.send_key(command_executor.KEYPRESS.MONKEY.value)
                elif key_val == KEY_PLUS:
                    command_executor.send_key(command_executor.KEYPRESS.PLUS.value)
                elif key_val == KEY_MINUS:
                    command_executor.send_key(command_executor.KEYPRESS.MINUS.value)
                elif key_val == KEY_SLASH:
                    command_executor.send_key(command_executor.KEYPRESS.SLASH.value)
                elif key_val == KEY_COLON:
                    command_executor.send_key(command_executor.KEYPRESS.COLON.value)
                elif key_val == KEY_SEMICOLON:
                    command_executor.send_key(command_executor.KEYPRESS.SEMICOLON.value)
                elif key_val == KEY_EQUAL:
                    command_executor.send_key(command_executor.KEYPRESS.EQUAL.value)
                elif key_val == KEY_PERCENT:
                    command_executor.send_key(command_executor.KEYPRESS.PERCENT.value)
                elif key_val == KEY_LESS:
                    command_executor.send_key(command_executor.KEYPRESS.LESS.value)
                elif key_val == KEY_GREATER:
                    command_executor.send_key(command_executor.KEYPRESS.GREATER.value)
                elif key_val == KEY_TILDE:
                    command_executor.send_key(command_executor.KEYPRESS.TILDE.value)
                elif key_val == KEY_QUESTION:
                    command_executor.send_key(command_executor.KEYPRESS.QUESTION.value)
                elif key_val == KEY_UNDERSCORE:
                    command_executor.send_key(command_executor.KEYPRESS.UNDERSCORE.value)
                elif key_val == KEY_EXCLAMATION:
                    command_executor.send_key(command_executor.KEYPRESS.EXCLAMATION.value)
                elif key_val == KEY_COMMA:
                    command_executor.send_key(command_executor.KEYPRESS.COMMA.value)
                elif key_val == KEY_PERIOD:
                    command_executor.send_key(command_executor.KEYPRESS.PERIOD.value)
                elif key_val == KEY_QUOTE:
                    command_executor.send_key(command_executor.KEYPRESS.QUOTE.value)
                elif key_val == KEY_CLOSE:
                    command_executor.execute_cmd(command_executor.Cmd.CLOSE)

            elif cmd_code == CODE_MOUSE:
                # Coordinates should be signed 8 bit numbers. We get unsigned bytes.
                x_offset = rxbuf[1]
                y_offset = rxbuf[2]
                if x_offset > 127:
                    x_offset = (256 - x_offset) * (-1)
                if y_offset > 127:
                    y_offset = (256 - y_offset) * (-1)

                command_executor.move_cursor(x_offset, y_offset)
        except Exception as e:
            exc_type, exc_obj, exc_tb = sys.exc_info()
            print("\nERROR COMMS on line{}: {}".format(exc_tb.tb_lineno, e))


def udp_server():
    print("Starting UDP server\n")

    last_ping_timestamp = 0
    while not stop_flag:
        try:
            (msg, (sender_addr, sender_port)) = udp_receiver.recvfrom(24)

            if msg is not None and len(msg) > 0:
                try:
                    msg_text = msg.decode("utf-8")
                except:
                    msg_text = ""

                if MSG_PING in msg_text:
                    if (time() - last_ping_timestamp) > 0.100:
                        if not send_tcp_response(sender_addr, MAC.encode()):
                            print("ERROR: No TCP connection!")
                        else:
                            print("Responding via TCP with: ", MAC)
                    last_ping_timestamp = time()

                else:
                    udp_rxbuf = []
                    for val in msg:
                        udp_rxbuf.append(val)
                    process_rx(udp_rxbuf)

        except Exception as e:
            # print("ERR: {}".format(e))
            try:
                udp_receiver.close()
            except:
                pass
            try:
                udp_receiver = socket(AF_INET, SOCK_DGRAM)
                udp_receiver.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)
                udp_receiver.settimeout(10)
                udp_receiver.bind(("", UDP_RX_PORT))
            except:
                pass

    print("Ending UDP server\n")


def tcp_server():
    print("Starting TCP receiver\n")
    server = socket(AF_INET, SOCK_STREAM)

    try:
        server.bind((IP, TCP_RX_PORT))
        server.listen(1)

        while not stop_flag:
            # Wait for a connection
            connection, client_address = server.accept()
            try:
                # Receive the data in small chunks and retransmit it
                while not stop_flag:
                    data = connection.recv(5)
                    if data is not None and len(data) > 0:
                        tcp_rxbuf = []
                        for val in data:
                            tcp_rxbuf.append(val)
                        process_rx(tcp_rxbuf)

                    else:
                        break
            except Exception as e:
                exc_type, exc_obj, exc_tb = sys.exc_info()
                print("\nERROR1 on line{}: {}".format(exc_tb.tb_lineno, e))
            finally:
                # Clean up the connection
                connection.close()

    except Exception as e:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        print("\nERROR2 on line{}: {}".format(exc_tb.tb_lineno, e))

    print("Ending TCP receiver\n")


def init(standalone=False):
    global stop_flag
    global IP
    global MAC

    # Setup IP address
    IP = get_ip()
    while IP is None and not stop_flag:
        sleep(5)
        IP = get_ip()

    # Setup MAC address with device ID appendix
    mac = "%012X" % get_mac()
    MAC = ""
    for i in range(0, 10, 2):
        MAC += mac[i: i + 2] + ":"

    MAC += mac[10: 12] + ":" + DEV_ID

    # Activate communication
    t_window_title = threading.Thread(target=thread_window_title)
    t_window_title.start()

    t_udp_server = threading.Thread(target=udp_server)
    t_udp_server.start()

    if standalone:
        tcp_server()
    else:
        t_tcp_server = threading.Thread(target=tcp_server)
        t_tcp_server.start()


def stop():
    global stop_flag

    stop_flag = True


if __name__ == '__main__':
    try:
        init(True)
    except KeyboardInterrupt:
        stop()
        print("Ending TCP receiver\n")
