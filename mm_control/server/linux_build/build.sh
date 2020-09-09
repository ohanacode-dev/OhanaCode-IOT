#!/bin/bash

APP_DIR=mm_ctrl
TIMESTAMP=$(date +"%y%m%d.%I")

mkdir $APP_DIR

rm -rf ../__pycache__
rm mm_installer*

cp -r ../assets $APP_DIR/
cp -r ../templates $APP_DIR/
cp ../logo.png $APP_DIR/
cp ../*.py $APP_DIR/
cp uninstall.sh $APP_DIR/
cp install.sh $APP_DIR/

tar -czvf $APP_DIR-$TIMESTAMP.tar.gz $APP_DIR

rm -rf $APP_DIR

