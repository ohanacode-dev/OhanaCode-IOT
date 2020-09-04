#!/bin/bash

INSTALLDIR=magieye_install
APP_DIR=magiceye
TIMESTAMP=$(date +"%y%m%d.%I")

mkdir $APP_DIR

rm -rf ../commonlib/__pycache__
rm -rf ../__pycache__
rm magiceyeinstaller*

cp -r ../commonlib $APP_DIR/
cp ../logo.png $APP_DIR/
cp ../magiceye-gui.py $APP_DIR/
cp uninstaller.sh $APP_DIR/

tar -czvf app.tar.gz $APP_DIR

mkdir $INSTALLDIR 
mv app.tar.gz $INSTALLDIR 
cp installer.sh $INSTALLDIR

tar -czvf magiceyeinstaller.$TIMESTAMP.tar.gz $INSTALLDIR

rm -rf $APP_DIR
rm -rf $INSTALLDIR

