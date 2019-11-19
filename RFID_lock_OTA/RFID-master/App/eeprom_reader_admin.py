#!/usr/bin/python2.7
"""
Author: Rada Berar
Date 06.01.2017.

This script communicates with RFID EEPROM COPY device and records the id codes in a text file.
It also can write content of a text file into device for programming RFID locks
Written for python 2
"""

from serial import Serial
from os import path
import sys
from platform import system

print '\n\n'

# determine if application is a script file or frozen exe
if getattr(sys, 'frozen', False):
    app_name = path.basename(sys.executable)
    current_path = path.dirname(sys.executable)
elif __file__:
    app_name = path.basename(__file__)
    current_path = path.dirname(path.realpath(__file__))

CODE_COUNT = 32     # total number of rfid codes contained in EEPROM

# check parameters
if len(sys.argv) < 2:
    print "Usage: %s <device serial port> [<path/to/file.txt>]" % app_name

    if 'Linux' in system():
        print "  example: %s ttyUSB0" % app_name
    else:
        print "  example: %s com7" % app_name

    print "Press ENTER to exit"
    raw_input()
    sys.exit()

device_name = sys.argv[1]

if 'Linux' in system():
    # making sure device name starts right
    device_name = device_name.replace('/', '')
    device_name = device_name.replace('dev', '')
    device_name = '/dev/' + device_name

if len(sys.argv) < 3:
    fileName = current_path + '/codes.txt'

    print 'No file specified. using "', fileName, '"'
else:
    fileName = sys.argv[2]

    if '/' not in fileName:
        fileName = current_path + '/' + fileName

if not path.exists(fileName):
    print "File", fileName, "does not exist. Creating an empty one."
    ft = open(fileName, 'w')
    ft.close()

try:
    device = Serial(device_name, baudrate=9600, timeout=2)
except:
    sys.exit("Device %s not found. Please specify valid COM port" % device_name)
#  end of parameter check

data = []


def check_file():
    global fileName
    global data

    f = open(fileName, 'r')
    content = f.read().splitlines()
    f.close()

    # the file must contain CODE_COUNT lines of rfid codes
    if len(content) != CODE_COUNT:
        sys.exit("File %s does not contain valid number of rfid codes" % fileName)

    data = []
    for i in range(0, CODE_COUNT):
        value = []

        # allow each row to have a comment like owner data
        line = content[i][0:8]

        if len(line) != 8:
            sys.exit("data in line %d is corrupted" % i)

        value.append(int(line[0:2], 16))
        value.append(int(line[2:4], 16))
        value.append(int(line[4:6], 16))
        value.append(int(line[6:8], 16))

        data.append(value)

    print "File OK"


def write_to_file():
    global fileName
    global data

    f = open(fileName, 'r')
    content = f.read().splitlines()
    f.close()

    values_changed = False

    for i in range(0, CODE_COUNT):

        if i < len(content):
            if data[i] == content[i][0:8]:
                # this value is already saved, so do nothing
                print "Match in line %d. Skipping" % i
            else:
                # this value is different, so change the content in this line
                content[i] = data[i]
                values_changed = True
                print "Writing line", i
        else:
            content.append(data[i])
            values_changed = True

    # write the content back to the file if values are different
    if values_changed:
        f = open(fileName, 'w')
        for i in range(0, CODE_COUNT):
            f.write(content[i])
            f.write('\n')
        f.close()


# wait for device command
rx = []

print "Waiting for command."
print "     Press the write pushbutton to dump recorded codes in the specified text file."
print "     Press the read pushbutton to record id codes from the specified text file to device."

while len(rx) == 0:
    rx = device.read(1)

# respond to command
device.write(rx)

if rx[0] == 'r':
    # read codes from file and send to EEPROM copy device

    check_file()

    print "Writing to device"

    for idx in range(0, CODE_COUNT):
        device.write(data[idx])
        rx = device.read(1)
        if ord(rx[0]) != 1:
            sys.exit("unexpected device response in first data send")

    # device should now perform a second read to compare saved values
    rx = device.read(1)

    if rx[0] == 'r':
        # respond to command
        device.write(rx)

        print "Validating device data"

        for idx in range(0, CODE_COUNT):
            device.write(data[idx])
            rx = device.read(1)
            if ord(rx[0]) != 1:
                sys.exit("unexpected device response in validation")

        print "Data Validation successful"
    else:
        sys.exit("Data validation unsuccessful")

else:
    if rx[0] == 'w':
        # receive codes and write them to file

        print "reading from device"

        data = []
        for idx in range(0, CODE_COUNT):
            temp = device.read(4)

            # print ord(temp[0]), ord(temp[1]), ord(temp[2]), ord(temp[3])
            tempStr = "%0.2X" % ord(temp[0]) + "%0.2X" % ord(temp[1]) + "%0.2X" % ord(temp[2]) + "%0.2X" % ord(temp[3])
            data.append(tempStr)
            device.write(bytearray([1]))

        write_to_file()

        # device should now perform a read to validate saved data
        rx = device.read(1)

        if rx[0] == 'r':
            # respond to command
            device.write(rx)

            print "Validating saved data"

            check_file()

            for idx in range(0, CODE_COUNT):
                device.write(data[idx])
                rx = device.read(1)
                if ord(rx[0]) != 1:
                    sys.exit("unexpected device response in validation")

            print "Data Validation successful"
        else:
            sys.exit("Data validation unsuccessful")

    else:
        print "Unknown command received:", rx[0]

print "Finished successfully! If the device green LED is ON, everything is OK"
device.close()
sys.exit()
