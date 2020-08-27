#!/bin/sh

cd "$(dirname "$0")"
cd ..
ver="3.1"

# building standalone app
pyinstaller -F ./tcp_api.py

# removing any leftovers from previous build
rm -f *.spec
rm -fr build/
rm -fr __pycache__/
rm -fr ./linux_build/lazyctrl-$ver/opt/lazyctrl/
cp -f ./icon.png dist/
cp -f ./startup.wav dist/
cp -f ./theend.wav dist/
mkdir dist/skins
cp -f -r ./skins/* dist/skins/

# creating necessary folders (in case they do not exist) for building the debian package
mkdir linux_build/lazyctrl-$ver/opt

mv dist/ linux_build/lazyctrl-$ver/opt/lazyctrl/

chmod -R 755 linux_build/lazyctrl-$ver/DEBIAN/

dpkg-deb --build ./linux_build/lazyctrl-$ver/

cd linux_build/lazyctrl-$ver/opt

zip -r App.zip ./lazyctrl
mv App.zip ../../
cd ..
cd ..

