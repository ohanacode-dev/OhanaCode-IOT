#!/bin/sh

cd "$(dirname "$0")"

pyinstaller -F eeprom_reader_admin.py
mv dist/eeprom_reader_admin eeprom_reader_admin

rm -f *.spec
rm -fr build/
rm -fr __pycache__/
rm -fr dist/

pyinstaller -F rfid_reader_admin.py
mv dist/rfid_reader_admin rfid_reader_admin

rm -f *.spec
rm -fr build/
rm -fr __pycache__/
rm -fr dist/

