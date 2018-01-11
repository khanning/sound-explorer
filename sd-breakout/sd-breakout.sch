EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
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
L Micro_SD_Card J1
U 1 1 5A57924F
P 8800 2000
F 0 "J1" H 8150 2600 50  0000 C CNN
F 1 "Micro_SD_Card" H 9450 2600 50  0000 R CNN
F 2 "sean:MICROSD" H 9950 2300 50  0001 C CNN
F 3 "" H 8800 2000 50  0001 C CNN
	1    8800 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 1800 7900 1800
Wire Wire Line
	7400 1900 7900 1900
Wire Wire Line
	7400 2000 7900 2000
Wire Wire Line
	7400 2100 7900 2100
Wire Wire Line
	7400 2200 7900 2200
Wire Wire Line
	7400 2300 7900 2300
NoConn ~ 7900 1700
NoConn ~ 7900 2400
NoConn ~ 9600 2600
Text Label 7400 1800 0    60   ~ 0
SS
Text Label 7400 1900 0    60   ~ 0
MOSI
Text Label 7400 2000 0    60   ~ 0
VCC
Text Label 7400 2100 0    60   ~ 0
SCK
Text Label 7400 2200 0    60   ~ 0
GND
Text Label 7400 2300 0    60   ~ 0
MISO
$Comp
L CONN_01X06 J2
U 1 1 5A579312
P 7200 2050
F 0 "J2" H 7200 2400 50  0000 C CNN
F 1 "CONN_01X06" V 7300 2050 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm_SMD_Pin1Left" H 7200 2050 50  0001 C CNN
F 3 "" H 7200 2050 50  0001 C CNN
	1    7200 2050
	-1   0    0    1   
$EndComp
$EndSCHEMATC
