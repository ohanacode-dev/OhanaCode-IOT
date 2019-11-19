
pyinstaller -F eeprom_reader_admin.py

cd dist
copy /y "eeprom_reader_admin.exe" ..

cd ..
del *.spec
rmdir /s /q build
rmdir /s /q __pycache__
rmdir /s /q dist


pyinstaller -F rfid_reader_admin.py

cd dist
copy /y "rfid_reader_admin.exe" ..

cd ..
del *.spec
rmdir /s /q build
rmdir /s /q __pycache__
rmdir /s /q dist