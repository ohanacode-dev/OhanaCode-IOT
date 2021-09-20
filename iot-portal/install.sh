#!/bin/bash

sudo apt install mosquitto python3-dev python3-smbus i2c-tools python3-libgpiod libfreetype6-dev ttf-mscorefonts-installer
sudo pip3 install paho-mqtt fastapi uvicorn jinja2 aiofiles websocket-client websockets luma.oled pillow==2.8.2

sudo mkdir /usr/share/fonts
sudo cp DejaVuSans.ttf /usr/share/fonts/

echo "installing startup service"

SERVICE_FILE=iotportal.service
{
echo "[Unit]"
echo Description=IOT-Portal
echo
echo "[Service]"
echo Type=simple
echo User=root
echo WorkingDirectory=$PWD
echo ExecStart=/usr/bin/python3 iot-portal.py 80
echo
echo "[Install]"
echo WantedBy=multi-user.target
} > $SERVICE_FILE

sudo mv $SERVICE_FILE /etc/systemd/system/
sudo systemctl enable $SERVICE_FILE


