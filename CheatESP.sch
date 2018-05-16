EESchema Schematic File Version 2
LIBS:74xgxx
LIBS:74xx
LIBS:Altera
LIBS:ESD_Protection
LIBS:LEM
LIBS:Lattice
LIBS:Oscillators
LIBS:Power_Management
LIBS:RFSolutions
LIBS:Worldsemi
LIBS:Xicor
LIBS:Zilog
LIBS:ac-dc
LIBS:actel
LIBS:adc-dac
LIBS:allegro
LIBS:analog_devices
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:battery_management
LIBS:bbd
LIBS:bosch
LIBS:brooktre
LIBS:cmos4000
LIBS:cmos_ieee
LIBS:conn
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:device
LIBS:digital-audio
LIBS:diode
LIBS:display
LIBS:dsp
LIBS:elec-unifil
LIBS:ftdi
LIBS:gennum
LIBS:graphic_symbols
LIBS:hc11
LIBS:infineon
LIBS:intel
LIBS:interface
LIBS:intersil
LIBS:ir
LIBS:leds
LIBS:linear
LIBS:logic_programmable
LIBS:maxim
LIBS:mechanical
LIBS:memory
LIBS:microchip
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic24mcu
LIBS:microchip_pic32mcu
LIBS:microcontrollers
LIBS:modules
LIBS:motor_drivers
LIBS:motorola
LIBS:motors
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:opto
LIBS:philips
LIBS:power
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:siliconi
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:switches
LIBS:texas
LIBS:transf
LIBS:transistors
LIBS:triac_thyristor
LIBS:ttl_ieee
LIBS:valves
LIBS:video
LIBS:wiznet
LIBS:xilinx
LIBS:zetex
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
L GND #PWR?
U 1 1 5AFC39E3
P 4300 4800
F 0 "#PWR?" H 4300 4550 50  0001 C CNN
F 1 "GND" H 4300 4650 50  0000 C CNN
F 2 "" H 4300 4800 50  0001 C CNN
F 3 "" H 4300 4800 50  0001 C CNN
	1    4300 4800
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 5AFC39F7
P 4800 2800
F 0 "#PWR?" H 4800 2650 50  0001 C CNN
F 1 "+3V3" H 4800 2940 50  0000 C CNN
F 2 "" H 4800 2800 50  0001 C CNN
F 3 "" H 4800 2800 50  0001 C CNN
	1    4800 2800
	1    0    0    -1  
$EndComp
Text Label 4500 3700 0    60   ~ 0
VCC
Text Label 4500 3900 0    60   ~ 0
RESET
Text Label 4500 4100 0    60   ~ 0
CH_PD
Text Label 4500 4300 0    60   ~ 0
TXD
Text Label 4200 3700 2    60   ~ 0
TXD
Text Label 4200 3900 2    60   ~ 0
GPIO0
Text Label 4200 4100 2    60   ~ 0
GPIO2
Text Label 4200 4300 2    60   ~ 0
GND
Text Label 2700 3700 2    60   ~ 0
VCC
Text Label 2700 4300 2    60   ~ 0
GND
Text Label 2700 4100 2    60   ~ 0
SDA
Text Label 2700 3900 2    60   ~ 0
SCL
Entry Wire Line
	4200 4700 4300 4800
Entry Wire Line
	4400 2800 4500 2900
$Comp
L R R1
U 1 1 5AFC3E5D
P 2850 3500
F 0 "R1" V 2930 3500 50  0000 C CNN
F 1 "4K7" V 2850 3500 50  0000 C CNN
F 2 "" V 2780 3500 50  0001 C CNN
F 3 "" H 2850 3500 50  0001 C CNN
	1    2850 3500
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5AFC3F5A
P 3150 3600
F 0 "R2" V 3230 3600 50  0000 C CNN
F 1 "4K7" V 3150 3600 50  0000 C CNN
F 2 "" V 3080 3600 50  0001 C CNN
F 3 "" H 3150 3600 50  0001 C CNN
	1    3150 3600
	1    0    0    -1  
$EndComp
Connection ~ 4500 3700
Connection ~ 4500 3900
Connection ~ 4500 4100
Connection ~ 4500 4300
Connection ~ 4200 4300
Connection ~ 4200 4100
Connection ~ 4200 3900
Connection ~ 4200 3700
Connection ~ 4800 2800
Connection ~ 2700 3900
Connection ~ 2700 4100
Connection ~ 2700 4300
Connection ~ 2700 3700
Wire Bus Line
	2400 4800 6050 4800
Wire Wire Line
	4500 2900 4500 3700
Wire Wire Line
	4500 4100 4400 4100
Wire Wire Line
	4400 4100 4400 3250
Wire Wire Line
	4400 3250 4500 3250
Connection ~ 4500 3250
Wire Bus Line
	2700 3900 4200 3900
Wire Bus Line
	2700 4100 4200 4100
Wire Wire Line
	4200 4300 4200 4700
Entry Wire Line
	2700 4700 2800 4800
Wire Wire Line
	2700 4700 2700 4300
Wire Notes Line
	3900 3550 3900 4350
Wire Notes Line
	3900 4350 4800 4350
Wire Notes Line
	4800 4350 4800 3550
Wire Notes Line
	4800 3550 3900 3550
Text GLabel 4800 3600 2    60   Input ~ 0
ESP8266
Wire Notes Line
	2750 4350 2750 3600
Wire Notes Line
	2750 3600 2500 3600
Wire Notes Line
	2500 3600 2500 4350
Wire Notes Line
	2500 4350 2750 4350
Text GLabel 2500 3900 0    60   Input ~ 0
OLED_IIC
Entry Wire Line
	5450 4700 5550 4800
$Comp
L SW_Push SW1
U 1 1 5AFC44C9
P 3650 4500
F 0 "SW1" H 3700 4600 50  0000 L CNN
F 1 "SW_Push" H 3650 4440 50  0000 C CNN
F 2 "" H 3650 4700 50  0001 C CNN
F 3 "" H 3650 4700 50  0001 C CNN
	1    3650 4500
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 5AFC4539
P 5450 4550
F 0 "R1" V 5530 4550 50  0000 C CNN
F 1 "4K7" V 5450 4550 50  0000 C CNN
F 2 "" V 5380 4550 50  0001 C CNN
F 3 "" H 5450 4550 50  0001 C CNN
	1    5450 4550
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW2
U 1 1 5AFC46EA
P 5450 4200
F 0 "SW2" H 5500 4300 50  0000 L CNN
F 1 "SW_Push" H 5450 4140 50  0000 C CNN
F 2 "" H 5450 4400 50  0001 C CNN
F 3 "" H 5450 4400 50  0001 C CNN
	1    5450 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	4500 3900 5450 3900
Wire Wire Line
	5450 3900 5450 4000
Entry Wire Line
	3650 4700 3750 4800
Entry Wire Line
	3650 4000 3750 3900
$Comp
L R R3
U 1 1 5AFC49A3
P 3650 4150
F 0 "R3" V 3730 4150 50  0000 C CNN
F 1 "4K7" V 3650 4150 50  0000 C CNN
F 2 "" V 3580 4150 50  0001 C CNN
F 3 "" H 3650 4150 50  0001 C CNN
	1    3650 4150
	1    0    0    -1  
$EndComp
Entry Wire Line
	2850 3800 2950 3900
Entry Wire Line
	3150 4000 3250 4100
Wire Wire Line
	3150 3750 3150 4000
Wire Wire Line
	2850 3800 2850 3650
Wire Bus Line
	2400 2800 6050 2800
Entry Wire Line
	3150 2900 3250 2800
Entry Wire Line
	2850 2900 2950 2800
Entry Wire Line
	2700 2900 2800 2800
Wire Wire Line
	2700 2900 2700 3700
Wire Wire Line
	2850 3350 2850 2900
Wire Wire Line
	3150 2900 3150 3450
$EndSCHEMATC
