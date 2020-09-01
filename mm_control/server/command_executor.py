#!/usr/bin/env python3

# -*- coding: utf-8 -*-

from time import sleep, time
import subprocess
from enum import Enum
import sys

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
    MOUSE_DOWN_LEFT = 106
    MOUSE_DOWN_RIGHT = 107
    MOUSE_UP_LEFT = 108
    MOUSE_UP_RIGHT = 109


def run_process(command_list):
    result = subprocess.run(command_list, stdout=subprocess.PIPE)

    return str(result.stdout, 'utf-8')


class Keypress(Enum):
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
    BACKSPACE = "BackSpace"
    MEDIA = "XF86AudioMedia"
    SHIFT = 'shift'
    ALT = 'alt'
    CTRL = 'ctrl'
    ARROW_LEFT = "Left"
    ARROW_RIGHT = "Right"
    ARROW_UP = "Up"
    ARROW_DOWN = "Down"
    CAPS_LOCK = "Caps_Lock"
    PAGE_UP = "Page_Up"
    PAGE_DOWN = "Page_Down"
    ENTER = "Return"
    MONKEY = "at"
    PLUS = "plus"
    MINUS = "minus"
    SLASH = "slash"
    PERCENT = "percent"
    LESS = "less"
    GREATER = "greater"
    COLON = "colon"
    SEMICOLON = "semicolon"
    TILDE = "asciitilde"
    QUESTION = "question"
    UNDERSCORE = "underscore"
    EXCLAMATION = "exclam"
    COMMA = "comma"
    PERIOD = "period"
    QUOTE = "quotedbl"
    EQUAL = "equal"


def send_key(keycode, modifier=""):
    try:
        if modifier == CmdMod.ALT:
            mod_key = "{}+".format(Keypress.ALT)
        elif modifier == CmdMod.SHIFT:
            mod_key = "{}+".format(Keypress.SHIFT)
        elif modifier == CmdMod.CTRL:
            mod_key = "{}+".format(Keypress.CTRL)
        else:
            mod_key = ""

        print("SENDING:", mod_key, keycode)

        run_process(["xdotool", "key", "{}{}".format(mod_key, keycode)])

    except Exception as e:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        print("\nERROR CMD_EXEC on line{}: {}".format(exc_tb.tb_lineno, e))


def move_cursor(x_offset, y_offset):
    result = run_process(["xdotool", "getmouselocation"])
    try:
        data = result.split(' ')
        current_x = data[0].split(':')[1]
        current_y = data[1].split(':')[1]

        target_x = int(current_x) + x_offset
        target_y = int(current_y) + y_offset

        run_process(["xdotool", "mousemove", str(target_x), str(target_y)])

    except Exception as e:
        pass


def mouse_click(button):
    if button == Cmd.MOUSE_DOWN_RIGHT:
        run_process(["xdotool", "mousedown", "3"])
    elif button == Cmd.MOUSE_UP_RIGHT:
        run_process(["xdotool", "mouseup", "3"])
    elif button == Cmd.MOUSE_DOWN_LEFT:
        run_process(["xdotool", "mousedown", "1"])
    elif button == Cmd.MOUSE_UP_LEFT:
        run_process(["xdotool", "mouseup", "1"])
    elif button == Cmd.MOUSE_CLICK_LEFT:
        run_process(["xdotool", "click", "1"])
    elif button == Cmd.MOUSE_CLICK_RIGHT:
        run_process(["xdotool", "click", "3"])


def execute_cmd(cmdVal, current_window_title="", cursor_offset_x=0, cursor_offset_y=0):
    global last_cmd_timestamp

    if cmdVal == Cmd.CLOSE:
        send_key(Keypress.F4, Keypress.ALT)
    elif cmdVal == Cmd.SHUTDOWN:
        run_process(['poweroff'])
    elif cmdVal == Cmd.STOP:
        send_key(Keypress.STOP)
    elif cmdVal == Cmd.VOL_UP:
        send_key(Keypress.VOL_UP)
    elif cmdVal == Cmd.PREVIOUS:
        if "butter" in current_window_title.lower():
            send_key(Keypress.LEFT_ARROW, Keypress.SHIFT)
        else:
            send_key(Keypress.PREVIOUS_TRACK)
    elif cmdVal == Cmd.REWIND:
        if "youtube" in current_window_title.lower():
            send_key(Keypress.J)
        elif "vlc media player" in current_window_title.lower():
            send_key(Keypress.LEFT_ARROW, Keypress.ALT)
        else:
            send_key(Keypress.LEFT_ARROW)
    elif cmdVal == Cmd.PLAY:
        if "youtube" in current_window_title.lower():
            send_key(Keypress.K.value)
        elif "butter" in current_window_title.lower():
            send_key(Keypress.SPACEBAR.value)
        else:
            send_key(Keypress.PLAY.value)
    elif cmdVal == Cmd.NEXT:
        if "butter" in current_window_title.lower():
            send_key(Keypress.RIGHT_ARROW, Keypress.SHIFT)
        else:
            send_key(Keypress.NEXT_TRACK)
    elif cmdVal == Cmd.FORWARD:
        if "youtube" in current_window_title.lower():
            send_key(Keypress.L)
        elif "vlc media player" in current_window_title.lower():
            send_key(Keypress.RIGHT_ARROW, Keypress.ALT)
        elif "butter" in current_window_title.lower():
            send_key(Keypress.RIGHT_ARROW)
        else:
            send_key(Keypress.FORWARD)
    elif cmdVal == Cmd.VOL_DOWN:
        send_key(Keypress.VOLUME_DOWN)
    elif cmdVal == Cmd.MUTE:
        if (time() - last_cmd_timestamp) > 1:
            send_key(Keypress.VOLUME_MUTE)
        last_cmd_timestamp = time()
    elif cmdVal == Cmd.MEDIA:
        send_key(Keypress.MEDIA)
    elif cmdVal == Cmd.POWEROFF:
        run_process(["poweroff"])


    else:
        print("ERROR: Command executor, unknown command: ", cmdVal)

