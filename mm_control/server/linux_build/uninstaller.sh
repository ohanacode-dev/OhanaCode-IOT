#!/bin/bash

APPDIR=$HOME/magiceye
SHORTCUTFILE=$HOME/.local/share/applications/magiceye.desktop
UDEVRULESFILE=/etc/udev/rules.d/ttyACM_magiceye.rules

cd $HOME

rm -f $SHORTCUTFILE
rm -rf $APPDIR
sudo rm -f $UDEVRULESFILE



