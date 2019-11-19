#!/usr/bin/env python3

# -*- coding: utf-8 -*-
'''
Written for python3
'''

from tkinter import Tk, LEFT, Label, PhotoImage, BOTTOM, Button, Frame, X
from icon_data import icon_image, power_image , chat_image
from socket import socket, AF_INET, SOCK_DGRAM, SOL_SOCKET, SO_BROADCAST, SOCK_STREAM
from uuid import getnode as get_mac
import threading
from time import sleep, time
from http.server import BaseHTTPRequestHandler, HTTPServer
import sys
from subprocess import Popen, CalledProcessError, PIPE
from os import path, system, walk, makedirs
from json import dumps as strEscape


if 'linux' in sys.platform:
    OS_PLATFORM = 'linux'
else:
    OS_PLATFORM = 'windows'


def cleanup_string(msg):
    esc_seq = [['%20', ' '], ['%9E', 'ž'], ['%8E', 'Ž'], ['%9A', 'š'], ['%8A', 'Š'], ['%26%23269%3B', 'č'],
               ['%26%23268%3B', 'Č'], ['%26%23263%3B', 'ć'], ['%26%23273%3B', 'đ'], ['%26%23272%3B', 'Đ'],
               ['%26%23262%3B', 'Ć'], ['%27', "'"], ['%22', '"']]

    for c in esc_seq:
        while c[0] in msg:
            msg = msg.replace(c[0], c[1])

    return msg


if 'linux' in OS_PLATFORM:
    PLAY = "XF86AudioPlay"
    PAUSE = "XF86AudioPause"
    MUTE = "XF86AudioMute"
    STOP = "XF86AudioStop"
    NEXT = "XF86AudioNext"
    PREV = "XF86AudioPrev"
    SHUT_DOWN = "XF86Close"
    VOL_DOWN = "XF86AudioLowerVolume"
    VOL_UP = "XF86AudioRaiseVolume"
    FORWARD = "XF86Forward"
    REWIND = "XF86Rewind"
    CLOSE = "alt+F4"
    SPACE = "space"
    MEDIA = "XF86AudioMedia"


elif 'windows' in OS_PLATFORM:
    from win32gui import GetWindowText, GetForegroundWindow
    from win32api import keybd_event
    from winsound import PlaySound, SND_FILENAME
    from win32con import KEYEVENTF_KEYUP

    winKeyCodes = {'shift': 0x10,
                   'ctrl': 0x11,
                   'alt': 0x12,
                   'pause': 0x13,
                   'spacebar': 0x20,
                   'page_up': 0x21,
                   'page_down': 0x22,
                   'end': 0x23,
                   'home': 0x24,
                   'left_arrow': 0x25,
                   'up_arrow': 0x26,
                   'right_arrow': 0x27,
                   'down_arrow': 0x28,
                   '0': 0x30,
                   '9': 0x39,
                   'a': 0x41,
                   'f': 0x46,
                   'j': 0x4A,
                   'k': 0x4B,
                   'l': 0x4C,
                   'x': 0x58,
                   'F4': 0x73,
                   'browser_back': 0xA6,
                   'browser_forward': 0xA7,
                   'browser_refresh': 0xA8,
                   'browser_stop': 0xA9,
                   'browser_search': 0xAA,
                   'browser_favorites': 0xAB,
                   'browser_start_and_home': 0xAC,
                   'volume_mute': 0xAD,
                   'volume_down': 0xAE,
                   'volume_up': 0xAF,
                   'next_track': 0xB0,
                   'previous_track': 0xB1,
                   'stop_media': 0xB2,
                   'play/pause_media': 0xB3,
                   'play_key': 0xFA,
                   'media_key': 0xB5
                   }

    PLAY = "play/pause_media"
    STOP = "stop_media"
    NEXT = "next_track"
    PREV = "previous_track"
    VOL_DOWN = "volume_down"
    VOL_UP = "volume_up"
    SPACE = "spacebar"
    FORWARD = 'browser_forward'
    REWIND = 'browser_back'
    MUTE = 'volume_mute'
    MEDIA = 'media_key'


BROADCAST_PORT = 4210
IP = None
port = 0
stop_flag = False
MAC = ""
current_window_title = ""
flag_shutdown_init = False
APP_TITLE = "Lazy Control"
MSG_PING = "ujagaga ping"
last_shutdown_timestamp = 0
last_mute_timestamp = 0
last_ping_timestamp = 0
FEATURES = '"MODEL":"Lazy Control","CTRL_URL":"/ctrl?CMD=","CTRL_RANGE":"0 - 100"'
messages = []
current_skin = "default"

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

webio_path = current_path + "/skins/" + current_skin + "/"
cfg_path = path.expanduser("~") + "/.lazyctrl"
cfg_name = "config.txt"


def run_async_process(command_list):
    print("Execute: ", command_list)
    try:
        Popen(command_list)
    except:
        pass


if 'linux' in OS_PLATFORM:
    def send_key(key):
        run_async_process(["xdotool", "key", key])

    def get_active_window_name():
        try:
            return Popen(["xdotool", "getwindowfocus", "getwindowname"], stdout=PIPE).communicate()[0].decode()
        except CalledProcessError as e:
            return ""

elif 'windows' in OS_PLATFORM:
    def send_key(key, modifier=''):
        global winKeyCodes

        key_virtual_value = winKeyCodes[key]

        if modifier != '':
            keybd_event(winKeyCodes[modifier], 0, 0, 0)
        keybd_event(key_virtual_value, 0, 0, 0)
        sleep(0.05)
        keybd_event(key_virtual_value, 0, KEYEVENTF_KEYUP, 0)

        if modifier != '':
            keybd_event(winKeyCodes[modifier], 0, KEYEVENTF_KEYUP, 0)


    def get_active_window_name():
        return GetWindowText(GetForegroundWindow())
else:
    def send_key(key=""):
        print("MAC OSX is not yet supported")

    def get_active_window_name():
        # MAC OSX is not yet supported
        return " "


class InfoBox(Tk):
    def __init__(self, msg_text):
        global APP_TITLE

        Tk.__init__(self)

        self.call('wm', 'iconphoto', self._w, PhotoImage(data=icon_image))
        self.attributes("-topmost", True)

        self.title(APP_TITLE)
        self.minsize(300, 50)

        self.image = PhotoImage(data=chat_image)
        Label(self, image=self.image).pack(side=LEFT, padx=10, pady=5)

        Label(self, text=msg_text, font=("Courier", 30, "bold"), wraplength=600).pack()

        Button(self, text="Dismiss", command=self.dismiss, width=10).pack(side=BOTTOM)

        self.focus_force()

    def dismiss(self, event=None):
        self.wm_withdraw()
        self.destroy()


class CountDownBox(Tk):
    global flag_shutdown_init
    global OS_PLATFORM

    def __init__(self):
        Tk.__init__(self)

        self.count = 15

        self.title_string = 'Shutdown in %d s' % self.count
        self.title(self.title_string)
        self.wm_title(self.title_string)
        self.minsize(300, 50)
        self.call('wm', 'iconphoto', self._w, PhotoImage(data=icon_image))
        self.attributes("-topmost", True)

        self.frame1 = Frame(self)
        self.frame1.pack(pady=0, padx=0, fill=X)

        if 'linux' in OS_PLATFORM:
            self.image = PhotoImage(data=power_image)
            Label(self.frame1, image=self.image).pack(side=LEFT, padx=10, pady=5)

        Label(self.frame1, text="Lazyctrl server initiated system shutdown.", justify=LEFT).pack(padx=5)

        self.count_down_label = Label(self.frame1, text="", justify=LEFT)
        self.count_down_label.pack()

        self.update_clock()

        self.focus_force()

    def update_clock(self):

        if flag_shutdown_init:

            self.count_down_label.configure(text="Seconds left: %d \n close to stop!" % self.count)
            self.after(1000, self.update_clock)
            self.count -= 1

            self.title_string = 'Shutdown in %d s' % self.count
            self.title(self.title_string)
            self.wm_title(self.title_string)

            if self.count == 0:
                if OS_PLATFORM == 'linux':
                    system("shutdown -h now")
                else:
                    system("shutdown.exe -f -s -t 0")
                self.destroy()

        else:
            self.wm_withdraw()
            self.destroy()


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


def change_skin():
    global current_skin
    global webio_path
    global current_path
    global cfg_path
    global cfg_name

    currentSkinFoundFlag = False
    skinChanged = False

    for dirname, dirnames, filenames in walk(current_path + "/skins/"):
        # print path to all subdirectories first.
        if len(dirnames) > 0:
            for i in range(0, len(dirnames)):
                skin = dirnames[i]
                if skin == current_skin:
                    currentSkinFoundFlag = True
                else:
                    if currentSkinFoundFlag:
                        current_skin = skin
                        skinChanged = True
                        break

            if not skinChanged:
                current_skin = dirnames[0]
            break

    webio_path = current_path + "/skins/" + current_skin + "/"
    print("Setting skin: ", current_skin)

    if not path.exists(cfg_path):
        makedirs(cfg_path)

    cfgfile = open(cfg_path + "/" + cfg_name, 'w+')

    cfgfile.write("skin=" + current_skin)


def execute_cmd(cmdVal):
    global OS_PLATFORM
    global flag_shutdown_init
    global last_shutdown_timestamp
    global last_mute_timestamp
    global current_window_title

    result = 'OK'

    if cmdVal == 1:
        print("p window close")
        if OS_PLATFORM == 'linux':
            send_key(CLOSE)
        else:
            send_key('F4', 'alt')
    elif cmdVal == 2:
        print("Lp Shutdown")

        if flag_shutdown_init:
            timeout = 1
        else:
            timeout = 3

        if (time() - last_shutdown_timestamp) > timeout:
            flag_shutdown_init = not flag_shutdown_init

        last_shutdown_timestamp = time()

    elif cmdVal == 3:
        print("p stop")
        send_key(STOP)
    elif cmdVal == 4:
        print("lp stop")
        send_key(STOP)
    elif cmdVal == 5:
        print("p vol up")
        send_key(VOL_UP)
    elif cmdVal == 6:
        print("lp vol up")
        send_key(VOL_UP)
    elif cmdVal == 7:
        print("p prew")
        if "butter" in current_window_title.lower():
            send_key("shift+Left")
        else:
            send_key(PREV)
    elif cmdVal == 8:
        print("lp rew")
        if "youtube" in current_window_title.lower():
            send_key("j")
        elif "vlc media player" in current_window_title.lower():
            if OS_PLATFORM == 'linux':
                send_key("alt+Left")
            else:
                send_key("left_arrow", 'alt')
        elif "butter" in current_window_title.lower():
            send_key("Left")
        else:
            send_key(REWIND)
    elif cmdVal == 9:
        print("p play/pause")
        if "youtube" in current_window_title.lower():
            send_key("k")
        elif "butter" in current_window_title.lower():
            send_key(SPACE)
        else:
            send_key(PLAY)
    elif cmdVal == 10:
        print("lp play/pause")
        send_key(SPACE)
    elif cmdVal == 11:
        print("p next")
        if "butter" in current_window_title.lower():
            send_key("shift+Right")
        else:
            send_key(NEXT)
    elif cmdVal == 12:
        print("lp ffwd")
        if "youtube" in current_window_title.lower():
            send_key("l")
        elif "vlc media player" in current_window_title.lower():
            if OS_PLATFORM == 'linux':
                send_key("alt+Right")
            else:
                send_key("right_arrow", 'alt')
        elif "butter" in current_window_title.lower():
            send_key("Right")
        else:
            send_key(FORWARD)
    elif cmdVal == 13:
        print("p vol down")
        send_key(VOL_DOWN)
    elif cmdVal == 14:
        print("lp vol down")

        if (time() - last_mute_timestamp) > 1:
            send_key(MUTE)
        last_mute_timestamp = time()
    elif cmdVal == 15:
        print("p media")
        send_key(MEDIA)
    elif cmdVal == 16:
        print("lp media")
        send_key(MEDIA)
    else:
        print("Unknown: ", cmdVal)
        result = "Unknown cmd: %d" % cmdVal

    return result


class MyServer(BaseHTTPRequestHandler):
    def _set_headers(self, header_type='text/html'):
        self.send_response(200)
        self.send_header('Content-type', header_type)
        self.end_headers()

    def sendMsg(self, message):
        self._set_headers(header_type='text/plain')
        self.wfile.write(bytes(message, "utf-8"))

    def do_GET(self):
        global FEATURES
        global MAC
        global current_window_title
        global extension
        global current_path
        global messages

        request = self.path

        if request.split("?")[0] == "/ctrl":    # CTRL request
            cmd = request.split("?")[1]
            cmdVal = -1

            if cmd.startswith("CMD="):
                cmdVal = int(cmd.split("=")[1])

            self.sendMsg(execute_cmd(cmdVal))

        elif request == "/id":  # ID request
            self.sendMsg('{"MAC":"' + MAC + '",' + FEATURES + ',"CURRENT":' + current_window_title + '}')

        elif request == "/":    # General request
            INDEX_PATH = webio_path + "index.html"
            if path.exists(INDEX_PATH):
                try:
                    file = open(INDEX_PATH, "rb")
                    self._set_headers()
                    self.wfile.write(file.read())
                    file.close()
                except IOError:
                    self.send_error(404, 'File Not Found: %s' % self.path)

        elif request == "/change_skin":
            change_skin()
            self.sendMsg("OK")

        elif request.split("?")[0] == "/msg":    # General request
            msgText = cleanup_string(request.split("?")[1])

            msgText = msgText.split("&")

            msg_box_text = ""
            for param in msgText:
                key_val = param.split("=")
                if len(key_val) == 1:
                    msg_box_text = key_val[0]
                    break
                else:
                    if "from" in key_val[0]:
                        msg_box_text = "Message from: " + key_val[1] + "\n\n" + msg_box_text
                    if "msg" in key_val[0]:
                        msg_box_text += key_val[1]

            print("MSG: ", msg_box_text)

            messages += [msg_box_text]

            self.sendMsg("OK")
        elif ".." not in request:
            if path.exists(webio_path + request):
                try:
                    file = open(webio_path + request, "rb")

                    if request.endswith(".css"):
                        self._set_headers(header_type='text/css')
                    elif request.endswith(".html"):
                        self._set_headers(header_type='text/html')
                    elif request.endswith(".js"):
                        self._set_headers(header_type='text/javascript')
                    else:
                        self._set_headers()

                    self.wfile.write(file.read())
                    file.close()
                except IOError:
                    self.send_error(404, 'File Not Found: %s' % self.path)

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


def thread_message_box():
    global messages

    while not stop_flag:
        if len(messages) > 0:
            msg_text = messages.pop(0)
            box = InfoBox(msg_text)
            box.mainloop()


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
                    print("Responding via UDP with: ", MAC)
                    broadcast_receiver.sendto(MAC.encode(), (sender_addr, sender_port))
                else:
                    print("Responding via TCP with: ", MAC)

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


IP = get_ip()
while IP is None:
    sleep(5)
    IP = get_ip()

print("Running on IP: ", IP, ":", BROADCAST_PORT)

# read config
try:
    configfile = open(cfg_path + "/" + cfg_name, 'r')
    content = configfile.readlines()[0]
    skin_to_set = content.split("=")[1]

    if path.exists(current_path + "/skins/" + skin_to_set):
        current_skin = skin_to_set
        webio_path = current_path + "/skins/" + current_skin + "/"
        print("Skin:", current_skin)
except:
    pass

t_beacon = threading.Thread(target=thread_beacon)
t_beacon.start()

t_window_title = threading.Thread(target=thread_window_title)
t_window_title.start()

t_countdown = threading.Thread(target=thread_countdown)
t_countdown.start()

t_msgbox = threading.Thread(target=thread_message_box)
t_msgbox.start()

try:
    httpd = HTTPServer(('', BROADCAST_PORT + 1), MyServer)
    print('Starting httpd...')
except:
    stop_flag = True
    sys.exit("Port %d unavailable" % BROADCAST_PORT)

try:
    httpd.serve_forever()
except KeyboardInterrupt:
    pass

stop_flag = True
httpd.server_close()
print("Closed http server")
