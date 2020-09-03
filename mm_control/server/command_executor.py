#!/usr/bin/env python3

# -*- coding: utf-8 -*-

from time import sleep, time
import subprocess
from enum import Enum
import sys

last_cmd_timestamp = 0


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
    MOUSE_DOUBLECLICK_LEFT = 110


def run_process(command_list):
    result = subprocess.run(command_list, stdout=subprocess.PIPE)

    return str(result.stdout, 'utf-8')


class KEYPRESS(Enum):
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
    SHIFT_KEY = 'shift'
    ALT_KEY = 'alt'
    CTRL_KEY = 'ctrl'
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

        if modifier != "":
            requested_keypress = "{}+{}".format(modifier, keycode)
        else:
            requested_keypress = keycode

        print("SENDING:", requested_keypress)

        run_process(["xdotool", "key", "{}".format(requested_keypress)])

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
    elif button == Cmd.MOUSE_DOUBLECLICK_LEFT:
        run_process(["xdotool", "click", "1"])
        sleep(0.05)
        run_process(["xdotool", "click", "1"])
    else:
        print("MOUSE UNKNOWN", button)


def execute_cmd(cmdVal, current_window_title="", cursor_offset_x=0, cursor_offset_y=0):
    global last_cmd_timestamp

    try:

        if cmdVal == Cmd.CLOSE:
            send_key(KEYPRESS.F4.value, KEYPRESS.ALT_KEY.value)
        elif cmdVal == Cmd.PLAY:
            if "youtube" in current_window_title.lower():
                send_key(KEYPRESS.K.value)
            elif "butter" in current_window_title.lower():
                send_key(KEYPRESS.SPACEBAR.value)
            else:
                send_key(KEYPRESS.PLAY.value)
        elif cmdVal == Cmd.SHUTDOWN:
            run_process(['poweroff'])
        elif cmdVal == Cmd.STOP:
            send_key(KEYPRESS.STOP.value)
        elif cmdVal == Cmd.VOL_UP:
            send_key(KEYPRESS.VOL_UP.value)
        elif cmdVal == Cmd.PREVIOUS:
            if "butter" in current_window_title.lower():
                send_key(KEYPRESS.LEFT_ARROW.value, KEYPRESS.SHIFT.value)
            else:
                send_key(KEYPRESS.PREVIOUS_TRACK.value)
        elif cmdVal == Cmd.REWIND:
            if "youtube" in current_window_title.lower():
                send_key(KEYPRESS.J.value)
            elif "vlc media player" in current_window_title.lower():
                send_key(KEYPRESS.LEFT_ARROW.value, KEYPRESS.ALT.value)
            else:
                send_key(KEYPRESS.LEFT_ARROW.value)

        elif cmdVal == Cmd.NEXT:
            if "butter" in current_window_title.lower():
                send_key(KEYPRESS.RIGHT_ARROW.value, KEYPRESS.SHIFT.value)
            else:
                send_key(KEYPRESS.NEXT_TRACK.value)
        elif cmdVal == Cmd.FORWARD:
            if "youtube" in current_window_title.lower():
                send_key(KEYPRESS.L.value)
            elif "vlc media player" in current_window_title.lower():
                send_key(KEYPRESS.RIGHT_ARROW.value, KEYPRESS.ALT.value)
            elif "butter" in current_window_title.lower():
                send_key(KEYPRESS.RIGHT_ARROW.value)
            else:
                send_key(KEYPRESS.FORWARD.value)
        elif cmdVal == Cmd.VOL_DOWN:
            send_key(KEYPRESS.VOLUME_DOWN.value)
        elif cmdVal == Cmd.MUTE:
            if (time() - last_cmd_timestamp) > 1:
                send_key(KEYPRESS.VOLUME_MUTE.value)
            last_cmd_timestamp = time()
        elif cmdVal == Cmd.MEDIA:
            send_key(KEYPRESS.MEDIA.value)
        elif cmdVal == Cmd.POWEROFF:
            run_process(["poweroff"])
        else:
            print("ERROR: Command executor, unknown command: ", cmdVal)

    except Exception as e:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        print("\nERROR execute_cmd on line{}: {}".format(exc_tb.tb_lineno, e))

