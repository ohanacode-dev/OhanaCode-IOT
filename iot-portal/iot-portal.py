#!/usr/bin/python3

import json
import os
import sys
import sqlite3
import paho.mqtt.client as mqtt
from fastapi import FastAPI, Request, WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse, FileResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
from typing import List
from websocket import create_connection as ws_client_connection
import uvicorn


MQTT_RX_TOPIC = "device_status"
MQTT_SERVER_ADDR = "localhost"
MQTT_SERVER_PORT = 1883
DATABASE = ''
LABELBASE = ''
WEB_PORT = 8000


class MqttStatusReceiver:
    def __init__(self, host=MQTT_SERVER_ADDR, port=MQTT_SERVER_PORT, topic=MQTT_RX_TOPIC):
        self.host = host
        self.port = port
        self.topic = topic
        self.rx_msg = None

        self.client = mqtt.Client('IOT_Portal')
        try:
            self.client.connect(host, port=port)
            self.client.on_connect = self.on_connect
            self.client.on_message = self.on_message
            self.client.loop_start()
        except:
            pass

    def on_connect(self, client, userdata, flags, rc):
        client.subscribe(self.topic)

    def on_message(self, client, userdata, msg):
        self.rx_msg = msg.payload.decode()
        print("MSG:", self.rx_msg)
        if self.rx_msg is not None:
            write_status_to_db(self.rx_msg)

    def stop(self):
        self.client.disconnect()
        self.client.loop_stop()


class WsConnectionManager:
    def __init__(self):
        self.active_connections: List[WebSocket] = []

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.append(websocket)

    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)

    async def send_text(self, message: str, websocket: WebSocket):
        await websocket.send_text(message)

    async def broadcast(self, message: str):
        for connection in self.active_connections:
            await connection.send_text(message)


def mqttSend(device_mac, message, host=MQTT_SERVER_ADDR, port=MQTT_SERVER_PORT):
    client = mqtt.Client('Tx')
    client.connect(host, port=port)
    client.publish(device_mac, message)


def init_database():
    global DATABASE
    global LABELBASE

    # Setup main database
    db_dir = '/tmp/.iot-portal'
    if not os.path.isdir(db_dir):
        os.mkdir(db_dir)

    DATABASE = os.path.join(db_dir, "database.db")
    if os.path.exists(DATABASE):
        os.remove(DATABASE)

    # Create an empty database
    db = sqlite3.connect(DATABASE)

    # Create a status table
    sql = "create table status (id INTEGER PRIMARY KEY AUTOINCREMENT, mac TEXT, data TEXT)"
    db.execute(sql)
    db.commit()

    db.close()

    # Setup label database
    db_dir = os.path.join( os.path.expanduser('~'), '.iot-portal')
    if not os.path.isdir(db_dir):
        os.mkdir(db_dir)

    LABELBASE = os.path.join(db_dir, "database.db")

    if not os.path.exists(LABELBASE):
        # Create an empty database
        db = sqlite3.connect(LABELBASE)

        # Create a label table
        sql = "create table label (id INTEGER PRIMARY KEY AUTOINCREMENT, mac TEXT, label TEXT)"
        db.execute(sql)
        db.commit()

        db.close()


def query_db(db, query, args=(), one=False):
    cur = db.execute(query, args)
    rv = [dict((cur.description[idx][0], value)
               for idx, value in enumerate(row)) for row in cur.fetchall()]
    return (rv[0] if rv else None) if one else rv


def write_status_to_db(msg):
    try:
        data = json.loads(msg)
        data['label'] = get_device_label(data['mac'])
        mac = data['mac']
        data.pop('mac', None)       # Remove redundant mac info

        db = sqlite3.connect(DATABASE)

        # Get this device data if exists
        sql = "SELECT * FROM status WHERE mac='{}'".format(mac)
        result = query_db(db, sql, one=True)

        if result is None:
            sql = "INSERT INTO status (mac, data) VALUES ('{}', '{}')".format(mac, json.dumps(data))
        else:
            sql = "UPDATE status SET data='{}' WHERE mac='{}'".format(json.dumps(data), mac)

        db.execute(sql)
        db.commit()
        db.close()

        report_device_status_change(mac)

    except Exception as exc:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        print("ERROR writing status to db on line {}!\n\t{}".format(exc_tb.tb_lineno, exc))


def get_devices_from_db(device_mac=None, id=None):

    sql = "SELECT * FROM status"

    get_one = False
    if device_mac is not None:
        sql += " WHERE mac='{}'".format(device_mac)
        get_one = True
    elif id is not None:
        sql += " WHERE id='{}'".format(id)
        get_one = True

    db = sqlite3.connect(DATABASE)
    result = query_db(db, sql, one=get_one)
    db.close()

    return result


def remove_device_from_db(device_mac=None, id=None):

    sql = "DELETE FROM status"

    if device_mac is not None:
        sql += " WHERE mac='{}'".format(device_mac)
    elif id is not None:
        sql += " WHERE id='{}'".format(id)

    db = sqlite3.connect(DATABASE)
    db.execute(sql)
    db.commit()
    db.close()


def set_device_label(device_mac, label):
    try:
        db = sqlite3.connect(LABELBASE)

        if len(label) == 0:
            sql = "DELETE FROM label WHERE mac='{}'".format(device_mac)
        else:
            # Get this device data if exists
            sql = "SELECT * FROM label WHERE mac='{}'".format(device_mac)
            result = query_db(db, sql, one=True)

            if result is None:
                sql = "INSERT INTO label (mac, label) VALUES ('{}', '{}')".format(device_mac, label)
            else:
                sql = "UPDATE label SET label='{}' WHERE mac='{}'".format(label, device_mac)

        db.execute(sql)
        db.commit()
        db.close()

    except Exception as exc:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        print("ERROR writing label to db on line {}!\n\t{}".format(exc_tb.tb_lineno, exc))


def report_device_status_change(device_mac):
    try:
        device = get_devices_from_db(device_mac=device_mac)
        msg = {'topic': "notify_device_status", 'id': device['id'], 'data': device}

        ws = ws_client_connection("ws://localhost:{}/devices".format(WEB_PORT))
        ws.send(json.dumps(msg))
        result = ws.recv()
        ws.close()
    except Exception as exc:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        print("ERROR repporting status via WS on line {}!\n\t{}".format(exc_tb.tb_lineno, exc))


init_database()
# Declare and start the mqtt status receiver
receiver = MqttStatusReceiver()

app = FastAPI()
app.mount("/static", StaticFiles(directory="static"), name="static")
templates = Jinja2Templates(directory="templates")
ws_manager = WsConnectionManager()


def get_device_label(device_mac):
    result = device_mac

    try:
        if device_mac is not None:
            sql = "SELECT * FROM label WHERE mac='{}'".format(device_mac)

            db = sqlite3.connect(LABELBASE)
            data = query_db(db, sql, one=True)
            db.close()

            if data is not None:
                result = data.get('label', device_mac)

    except Exception as exc:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        print("ERROR reading label on line {}!\n\t{}".format(exc_tb.tb_lineno, exc))

    return result


@app.get("/", response_class=HTMLResponse)
def home(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})


@app.get('/favicon.ico')
async def favicon():
    file_name = "favicon.ico"
    file_path = os.path.join(app.root_path, "static", file_name)
    return FileResponse(path=file_path, headers={"Content-Disposition": "attachment; filename=" + file_name})


@app.websocket("/devices")
async def websocket_server(websocket: WebSocket):
    await ws_manager.connect(websocket)
    try:
        while True:
            msg = await websocket.receive_text()
            data = json.loads(msg)

            if data['topic'] == "device_list":
                devices = get_devices_from_db()

                # Update labels
                for i in range(0, len(devices)):
                    data = json.loads(devices[i]['data'])
                    data['label'] = get_device_label(devices[i]['mac'])
                    devices[i]['data'] = json.dumps(data)

                response = {'topic': 'device_list', 'data': devices}

                await ws_manager.send_text(json.dumps(response), websocket)

            if data['topic'] == "remove_device":
                id = data.get('id', None)
                mac = data.get('mac', None)

                if id is not None:
                    device = get_devices_from_db(id=id)
                elif mac is not None:
                    device = get_devices_from_db(device_mac=mac)
                else:
                    device = None

                remove_device_from_db(device["id"])

                devices = get_devices_from_db()

                # Update labels
                for i in range(0, len(devices)):
                    data = json.loads(devices[i]['data'])
                    data['label'] = get_device_label(devices[i]['mac'])
                    devices[i]['data'] = json.dumps(data)

                response = {'topic': 'device_list', 'data': devices}

                await ws_manager.send_text(json.dumps(response), websocket)

            elif data['topic'] == "set_device":
                id = data.get('id', None)
                mac = data.get('mac', None)
                new_device_data = data.get('data', {})

                if id is not None:
                    device = get_devices_from_db(id=id)
                elif mac is not None:
                    device = get_devices_from_db(device_mac=mac)
                else:
                    device = None

                if device is not None:
                    device_db_data = json.loads(device['data'])

                    for key in device_db_data.keys():
                        if key in new_device_data:
                            if key == 'label':
                                if device_db_data[key] != new_device_data[key]:
                                    set_device_label(device['mac'], new_device_data[key])

                            device_db_data[key] = new_device_data[key]

                    # device['data'] = json.dumps(device_db_data)
                    # Send updated data to device. Device will report the change which will be propagated to the UI.
                    mqttSend(device['mac'], str(new_device_data))

            elif data['topic'] == "notify_device_status":
                id = data.get('id', None)
                device = get_devices_from_db(id=id)
                response = {'topic': 'set_device', 'data': device}
                await ws_manager.broadcast(json.dumps(response))

    except WebSocketDisconnect:
        ws_manager.disconnect(websocket)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        arg_port = sys.argv[1]
        try:
            new_port = int(arg_port)
            if new_port > 79:
                WEB_PORT = new_port
        except Exception as e:
            print(e)

    uvicorn.run(app, host="0.0.0.0", port=WEB_PORT)
