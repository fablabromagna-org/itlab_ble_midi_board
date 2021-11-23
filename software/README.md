# The Software

This subfolder contains software implmentation for both Firmware and PC/Mobile sides.

You can find additional informations in README files contained in each subfolder.

* **esp32:**  the device firmware for Espressif ESP32 
* **tools:**  software tools and utility for PC, used to test and debug the firmware and to program the device configuration



----

## Microcontroller Firmware
We program the ESP32 using VSCode (or Code-OSS) and PlatformIO IDE and libraries. 

For BLE management we use BLE Arduino Library, but some testing branches are also using NimBLE.

Refer to README files in the software subfolders for details.


> May be we will do some test with Micropython / CircuitPython too.




## Configuration Application
(... tbd)


## Test and debug tools

Tested on Android 6, iOS 11 and Linux Debian.

For debug purpose we suggest the following utilities to verify if your device can receive midi messages:

* midiscope for Android
* midiwrench for iOS
* JACK  + kmidimon  for Linux
