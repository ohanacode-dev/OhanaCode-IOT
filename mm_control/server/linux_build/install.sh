#!/bin/bash

APPDIR=$PWD
SHORTCUT_NAME=$HOME/.local/share/applications/mm_ctrl.desktop
STARTUP_LINK=$HOME/.config/autostart/mm_ctrl.desktop

# Install dependencies
sudo apt update
sudo apt install -y python3-pip mpd mpc xdotool
pip3 install Flask

# Create a start menu shortcut
mkdir $HOME/.local/share/applications

echo "[Desktop Entry]" > $SHORTCUT_NAME
echo "Encoding=UTF-8" >> $SHORTCUT_NAME
echo "Type=Application" >> $SHORTCUT_NAME
echo "Name=Ohanacode Multimedia Controll utility" >> $SHORTCUT_NAME
echo "Comment=''" >> $SHORTCUT_NAME
echo "Exec=python3 $APPDIR/mm_control.py" >> $SHORTCUT_NAME
echo "StartupNotify=false" >> $SHORTCUT_NAME
echo "Terminal=false" >> $SHORTCUT_NAME
echo "Path=$APPDIR" >> $SHORTCUT_NAME
echo "Icon=$APPDIR/logo.png" >> $SHORTCUT_NAME
echo "Categories=AudioVideo;" >> $SHORTCUT_NAME

cp $SHORTCUT_NAME $STARTUP_LINK

