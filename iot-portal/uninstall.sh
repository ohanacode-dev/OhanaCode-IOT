#!/bin/bash

IOT_SERVICE_FILE=iotportal.service
OLED_SERVICE_FILE=oled.service
CHECKER_SERVICE_FILE=periodicchecker.service
# Disable all existing services if any
sudo systemctl disable $IOT_SERVICE_FILE
sudo systemctl disable $OLED_SERVICE_FILE
sudo systemctl disable $CHECKER_SERVICE_FILE

sudo rm /etc/systemd/system/$IOT_SERVICE_FILE
sudo rm /etc/systemd/system/$OLED_SERVICE_FILE
sudo rm /etc/systemd/system/$CHECKER_SERVICE_FILE
