EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L esp32_devkit_v1_doit:ESP32_DevKit_V1_DOIT U1
U 1 1 6176C2B3
P 1600 2500
F 0 "U1" H 1600 4081 50  0000 C CNN
F 1 "ESP32_DevKit_V1_DOIT" H 1600 3990 50  0000 C CNN
F 2 "ESP32_DevKit_V1_DOIT:esp32_devkit_v1_doit" H 1150 3850 50  0001 C CNN
F 3 "https://aliexpress.com/item/32864722159.html" H 1150 3850 50  0001 C CNN
	1    1600 2500
	1    0    0    -1  
$EndComp
Text GLabel 4000 2150 0    50   Output ~ 0
sw1
$Comp
L Switch:SW_Push SW1
U 1 1 61771AAF
P 4100 1850
F 0 "SW1" V 4054 1998 50  0000 L CNN
F 1 "Footswitch 1" V 4145 1998 50  0000 L CNN
F 2 "" H 4100 2050 50  0001 C CNN
F 3 "~" H 4100 2050 50  0001 C CNN
	1    4100 1850
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 61772266
P 4100 2400
F 0 "R1" H 4170 2446 50  0000 L CNN
F 1 "10k" H 4170 2355 50  0000 L CNN
F 2 "" V 4030 2400 50  0001 C CNN
F 3 "~" H 4100 2400 50  0001 C CNN
	1    4100 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2150 4100 2150
Wire Wire Line
	4100 2150 4100 2250
Wire Wire Line
	4100 2050 4100 2150
Connection ~ 4100 2150
$Comp
L power:GND #PWR?
U 1 1 61772792
P 4100 2650
F 0 "#PWR?" H 4100 2400 50  0001 C CNN
F 1 "GND" H 4105 2477 50  0000 C CNN
F 2 "" H 4100 2650 50  0001 C CNN
F 3 "" H 4100 2650 50  0001 C CNN
	1    4100 2650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61772D70
P 4100 1500
F 0 "#PWR?" H 4100 1350 50  0001 C CNN
F 1 "+3.3V" H 4115 1673 50  0000 C CNN
F 2 "" H 4100 1500 50  0001 C CNN
F 3 "" H 4100 1500 50  0001 C CNN
	1    4100 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 1500 4100 1650
Wire Wire Line
	4100 2550 4100 2650
Text GLabel 4950 2150 0    50   Output ~ 0
sw2
$Comp
L Switch:SW_Push SW2
U 1 1 61779713
P 5050 1850
F 0 "SW2" V 5004 1998 50  0000 L CNN
F 1 "Footswitch 2" V 5095 1998 50  0000 L CNN
F 2 "" H 5050 2050 50  0001 C CNN
F 3 "~" H 5050 2050 50  0001 C CNN
	1    5050 1850
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 61779719
P 5050 2400
F 0 "R2" H 5120 2446 50  0000 L CNN
F 1 "10k" H 5120 2355 50  0000 L CNN
F 2 "" V 4980 2400 50  0001 C CNN
F 3 "~" H 5050 2400 50  0001 C CNN
	1    5050 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2150 5050 2150
Wire Wire Line
	5050 2150 5050 2250
Wire Wire Line
	5050 2050 5050 2150
Connection ~ 5050 2150
$Comp
L power:GND #PWR?
U 1 1 61779723
P 5050 2650
F 0 "#PWR?" H 5050 2400 50  0001 C CNN
F 1 "GND" H 5055 2477 50  0000 C CNN
F 2 "" H 5050 2650 50  0001 C CNN
F 3 "" H 5050 2650 50  0001 C CNN
	1    5050 2650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61779729
P 5050 1500
F 0 "#PWR?" H 5050 1350 50  0001 C CNN
F 1 "+3.3V" H 5065 1673 50  0000 C CNN
F 2 "" H 5050 1500 50  0001 C CNN
F 3 "" H 5050 1500 50  0001 C CNN
	1    5050 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 1500 5050 1650
Wire Wire Line
	5050 2550 5050 2650
Text GLabel 5850 2150 0    50   Output ~ 0
sw3
$Comp
L Switch:SW_Push SW3
U 1 1 61779E4C
P 5950 1850
F 0 "SW3" V 5904 1998 50  0000 L CNN
F 1 "Footswitch 3" V 5995 1998 50  0000 L CNN
F 2 "" H 5950 2050 50  0001 C CNN
F 3 "~" H 5950 2050 50  0001 C CNN
	1    5950 1850
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 61779E52
P 5950 2400
F 0 "R3" H 6020 2446 50  0000 L CNN
F 1 "10k" H 6020 2355 50  0000 L CNN
F 2 "" V 5880 2400 50  0001 C CNN
F 3 "~" H 5950 2400 50  0001 C CNN
	1    5950 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 2150 5950 2150
Wire Wire Line
	5950 2150 5950 2250
Wire Wire Line
	5950 2050 5950 2150
Connection ~ 5950 2150
$Comp
L power:GND #PWR?
U 1 1 61779E5C
P 5950 2650
F 0 "#PWR?" H 5950 2400 50  0001 C CNN
F 1 "GND" H 5955 2477 50  0000 C CNN
F 2 "" H 5950 2650 50  0001 C CNN
F 3 "" H 5950 2650 50  0001 C CNN
	1    5950 2650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61779E62
P 5950 1500
F 0 "#PWR?" H 5950 1350 50  0001 C CNN
F 1 "+3.3V" H 5965 1673 50  0000 C CNN
F 2 "" H 5950 1500 50  0001 C CNN
F 3 "" H 5950 1500 50  0001 C CNN
	1    5950 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 1500 5950 1650
Wire Wire Line
	5950 2550 5950 2650
Text GLabel 6750 2150 0    50   Output ~ 0
sw4
$Comp
L Switch:SW_Push SW4
U 1 1 6177A74C
P 6850 1850
F 0 "SW4" V 6804 1998 50  0000 L CNN
F 1 "Footswitch 4" V 6895 1998 50  0000 L CNN
F 2 "" H 6850 2050 50  0001 C CNN
F 3 "~" H 6850 2050 50  0001 C CNN
	1    6850 1850
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 6177A752
P 6850 2400
F 0 "R4" H 6920 2446 50  0000 L CNN
F 1 "10k" H 6920 2355 50  0000 L CNN
F 2 "" V 6780 2400 50  0001 C CNN
F 3 "~" H 6850 2400 50  0001 C CNN
	1    6850 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 2150 6850 2150
Wire Wire Line
	6850 2150 6850 2250
Wire Wire Line
	6850 2050 6850 2150
Connection ~ 6850 2150
$Comp
L power:GND #PWR?
U 1 1 6177A75C
P 6850 2650
F 0 "#PWR?" H 6850 2400 50  0001 C CNN
F 1 "GND" H 6855 2477 50  0000 C CNN
F 2 "" H 6850 2650 50  0001 C CNN
F 3 "" H 6850 2650 50  0001 C CNN
	1    6850 2650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 6177A762
P 6850 1500
F 0 "#PWR?" H 6850 1350 50  0001 C CNN
F 1 "+3.3V" H 6865 1673 50  0000 C CNN
F 2 "" H 6850 1500 50  0001 C CNN
F 3 "" H 6850 1500 50  0001 C CNN
	1    6850 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1500 6850 1650
Wire Wire Line
	6850 2550 6850 2650
$Comp
L Device:LED D1
U 1 1 6177FA07
P 4100 3450
F 0 "D1" V 4139 3332 50  0000 R CNN
F 1 "LED" V 4048 3332 50  0000 R CNN
F 2 "" H 4100 3450 50  0001 C CNN
F 3 "~" H 4100 3450 50  0001 C CNN
	1    4100 3450
	0    -1   -1   0   
$EndComp
Text GLabel 3950 3250 0    50   Output ~ 0
LED1
$Comp
L power:GND #PWR?
U 1 1 61780322
P 4100 4200
F 0 "#PWR?" H 4100 3950 50  0001 C CNN
F 1 "GND" V 4105 4027 50  0000 C CNN
F 2 "" H 4100 4200 50  0001 C CNN
F 3 "" H 4100 4200 50  0001 C CNN
	1    4100 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3250 4100 3250
Wire Wire Line
	4100 3250 4100 3300
$Comp
L Device:R R10
U 1 1 61788B31
P 4100 3900
F 0 "R10" H 4170 3946 50  0000 L CNN
F 1 "330" H 4170 3855 50  0000 L CNN
F 2 "" V 4030 3900 50  0001 C CNN
F 3 "~" H 4100 3900 50  0001 C CNN
	1    4100 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3600 4100 3750
Wire Wire Line
	4100 4050 4100 4200
$Comp
L Device:LED D2
U 1 1 6178EEFA
P 5050 3450
F 0 "D2" V 5089 3332 50  0000 R CNN
F 1 "LED" V 4998 3332 50  0000 R CNN
F 2 "" H 5050 3450 50  0001 C CNN
F 3 "~" H 5050 3450 50  0001 C CNN
	1    5050 3450
	0    -1   -1   0   
$EndComp
Text GLabel 4900 3250 0    50   Output ~ 0
LED2
$Comp
L power:GND #PWR?
U 1 1 6178EF01
P 5050 4200
F 0 "#PWR?" H 5050 3950 50  0001 C CNN
F 1 "GND" V 5055 4027 50  0000 C CNN
F 2 "" H 5050 4200 50  0001 C CNN
F 3 "" H 5050 4200 50  0001 C CNN
	1    5050 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 3250 5050 3250
Wire Wire Line
	5050 3250 5050 3300
$Comp
L Device:R R11
U 1 1 6178EF09
P 5050 3900
F 0 "R11" H 5120 3946 50  0000 L CNN
F 1 "330" H 5120 3855 50  0000 L CNN
F 2 "" V 4980 3900 50  0001 C CNN
F 3 "~" H 5050 3900 50  0001 C CNN
	1    5050 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3600 5050 3750
Wire Wire Line
	5050 4050 5050 4200
$Comp
L Device:LED D3
U 1 1 6178FBFF
P 5950 3450
F 0 "D3" V 5989 3332 50  0000 R CNN
F 1 "LED" V 5898 3332 50  0000 R CNN
F 2 "" H 5950 3450 50  0001 C CNN
F 3 "~" H 5950 3450 50  0001 C CNN
	1    5950 3450
	0    -1   -1   0   
$EndComp
Text GLabel 5800 3250 0    50   Output ~ 0
LED3
$Comp
L power:GND #PWR?
U 1 1 6178FC06
P 5950 4200
F 0 "#PWR?" H 5950 3950 50  0001 C CNN
F 1 "GND" V 5955 4027 50  0000 C CNN
F 2 "" H 5950 4200 50  0001 C CNN
F 3 "" H 5950 4200 50  0001 C CNN
	1    5950 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3250 5950 3250
Wire Wire Line
	5950 3250 5950 3300
$Comp
L Device:R R12
U 1 1 6178FC0E
P 5950 3900
F 0 "R12" H 6020 3946 50  0000 L CNN
F 1 "330" H 6020 3855 50  0000 L CNN
F 2 "" V 5880 3900 50  0001 C CNN
F 3 "~" H 5950 3900 50  0001 C CNN
	1    5950 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3600 5950 3750
Wire Wire Line
	5950 4050 5950 4200
$Comp
L Device:LED D4
U 1 1 61790958
P 6850 3450
F 0 "D4" V 6889 3332 50  0000 R CNN
F 1 "LED" V 6798 3332 50  0000 R CNN
F 2 "" H 6850 3450 50  0001 C CNN
F 3 "~" H 6850 3450 50  0001 C CNN
	1    6850 3450
	0    -1   -1   0   
$EndComp
Text GLabel 6700 3250 0    50   Output ~ 0
LED4
$Comp
L power:GND #PWR?
U 1 1 6179095F
P 6850 4200
F 0 "#PWR?" H 6850 3950 50  0001 C CNN
F 1 "GND" V 6855 4027 50  0000 C CNN
F 2 "" H 6850 4200 50  0001 C CNN
F 3 "" H 6850 4200 50  0001 C CNN
	1    6850 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3250 6850 3250
Wire Wire Line
	6850 3250 6850 3300
$Comp
L Device:R R13
U 1 1 61790967
P 6850 3900
F 0 "R13" H 6920 3946 50  0000 L CNN
F 1 "330" H 6920 3855 50  0000 L CNN
F 2 "" V 6780 3900 50  0001 C CNN
F 3 "~" H 6850 3900 50  0001 C CNN
	1    6850 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 3600 6850 3750
Wire Wire Line
	6850 4050 6850 4200
Text GLabel 1650 5300 2    50   Input ~ 0
LED1
Text GLabel 1650 5400 2    50   Input ~ 0
LED2
Text GLabel 1650 5500 2    50   Input ~ 0
LED3
Text GLabel 1650 5600 2    50   Input ~ 0
LED4
Text GLabel 2500 2000 2    50   Input ~ 0
sw4
Text GLabel 2500 1900 2    50   Input ~ 0
sw3
Text GLabel 2500 2100 2    50   Input ~ 0
sw2
Text GLabel 2500 3200 2    50   Input ~ 0
sw1
Wire Wire Line
	2500 1900 2200 1900
Wire Wire Line
	2500 2000 2200 2000
Wire Wire Line
	2500 2100 2200 2100
Wire Wire Line
	2200 3200 2500 3200
$EndSCHEMATC
