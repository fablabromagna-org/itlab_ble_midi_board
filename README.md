# BLE MIDI Foot Controller (waiting for a new name...)

![ProjectImage](docs/images/project_image.png)

A BLE (Bluetooth Low Energy) MIDI implementation based on ESP32

 
> **Important:** 
> this is a working-in-progress project and is in beta stage.
> 
> Since we not yet released the first stable version, most of the features works only in develop branch.
>
> You can find additional info about the project sources in ```/docs/how_to_controbute.md``` file.

 
## Description
 
This project aim to build a BLE MIDI Pedal Board using a cheap ESP32, or similar microcontroller.
 
We created this project to control music and guitar applications, but you can use it for any BLE MIDI compatible application.


>This project is born as personal project, but now it has become a project of Fablab Romagna APS, with the contribution of other FLR members.
>You can found the original README file describing the old implementation [here](./README_OLD.md) .


We are developing a complete new project to create a MIDI foot controller that will work in two modes:

**1.  standard MIDI controller (MIDI BLE Server)**   - use it as external controller for your PC/mobile/compatible device

> you can use it, for example, to control a DAW or a GuitarFX or a looper running on PC/mobile

**2. client MIDI device (MIDI BLE Client)** - use it to connect to BLE controller, acting as PC or smartphone/tablet

> you can use it, for example, to control a device like NUX Mighty, intead to use the original smartphone application


The MIDI controller will consists of a series of monentary-on switches. 

As optional feature the MIDI controller could work also with standard DIN MIDI port or USB (this requires more HW or different microcontroller)


The controller should work in both rechargeable battery and line powered mode (5V).


### Configurable Over-The-Air
The number of switches should be configured in the firmware.

Changing between the two modes should be implemented without reprogramming the firmware

MIDI messages assigned to each footswitch should be configured via software, without reprogramming the firmware.


## Detailed Informations

* [The hardware](/hardware/)
* [The software](/software/)

You can find implementation details on [docs](/docs) folder:


## Credits
Copyright (C) 2021 Fablab Romagna (www.fablabromagna.org) 

All the project files, sources and documents are released as open source and free software
