# ohana-radio, a Flask based python internet radio web interface #

The idea is to enable controlling a central radio via any web enabled device (phone). This project is intended only for linux. 
It is tested on Ubuntu20.
We are using Media Player Daemon to play an internet radio stream and Media Player Client to controll it.

### Setting up ###

The script uses xdotool to control volume, mpd and mpc to play a stream and python Flask to display the web interface. Install them all.

    sudo apt install mpd mpc xdotool python3-pip
    pip3 install flask


The next step is to configure the configuration file: 
    
    /etc/mpd.conf.

In the first section: Files and directories, change all paths to a folder in your home location like: 

    /home/<user>/.mpd/

Change user from mpd to your user name.

In the Audio Output section, to find the correct setting I had to run: 

    aplay -l

and got response:

    card 0: Generic [HD-Audio Generic], device 3: HDMI 0 [HDMI 0]
        Subdevices: 1/1
        subdevice #0: subdevice #0
    card 1: Generic_1 [HD-Audio Generic], device 0: ALC256 Analog [ALC256 Analog]
        Subdevices: 1/1
        Subdevice #0: subdevice #0

So my setting is:

    audio_output {
        type		"alsa"
        name		"Generic_1" 

Now restart your computer or try just the mpd service:

    sudo systemctl restart mpd.service

Try the mpd settings using:

    mpd --stdout --no-daemon --verbose

If any errors are displayed, you need to solve them and restart the mpd service. For your conveniance, there is an mpd.conf example file in this folder.

Test MPC:

    mpc help
    mpc add https://stream.vanillaradio.com:8028/live
    mpc play
    mpc stop

Run the radio script:

    chmod +x ohana-radio.py
    sudo ./ohana-radio.py

Currently the web interface will listen to port 80, so to access it, it must be ran as root. The address on the same computer is: 

    http://localhost

From remote computer use:

    http://192.168.x.x

or what ever the IP address is.
If you wish to use it without root priviledges, you need to change the port at the start of the ohana-radio.py script from WEB_PORT=80 to some other value like WEB_PORT=8888. To access it, use your computers IP address and the port 8888. On the same computer this will be 

    http://localhost:8888

or if it is on a remote computer: 

    http://192.168.x.x:8888 

An automated script, setupRadio.sh is also provided to help with the setup.

## Setting up on a single board computer ##

I used an OrangePi Zero to build an internet radio. On an Armbian OS it was enough to install mpc and mpd. No additional configuration was required. 
Added an audio amplifier with a potentiometer for volume control. Used 3 push buttons connected to GPIO pins for previous, next and play/pause.
My script to control the mpc functions via gpio is preset here as gpioctl.py. To start it, run: 
```
sudo apt install python3-dev
pip3 install OrangePi.GPIO
chmod +x gpiocmd.py
sudo ./gpiocmd.py
```

A usefull way to run it is to add it to sudoers as:
my_user_name	ALL=(ALL) NOPASSWD: path_to_gpiocmd.py
Then you can run it at startup without a password

## Streaming audio from another linux machine ##

Tried using pulse audio to stream audio to my internet radio, but the bandwidth was too high. Instead I installed on my desktop computer icecast and darkice:
```
sudo apt-get install icecast2 darkice

```

Start the server:
```
service icecast2 start
```

Create a darkice configuration file /etc/darkice.cfg with following content (do check the password):
```
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
password = your_icecast_password
mountPoint = defaultaudio.ogg
name = Your stream name
description = Streaming audio from my desktop
url = http://localhost
genre = my own
public = yes
localDumpFile = dump.ogg
```

Now start darkice:
```
darkice
```

On your internet radio add a playlist entry:
http://<your_server_ip>:8000/defaultaudio.ogg


To setup a streaming service on your comuter, you can also use my script: setupStreaming.sh
NOTE: It is conveniant if you set your computer IP address to static so the URL does not change through router restart.
    
## Contact ##

* [My busines web page](http://www.ohanacode-dev.com)
* [My personal web page](http://www.radinaradionica.com)

