Flir Pi Cam
====================================

![Product photo.](https://raw.githubusercontent.com/marshalltaylorSFE/FlirPiCam/master/Product_Photo.jpg)

This is an open source design for a FLiR thermal camera built on the raspberry Pi.

Features
----------------

* Flir Lepton module
* Raspberry Pi 2
* ~1 hour battery life
* Rechargeable
* Battery monitoring into linux
* Button interface for capturing screenshots, mounting and unmounting usb
* Buttons can trigger custom shell scripts which can be edited from the OS

Linux software
-----------------

* Raspbian
* Flir Video monitor demo for the Lepton module
* Custom qt4 application for system monitor
* scrot screen capture

Extra Linux software
-----------------

* capture for capturing usb video streams
* ffmpeg for compression

System Firmware
-------------------

* Operates as class compliant keyboard for some functions (login screepting).  TODO: auto login
* Operates as USB serial TTY for running screen capture/custom scripts
* Button hold features implemented, can be used for alternate scripts of sudoing the scripts

This is released under the [Creative Commons ShareAlike 4.0 International](https://creativecommons.org/licenses/by-sa/4.0/) license. 

Please consider contributing back to this library or others to help the open-source hardware community continue to thrive and grow! 
