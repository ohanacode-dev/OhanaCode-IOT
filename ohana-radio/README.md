# ujagaga-radio, a Flask based python internet radio web interface #

The idea is to enable controlling a central radio via any web enabled device (phone). This project is intended only for linux. 
It is tested on Ubuntu20.
We are using Media Player Daemon to play an internet radio stream and Media Player Client to controll it.

### Setting up ###

The script uses xdotool to control volume, mpd and mpc to play a stream, python Flask to display the web interface. Install them all.

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

Now restart mpd:

    sudo systemctl restart mpd.service

Try the mpd settings using:

    mpd --stdout --no-daemon --verbose

If any errors are displayed, you need to solve them.

Test MPC:

    mpc help
    mpc add https://stream.vanillaradio.com:8028/live
    mpc play
    mpc stop

Run the radio script:

    chmod +x radio.py
    ./radio.py

Currently the web interface will listen to port 8888, so to access it, use your computers IP address and the port 8888. On the same computer this will be 

    http://localhost:8888

If you wish to use it without the port, change the: WEB_PORT=8888 at the start of the script to: WEB_PORT=80 and then run the script as root. The address will now be 

    http://localhost

or if it is on a remote computer: 

    http://192.168.x.x 
    
or what ever the IP address is.

## Contact ##

* [My web page](http://www.radinaradionica.com)