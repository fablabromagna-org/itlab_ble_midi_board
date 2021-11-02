BLE_MIDI_board_client.ino
===============================

This is the Arduino source code for ESP32, acting as MIDI BLE client.
This is the first attemp to control my NUX MIGHTY PLUG without using the Smartphone app.


It perform the following actions:
* scan for BLE device
* connect to first BLE MIDI device (match BLE MIDI service)
* send a MIDI CC message every 5 seconds (CC 41)



## TODO

### Basic features

* use a small TFT display to show discovered devices
* use buttons or rotary encoder/push button to interact with the UI
* filter MIDI BLE devices by name to select the device to connect to
* use footswitch buttons to send MIDI messages


### Advanced features (maybe someday)

* use app on Smartphone or PC to program the MIDI messages for each footswitch button
    * assign and store single message to each button
    * assign and store multiple messages to each button (macro mode)
    * assign preset to each button (send all commands to assign and save preset)
* save binded devices to reconnect on switch on
