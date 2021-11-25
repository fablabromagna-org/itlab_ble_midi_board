# BLE MIDI footswitch board - ESP32 Firmware

Firmware for BLE MIDI footswitch controller using ESP32 microcontroller.

## General description

> In this documentation we use the abbreviation FS in place of FootSwitch(es)

This is the C++ firmware for BLE MIDI controller based on ESP32 chip. 

In this first release it acts only as Server BLE device. The BLE client functionality will be added in the near future.

This firmware is work-in-progress and some features are implemented in the simplest way to create a working prototype.

The full modularity of the firmware in not yet completed.


The firmware can manage a number of footswitches definied in the source code (the exact number of FS need to be edited before compiling, to meet the hardware you decide to use).
A first demo board use 4 FS.

The actions associated to each footswitch can be programmed via Bluethoot.


The firmware implements a series of BLE services, and related BLE characteristics :
* **The MIDI BLE Service**
    * The MIDI BLE Characteristic (R/NOTIFY)
* **The Device Configuration BLE Service**
    * The Footswitches Configuration BLE Characteristic (R/W/NOTIFY)
* **The Battery BLE Service**
    * The Battery Level BLE Service BLE Characteristic (R/NOTIFY)


The main purpose of the device is to work as BLE MIDI controller. So the MIDI BLE Characteristic is used to notify MIDI events to the client application (PC/Tablet/Smartphone).

The "Footswitches Configuration BLE Characteristic" is used to program the function (MIDI message to send) for each footswitch, without reprogramming the full firmware.

The firmware uses some standard libraries from PlatformIO framework (BLE classes, JSON management, ...) and a set of libraries created for this project, and contained in the ```/lib``` subfolder.



## How the firmware works

The main project file define a set of constants regarding the BLE UUID for services and characteristics, the number of FS, the pin numbers for each digital inputs and more.

Also it defines an Interrupt Service Routine for each FS. These routines are attached to CHANGE interrupt events for each FS pin.

In the ```setup()``` function all the BLE services and characteristics are initializated, with related callbacks, and the BLE advertising in started.

Finally a default testing binary configuration is processed, by the content from ```test_config.h```.

> In the future this will replaced loading the last received configuration from the flash storage.


The ```loop()``` control functions manage the BLE disconnections and reconnections and the functions related to REPEAT events configured for the switches (see below).



### Footswitch events

The press or release events on each FS are intercepted by the ISR routines and then passed to ```process_interrupt()``` function.

In this function, using a FootSwitch helper class and depending of the type of event and the previous state, the we detect if a TAP event occours or an HOLD events starts or terminates.

The helper class also implements a debounce software routine.

Once the type of event is recognized, another helper class (```FootSwitchController```) is used to obtain the MIDI message to send.

The global instance ```footSwitchController``` is initializated in the ```setup()``` function with the binary configuration.

The configuration define, for each TAP and HOLD event, the MIDI message to send.

Based on that configuration, the ```FootSwitchController::processEvent()``` function build and returns the relevant MIDI message.



### MIDI messages

MIDI message are built using the ```MidiHelper``` class.

At the moment we use only CC or PC Midi messages. 

In this first version, only MIDI over BLE is used, but in the future we can use classic MIDI serial (DIN-5 connector) or MIDI-USB (in this case we need a different microcontroller with native USB support)

----


## How the OTA footswitches configuration works

### Introduction

The purpose of this project is to have a footswitch MIDI controller with programmable MIDI messages assigned to each FS event.

A detailed description of each possible configuration is described in documents in the /doc folder of this repository (under construction).

In general, for each FS you can define a MIDI message for TAP event and a MIDI message for HOLD event.

The MIDI message binded to each event, for each fs, could be configured by an external application (PC or mobile) throught BLE.

A particular BLE characteristic is used to receive a new configuration, or to read the current one.
Once received a new configuration, it's stored in (permanent) flash memory  for using in the next device reboot.

> Since the amount of data transfered via BLE char is limited to 512 byte we use a binary format described below.
> So the high level configuration used in the PC/mobile app (tipically a JSON configuration) is converted to binary stream to be transmitted to the device. And vice versa.


### The binary format of the device configuration

> Note: the project is in a very early stage and some changes may occour in the binary format.
> It's very important to report any changes to the doc to mantain in sync both the firmware and PC/Mobile application.



In this first versione, the binary structure match byte-byte the struct ```ControllerConfiguration``` definied in the class ```FootSwitchController```.

At the moment is not yet implemented a robust serialization mechanism, so we only use an uint8_t array to copy the received data from BLE to the structure.

In addition, some control bytes are inserted to better identify some data area and permit a poor check of the binary packet.

Here the details of the binary configuration:

|  BYTE NR  |LEN|FIELD                     |NOTES          |
|-------|---|--------------------------|---------------|
|0      |  1|Ctrl start byte           |fixed to 0xf0  |
|1      | 32|Device name for BLE Adv.  |use only max 31 bytes |
|2      |  1|Firmware release Major Nr |   |
|3      |  1|Firmware release Minor Nr |   |
|4      |  1|BLE Mode                  |1=server / 2=client   |
|5      |  1|Number of Footswitces     |   |
|6      |  1|Val min variable 1         |minimum value for internal variable 1   |
|7      |  1|Val max variable 1         |maximum value for internal variable 1   |
|8      |  1|cycle variable 1           |Cycle when reached min/max value: 0=off / 1=on  |
|9      |  1|Val min variable 2         |minimum value for internal variable 2   |
|10     |  1|Val max variable 2         |maximum value for internal variable 2   |
|11     |  1|cycle variable 2           |Cycle when reached min/max value: 0=off / 1=on  |
|12     |  1|Val min variable 3         |minimum value for internal variable 3   |
|13     |  1|Val max variable 3         |maximum value for internal variable 3   |
|14     |  1|cycle variable 3           |Cycle when reached min/max value: 0=off / 1=on  |
|15     |  1|Val min variable 4         |minimum value for internal variable 4   |
|16     |  1|Val max variable 4         |maximum value for internal variable 4   |
|17     |  1|cycle variable 4           |Cycle when reached min/max value: 0=off / 1=on  |
|**TAP Config. for FS1**       |   | ||
|18     |  1|FS Ctrl start byte        |fixed to 0x80   |
|19     |  1|event          |1=single / 2=repeat / 3=increment / 4=on-off  |
|20     |  1|midi_ch        |MIDI channel 0-127   |
|21     |  1|midi_type      |MIDI message type: 1=CC / 2=PC |
|22     |  1|midi_nr        |MIDI number of CC/PC   |
|23     |  1|midi_value_on  |MIDI value to send   |
|24     |  1|midi_value_off |MIDI value to send (on-off event only)  |
|25     |  1|not used       |   |
|26     |  1|group_idx      |Group index: 1-4 (on-off event only)   |
|27     |  1|intval_idx     |Internal value index: 1-4 (increment event only)  |
|28     |  1|positive_step  |1=positive step / 0=negative step (increment event only) |
|29     |  1|intval_step    |absolute number of step  to increment/decrement (increment event only) |
|30     |  1|FS  Ctrl end byte      |fixed to 0x88   |
|**HOLD Config. for FS1** |||
|31     |  1|FS Ctrl start byte        |fixed to 0x80   |
|32     |  1|event          |1=single / 2=repeat / 3=increment / 4=on-off  |
|33     |  1|midi_ch        |MIDI channel 0-127   |
|34     |  1|midi_type      |MIDI message type: 1=CC / 2=PC |
|35     |  1|midi_nr        |MIDI number of CC/PC   |
|36     |  1|midi_value_on  |MIDI value to send   |
|37     |  1|midi_value_off |MIDI value to send (on-off event only)  |
|38     |  1|repeat_interval|interval repeat time (msec*10) for repeat or increment events    |
|39     |  1|group_idx      |Group index: 1-4 (on-off event only)   |
|40     |  1|intval_idx     |Internal value index: 1-4 (increment event only)  |
|41     |  1|positive_step  |1=positive step / 0=negative step (increment event only) |
|42     |  1|intval_step    |absolute number of step  to increment/decrement (increment event only) |
|43     |  1|FS  Ctrl end byte      |fixed to 0x88   |
|**repeat for FS2** |||
|...|||
|**repeat for FS3** |||
|...|||
|**repeat for FSn** |||
|...|||
|n-1    |  1|Ctrl end byte           |fixed to 0xff  |

> _*... repeat from byte 18 to byte 43 for each FS (the total nr of FS is defined on byte 2)*_ 
