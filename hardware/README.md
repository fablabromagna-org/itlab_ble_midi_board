# The Hardware

In the prototipal stage we will use board and solutions available as kit or consumer product.

May be in the future we'll create a custom PCB with KiCad, a custom enclosure with FreeCAD and 3d printer and a fancy decal with Inkscape.


## Footswitches and case
Foot switches are momentary-on (normally open). You can use any button, but metal foot switches are recommended 

![Footswitch](/docs/images/footswitch.jpg)

## Microcontroller
In this first version we use a a Wemos D1 R32 ESP32 board or equivalent ESP32 board with both WIFI and BLE.

Breadboard and dupont wires are our friends

## Battery and power supply
We aim to power the device with rechargeable battery. In this stage may be 18650 Li-ion battery is a good solution.

We need to provide power via micro-usb or USB-C port too (direct power supply and/or recharge mode).

## Port and connections
The main connection of the controller is wireless (primary BLE - WIFI as optional).

Despite this we have to provide micro-usb input for power and firmware upgrade.

Optional but notable could be standard DIN MIDI (at least as MIDI-OUT). This would allow to use the footswitch to directly control that devices that are not using BLE (e.g. guitar FX/modeler).

Another optional but interesting feature could be a MIDI-USB port. But in this case we need a microcontroller with programmable native-usb, or use additional microcontroller for this purpose.


