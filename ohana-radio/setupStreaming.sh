#!/bin/bash

sudo apt update
sudo apt -y install icecast2 darkice
service icecast2 start

echo "
[general]
duration = 0
bufferSecs = 5
reconnect = yes

[input]
device = default
sampleRate = 44100
bitsPerSample = 16
channel = 2

[icecast2-0]
bitrateMode = abr
format = vorbis
bitrate = 96

# Make sure your server 
server = localhost
port = 8000
password = hackme
mountPoint = defaultaudio.ogg
name = Computer audio
description = Streaming audio from my desktop
url = http://localhost
genre = my own
public = yes
localDumpFile = dump.ogg
" > darkice.cfg

sudo mv ./darkice.cfg /etc/

mkdir ~/.config/autostart

echo "
[Desktop Entry]
Type=Application
Name=darkice
Comment=
Exec=darkice
RunHook=0
StartupNotify=false
Terminal=false
Hidden=false
" > ~/.config/autostart/darkice.desktop


