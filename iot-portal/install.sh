#!/bin/bash

sudo apt install -y python3-pip mosquitto mosquitto-clients python3-dev python3-smbus i2c-tools python3-libgpiod libfreetype6-dev fonts-font-awesome ttf-dejavu
sudo pip3 install paho-mqtt fastapi uvicorn jinja2 aiofiles websocket-client websockets luma.oled pillow==2.8.2 fontawesome OrangePi.GPIO

IOT_SERVICE_FILE=iotportal.service
OLED_SERVICE_FILE=oled.service
CHECKER_SERVICE_FILE=periodicchecker.service
GPIO_BTN_SERVICE_FILE=gpiobtn.service
# Disable all existing services if any
sudo systemctl disable $IOT_SERVICE_FILE
sudo systemctl disable $OLED_SERVICE_FILE
sudo systemctl disable $CHECKER_SERVICE_FILE
sudo systemctl disable $GPIO_BTN_SERVICE_FILE

echo "installing IOT portal service"

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
} > $IOT_SERVICE_FILE

sudo mv $IOT_SERVICE_FILE /etc/systemd/system/
sudo systemctl enable $IOT_SERVICE_FILE


echo "installing OLED display service"

{
echo "[Unit]"
echo Description=OLED driver for weather and temperature display
echo
echo "[Service]"
echo Type=simple
echo User=root
echo Restart=always
echo RestartSec=10
echo WorkingDirectory=$PWD
echo ExecStart=/usr/bin/python3 oled_driver.py
echo
echo "[Install]"
echo WantedBy=multi-user.target
} > $OLED_SERVICE_FILE

sudo mv $OLED_SERVICE_FILE /etc/systemd/system/
sudo systemctl enable $OLED_SERVICE_FILE


echo "installing periodic checker service"

{
echo "[Unit]"
echo Description=Periodic runner of weather and temperature apps
echo
echo "[Service]"
echo Type=simple
echo User=$USER
echo Restart=always
echo RestartSec=30
echo WorkingDirectory=$PWD
echo ExecStart=$PWD/periodic_checker.sh
echo
echo "[Install]"
echo WantedBy=multi-user.target
} > $CHECKER_SERVICE_FILE

sudo mv $CHECKER_SERVICE_FILE /etc/systemd/system/
sudo systemctl enable $CHECKER_SERVICE_FILE

echo "installing GPIO button service"

{
echo "[Unit]"
echo Description=GPIO button reader service
echo
echo "[Service]"
echo Type=simple
echo User=root
echo Restart=always
echo RestartSec=5
echo WorkingDirectory=$PWD
echo ExecStart=$PWD/gpiobtn.py
echo
echo "[Install]"
echo WantedBy=multi-user.target
} > $GPIO_BTN_SERVICE_FILE

sudo mv $GPIO_BTN_SERVICE_FILE /etc/systemd/system/
sudo systemctl enable $GPIO_BTN_SERVICE_FILE
