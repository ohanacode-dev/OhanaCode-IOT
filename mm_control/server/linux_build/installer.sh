#!/bin/bash

APPDIR=$HOME/magiceye
SHORTCUT_NAME=$HOME/.local/share/applications/magiceye.desktop

# Install dependencies
sudo apt update
sudo apt install -y python3-pip python3-tk mosquitto-clients mosquitto
python3 -m pip install opencv-python numpy cvlib tensorflow pyserial PySimpleGUI paho-mqtt

# Copy the archive to APPDIR
rm -rf $APPDIR
mkdir $APPDIR
cp app.tar.gz $HOME
cd $HOME

# Unpack the app
tar -xvzf app.tar.gz
rm -f app.tar.gz

cd $APPDIR

# Create a start menu shortcut
mkdir $HOME/.local/share/applications

echo "[Desktop Entry]" > $SHORTCUT_NAME
echo "Encoding=UTF-8" >> $SHORTCUT_NAME
echo "Type=Application" >> $SHORTCUT_NAME
echo "Name=Magiceye GUI Calibration Utillity" >> $SHORTCUT_NAME
echo "Comment=''" >> $SHORTCUT_NAME
echo "Exec=python3 $APPDIR/magiceye-gui.py" >> $SHORTCUT_NAME
echo "StartupNotify=false" >> $SHORTCUT_NAME
echo "Terminal=false" >> $SHORTCUT_NAME
echo "Path=$APPDIR" >> $SHORTCUT_NAME
echo "Icon=$APPDIR/logo.png" >> $SHORTCUT_NAME


# Allow serial device access
echo "KERNEL==\"ttyACM[0…9]\" SYMLINK+=\"%k\" GROUP=\"$USER\" MODE=\"0666\"" > ttyACM_magiceye.rules
sudo cp ttyACM_magiceye.rules /etc/udev/rules.d/
rm ttyACM_magiceye.rules

