#!/usr/bin/env python3

from flask import Flask, request, render_template, send_from_directory, redirect, url_for, abort
import os
from socket import socket, AF_INET, SOCK_DGRAM, SOL_SOCKET, SO_BROADCAST, SOCK_STREAM
from uuid import getnode as get_mac
from command_executor import Cmd, CmdMod, execute_cmd, run_process
from tcp_api import init as tcp_init, stop as tcp_stop

WEB_PORT = 8080
SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))
PLS_PATH = os.path.join(os.path.expanduser('~'), '.mpd', 'playlists')
PLS_NAME = 'playlist'

app = Flask(__name__, static_url_path='/assets', static_folder='assets')
app.secret_key = 'OCRadio1303153011SecretKey'

url_list = []
current = 0
CMD_PLAY = "play"
CMD_STOP = "stop"
CMD_PAUSE = "pause"
CMD_CLEAR = "clear"
CMD_ADD = "add"
CMD_NEXT = "next"
CMD_LIST = "playlist"
CMD_DEL = "del"
CMD_CURRENT = "current"
CMD_MOVE = "move"
CMD_SAVE = "save"
CMD_LOAD = "load"

IP = None
port = 0
MSG_PING = "ujagaga ping"
FEATURES = '"MODEL":"Internet Radio Control","CTRL_URL":"/","CTRL_RANGE":"0 - 100"'
stop_flag = False


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


def get_playlist():
    cmd = ['mpc', CMD_LIST]
    ret_val = run_process(cmd)
    if ret_val.endswith('\n'):
        ret_val = ret_val[:-1]
    return ret_val.split('\n')


def sort_playlist():
    # Get playlist
    temp_list = get_playlist()
    url_list = []
    for item in temp_list:
        url_list.append(item.replace('  ', ' '))

    # Get sorted playlist
    sorted_list = url_list.copy()
    sorted_list.sort()

    print(sorted_list)

    # Check actual positions compared to sorted
    for i in range(0, len(sorted_list)):
        current_id = url_list.index(sorted_list[i])

        if current_id != i:
            # move from current_id to position i. Note that mpc playlist index starts from 1.
            cmd = ['mpc', CMD_MOVE, str(current_id + 1), str(i + 1)]
            run_process(cmd)

            temp_list = get_playlist()
            url_list = []
            for item in temp_list:
                url_list.append(item.replace('  ', ' '))


def load_cfg():
    global url_list
    global current

    # Get playlist
    new_url_list = get_playlist()

    # Populate list to display
    url_list = []
    for i in range(0, len(new_url_list)):
        item = new_url_list[i]
        name = item.split(':')[0]
        if name.startswith('http'):
            name = item.split('//')[1].split(':')[0].upper().replace('STREAMING', '').replace('STREAM', '').replace('..', '.')
            if name.startswith('.'):
                name = name[1:]

        data = {"name": name, "href": item, "id": len(url_list) + 1}
        url_list.append(data)

    # get currently playing
    current = 0
    cmd = ['mpc']
    status = run_process(cmd).split('\n')

    for i in range(0, len(status)):
        if '[playing]' in status[i]:
            current = int(status[i].split('#')[1].split('/')[0])


@app.route('/', methods=['GET', 'POST'])
def home():
    global current

    print('request:', request)

    load_cfg()

    action = request.args.get('action', '')
    id = int(request.args.get('id', '0'))

    if action == 'add':
        url = request.args.get('url', '').replace('"', '')

        if url != '':
            playlist = get_playlist()
            print('ADD:', url)
            print(playlist)

            if url not in playlist:
                cmd = ['mpc', CMD_ADD, url]
                run_process(cmd)
                load_cfg()

    elif action == 'del':
        if (id > 0) and (id <= len(url_list)):

            cmd = ['mpc', CMD_DEL, str(id)]
            run_process(cmd)

            load_cfg()

    elif action == 'play':
        if (id > 0) and (id <= len(url_list)):

            cmd = ['mpc', CMD_PLAY, str(id)]
            run_process(cmd)

    elif action == 'stop':
        current = 0
        cmd = ['mpc', CMD_STOP]
        run_process(cmd)

    elif action == 'vol_up':
        execute_cmd(Cmd.VOL_UP)

    elif action == 'vol_dn':
        execute_cmd(Cmd.VOL_DOWN)

    elif action == 'mm_play':
        execute_cmd(Cmd.VOL_UP)

    elif action == 'mm_stop':
        execute_cmd(Cmd.STOP)

    elif action == 'sort':
        sort_playlist()
        load_cfg()

    else:
        cur_cmd = ['mpc', CMD_CURRENT]
        current_text = run_process(cur_cmd)
        print(current_text)
        if len(current_text) > 2:
            try:
                song_title = current_text.split(':')[1]
            except:
                song_title = current_text
        else:
            song_title = ''

        ipaddress = 'http://' + get_ip()

        if WEB_PORT != 80:
            ipaddress += ':{}'.format(WEB_PORT)
        return render_template('index.html', stream_list=url_list, current=current, song_title=song_title, ipaddress=ipaddress)

    return redirect(url_for('home'))


@app.route('/uploadpls', methods=['POST'])
def uploadpls():
    f = request.files['file']
    if f:
        content = f.read().decode('utf-8')

        url_list = []
        for line in content.split('\n'):
            if line.startswith('http'):
                url_list.append(line)

        if len(url_list) > 0:
            # Clear current playlist
            cmd = ['mpc', CMD_CLEAR]
            run_process(cmd)

            for url in url_list:
                cmd = ['mpc', CMD_ADD, url]
                run_process(cmd)

            cmd = ['mpc', CMD_PLAY, str(len(url_list) + 1)]
            run_process(cmd)

            load_cfg()

    return redirect(url_for('home'))


@app.route('/downloadpls', methods=['GET'])
def downloadpls():
    file_path = os.path.join(PLS_PATH, PLS_NAME + '.m3u')

    if os.path.isfile(file_path):
        os.remove(file_path)

    # Save current playlist
    cmd = ['mpc', CMD_SAVE, PLS_NAME]
    run_process(cmd)

    if os.path.isfile(file_path):
        return send_from_directory(PLS_PATH, PLS_NAME + '.m3u')
    else:
        print('Not found:', os.path.join(PLS_PATH, PLS_NAME + '.m3u'))
        abort(404)


@app.route('/savepls', methods=['GET'])
def savepls():
    file_path = os.path.join(PLS_PATH, PLS_NAME + '.m3u')

    if os.path.isfile(file_path):
        os.remove(file_path)

    # Save current playlist
    cmd = ['mpc', CMD_SAVE, PLS_NAME]
    run_process(cmd)

    load_cfg()

    return redirect(url_for('home'))


@app.route('/favicon.ico')
def favicon():
    return send_from_directory(SCRIPT_PATH, 'assets/favicon.ico', mimetype='image/vnd.microsoft.icon')


@app.route('/id')
def get_id():
    print('id')
    cur_cmd = ['mpc', CMD_CURRENT]
    current_text = run_process(cur_cmd)
    if len(current_text) > 2:
        try:
            song_title = current_text.split(':')[1]
        except:
            song_title = current_text
    else:
        song_title = ''

    mac = "%012X" % get_mac()
    MAC = ""
    for i in range(0, 10, 2):
        MAC += mac[i: i + 2] + ":"

    MAC += mac[10: 12]

    return '{"MAC":"' + MAC + '",' + FEATURES + ',"CURRENT":"' + song_title + '"}'


if __name__ == '__main__':
    cmd = ['mpc', CMD_CLEAR]
    run_process(cmd)
    cmd = ['mpc', CMD_LOAD, PLS_NAME]
    run_process(cmd)

    load_cfg()
    tcp_init()

    app.run('0.0.0.0', WEB_PORT, threaded=True, debug=False)
    tcp_stop()
    stop_flag = True

