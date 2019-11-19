# Lazy Control

This application is a part of my personal, opensource IOT project. It is a remote controller for the PC. 
Written in python3, intended to work with Linux and Windows.
lazycrtl.py can simylate keyboard upon commands received via http requests.
It also responds to UDP broadcast (to string "ujagaga ping") so it can be located on network (using android application "UjagagaIOTBrowser").
The web user interface is contained in the "skins" subfolder, so you may customize skins as you wish and add new ones.

If you are working on windows, you can use win_build/App.zip. If you are on linux, use linux_build/App.zip or debian install package lazyctrl-ver.deb.
