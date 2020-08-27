#!/usr/bin/env python3

# -*- coding: utf-8 -*-

from time import sleep, time
import sys
import subprocess
from enum import Enum

last_cmd_timestamp = 0


class CmdMod(Enum):
    SHIFT = 1
    ALT = 2
    CTRL = 3


class Cmd(Enum):
    A = 1
    F = 2
    J = 3
    K = 4
    L = 5
    X = 6
    F4 = 7
    SPACEBAR = 8
    LEFT_ARROW = 9
    UP_ARROW = 10
    RIGHT_ARROW = 11
    DOWN_ARROW = 12
    CLOSE = 13
    SHUTDOWN = 14
    STOP = 15
    VOL_UP = 16
    VOL_DOWN = 17
    PREVIOUS = 18
    NEXT = 19
    FORWARD = 20
    REWIND = 21
    PLAY = 22
    MUTE = 23
    MEDIA = 24
    POWEROFF = 25
    NUM_0 = 90
    NUM_1 = 91
    NUM_2 = 92
    NUM_3 = 93
    NUM_4 = 94
    NUM_5 = 95
    NUM_6 = 96
    NUM_7 = 97
    NUM_8 = 98
    NUM_9 = 99
    MOUSE_LEFT = 100
    MOUSE_RIGHT = 101
    MOUSE_UP = 102
    MOUSE_DOWN = 103
    MOUSE_CLICK_LEFT = 104
    MOUSE_CLICK_RIGHT = 105


# def run_process(command_list):
#     print("Execute: ", command_list)
#     try:
#         Popen(command_list)
#     except:
#         pass

def run_process(command_list):
    result = subprocess.run(command_list, stdout=subprocess.PIPE)

    return str(result.stdout, 'utf-8')


if 'linux' in sys.platform:
    class Key(Enum):
        PLAY = "XF86AudioPlay"
        PAUSE = "XF86AudioPause"
        VOLUME_MUTE = "XF86AudioMute"
        STOP = "XF86AudioStop"
        NEXT_TRACK = "XF86AudioNext"
        PREVIOUS_TRACK = "XF86AudioPrev"
        SHUT_DOWN = "XF86Close"
        VOLUME_DOWN = "XF86AudioLowerVolume"
        VOLUME_UP = "XF86AudioRaiseVolume"
        FORWARD = "XF86Forward"
        REWIND = "XF86Rewind"
        F4 = "F4"
        A = "a"
        F = "f"
        J = "j"
        K = "k"
        L = "l"
        X = "x"
        SPACEBAR = "space"
        MEDIA = "XF86AudioMedia"
        SHIFT = 'shift'
        ALT = 'alt'
        CTRL = 'ctrl'

    def send_key(key, modifier=None):
        if modifier == CmdMod.ALT:
            mod_key = Key.ALT + '+'
        elif modifier == CmdMod.SHIFT:
            mod_key = Key.SHIFT + '+'
        elif modifier == CmdMod.CTRL:
            mod_key = Key.CTRL + "+"
        else:
            mod_key = ""

        run_process(["xdotool", mod_key+"key", key])

else:
    # Build for windows. Other platforms not supported.
    from win32api import keybd_event
    from win32con import KEYEVENTF_KEYUP

    class Key(Enum):
        SHIFT = 0x10
        CTRL = 0x11
        ALT = 0x12
        PAUSE = 0x13
        SPACEBAR = 0x20
        PAGE_UP = 0x21
        PAGE_DOWN = 0x22,
        END = 0x23
        HOME = 0x24
        LEFT_ARROW = 0x25
        UP_ARROW = 0x26
        RIGHT_ARROW = 0x27
        DOWN_ARROW = 0x28
        NUM_0 = 0x30
        NUM_1 = 0x31
        NUM_2 = 0x32
        NUM_3 = 0x33
        NUM_4 = 0x34
        NUM_5 = 0x35
        NUM_6 = 0x36
        NUM_7 = 0x37
        NUM_8 = 0x38
        NUM_9 = 0x39
        A = 0x41
        F = 0x46
        J = 0x4A
        K = 0x4B
        L = 0x4C
        X = 0x58
        F4 = 0x73
        BROWSER_BACK = 0xA6
        BROWSER_FORWARD = 0xA7
        BROWSER_REFRESH = 0xA8
        BROWSER_STOP = 0xA9
        BROWSER_SEARCH = 0xAA
        BROWSER_FAVOURITES = 0xAB
        BROWSER_START = 0xAC
        VOLUME_MUTE = 0xAD
        VOLUME_DOWN = 0xAE
        VOLUME_UP = 0xAF
        NEXT_TRACK = 0xB0
        PREVIOUS_TRACK = 0xB1
        STOP = 0xB2
        PLAY_PAUSE = 0xB3
        PLAY = 0xFA
        MEDIA = 0xB5

    def send_key(key, modifier=None):
        if modifier == CmdMod.ALT:
            mod_key = Key.ALT
        elif modifier == CmdMod.SHIFT:
            mod_key = Key.SHIFT
        elif modifier == CmdMod.CTRL:
            mod_key = Key.CTRL
        else:
            mod_key = None

        if mod_key is not None:
            keybd_event(mod_key, 0, 0, 0)

        keybd_event(key, 0, 0, 0)
        sleep(0.05)
        keybd_event(key, 0, KEYEVENTF_KEYUP, 0)

        if modifier is not None:
            keybd_event(mod_key, 0, KEYEVENTF_KEYUP, 0)


def execute_cmd(cmdVal, current_window_title=""):
    global last_cmd_timestamp

    if cmdVal == Cmd.CLOSE:
        send_key(Key.F4, Key.ALT)
    elif cmdVal == Cmd.SHUTDOWN:
        run_process(['poweroff'])
    elif cmdVal == Cmd.STOP:
        send_key(Key.STOP)
    elif cmdVal == Cmd.VOL_UP:
        send_key(Key.VOL_UP)
    elif cmdVal == Cmd.PREVIOUS:
        if "butter" in current_window_title.lower():
            send_key(Key.LEFT_ARROW, Key.SHIFT)
        else:
            send_key(Key.PREVIOUS_TRACK)
    elif cmdVal == Cmd.REWIND:
        if "youtube" in current_window_title.lower():
            send_key(Key.J)
        elif "vlc media player" in current_window_title.lower():
            send_key(Key.LEFT_ARROW, Key.ALT)
        else:
            send_key(Key.LEFT_ARROW)
    elif cmdVal == Cmd.PLAY:
        if "youtube" in current_window_title.lower():
            send_key(Key.K)
        elif "butter" in current_window_title.lower():
            send_key(Key.SPACEBAR)
        else:
            send_key(Key.PLAY)
    elif cmdVal == Cmd.NEXT:
        if "butter" in current_window_title.lower():
            send_key(Key.RIGHT_ARROW, Key.SHIFT)
        else:
            send_key(Key.NEXT_TRACK)
    elif cmdVal == Cmd.FORWARD:
        if "youtube" in current_window_title.lower():
            send_key(Key.L)
        elif "vlc media player" in current_window_title.lower():
            send_key(Key.RIGHT_ARROW, Key.ALT)
        elif "butter" in current_window_title.lower():
            send_key(Key.RIGHT_ARROW)
        else:
            send_key(Key.FORWARD)
    elif cmdVal == Cmd.VOL_DOWN:
        send_key(Key.VOLUME_DOWN)
    elif cmdVal == Cmd.MUTE:
        if (time() - last_cmd_timestamp) > 1:
            send_key(Key.VOLUME_MUTE)
        last_cmd_timestamp = time()
    elif cmdVal == Cmd.MEDIA:
        send_key(Key.MEDIA)
    elif cmdVal == Cmd.POWEROFF:
        run_process(["poweroff"])

    else:
        print("ERROR: Command executor, unknown command: ", cmdVal)

