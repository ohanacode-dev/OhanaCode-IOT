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
L Display_Character:LTS-6980HR AFF1
U 1 1 5FE1EBF7
P 9700 2350
F 0 "AFF1" H 9700 3017 50  0000 C CNN
F 1 "LTS-6980HR" H 9700 2926 50  0000 C CNN
F 2 "Display_7Segment:7SegmentLED_LTS6760_LTS6780" H 9700 1750 50  0001 C CNN
F 3 "http://datasheet.octopart.com/LTS-6960HR-Lite-On-datasheet-11803242.pdf" H 9700 2350 50  0001 C CNN
	1    9700 2350
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:LTS-6980HR AFF2
U 1 1 5FE1FE03
P 9700 3650
F 0 "AFF2" H 9700 4317 50  0000 C CNN
F 1 "LTS-6980HR" H 9700 4226 50  0000 C CNN
F 2 "Display_7Segment:7SegmentLED_LTS6760_LTS6780" H 9700 3050 50  0001 C CNN
F 3 "http://datasheet.octopart.com/LTS-6960HR-Lite-On-datasheet-11803242.pdf" H 9700 3650 50  0001 C CNN
	1    9700 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 5FE20A8E
P 8050 1950
F 0 "R11" V 8050 1900 50  0000 C CNN
F 1 "270R" V 8000 2150 50  0000 C CNN
F 2 "" V 7980 1950 50  0001 C CNN
F 3 "~" H 8050 1950 50  0001 C CNN
	1    8050 1950
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 5FE227C7
P 8050 2050
F 0 "R12" V 8050 2000 50  0000 C CNN
F 1 "270R" V 8000 2250 50  0000 C CNN
F 2 "" V 7980 2050 50  0001 C CNN
F 3 "~" H 8050 2050 50  0001 C CNN
	1    8050 2050
	0    1    1    0   
$EndComp
$Comp
L Device:R R13
U 1 1 5FE22AEE
P 8050 2150
F 0 "R13" V 8050 2100 50  0000 C CNN
F 1 "270R" V 8000 2350 50  0000 C CNN
F 2 "" V 7980 2150 50  0001 C CNN
F 3 "~" H 8050 2150 50  0001 C CNN
	1    8050 2150
	0    1    1    0   
$EndComp
$Comp
L Device:R R14
U 1 1 5FE22D9B
P 8050 2250
F 0 "R14" V 8050 2200 50  0000 C CNN
F 1 "270R" V 8000 2450 50  0000 C CNN
F 2 "" V 7980 2250 50  0001 C CNN
F 3 "~" H 8050 2250 50  0001 C CNN
	1    8050 2250
	0    1    1    0   
$EndComp
$Comp
L Device:R R15
U 1 1 5FE2305F
P 8050 2350
F 0 "R15" V 8050 2300 50  0000 C CNN
F 1 "270R" V 8000 2550 50  0000 C CNN
F 2 "" V 7980 2350 50  0001 C CNN
F 3 "~" H 8050 2350 50  0001 C CNN
	1    8050 2350
	0    1    1    0   
$EndComp
$Comp
L Device:R R16
U 1 1 5FE23365
P 8050 2450
F 0 "R16" V 8050 2400 50  0000 C CNN
F 1 "270R" V 8000 2650 50  0000 C CNN
F 2 "" V 7980 2450 50  0001 C CNN
F 3 "~" H 8050 2450 50  0001 C CNN
	1    8050 2450
	0    1    1    0   
$EndComp
$Comp
L Device:R R17
U 1 1 5FE236AD
P 8050 2550
F 0 "R17" V 8050 2500 50  0000 C CNN
F 1 "270R" V 8000 2750 50  0000 C CNN
F 2 "" V 7980 2550 50  0001 C CNN
F 3 "~" H 8050 2550 50  0001 C CNN
	1    8050 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	8200 1950 9100 1950
Wire Wire Line
	8200 2050 9050 2050
Wire Wire Line
	8200 2150 9000 2150
Wire Wire Line
	8200 2250 8950 2250
Wire Wire Line
	8200 2350 8900 2350
Wire Wire Line
	8200 2450 8850 2450
Wire Wire Line
	8200 2550 8800 2550
Wire Wire Line
	9100 1950 9100 3250
Wire Wire Line
	9100 3250 9200 3250
Connection ~ 9100 1950
Wire Wire Line
	9100 1950 9200 1950
Wire Wire Line
	9200 3350 9050 3350
Wire Wire Line
	9050 3350 9050 2050
Connection ~ 9050 2050
Wire Wire Line
	9050 2050 9200 2050
Wire Wire Line
	9000 2150 9000 3450
Wire Wire Line
	9000 3450 9200 3450
Connection ~ 9000 2150
Wire Wire Line
	9000 2150 9200 2150
Wire Wire Line
	9200 3550 8950 3550
Wire Wire Line
	8950 3550 8950 2250
Connection ~ 8950 2250
Wire Wire Line
	8950 2250 9200 2250
Wire Wire Line
	8900 2350 8900 3650
Wire Wire Line
	8900 3650 9200 3650
Connection ~ 8900 2350
Wire Wire Line
	8900 2350 9200 2350
Wire Wire Line
	9200 3750 8850 3750
Wire Wire Line
	8850 3750 8850 2450
Connection ~ 8850 2450
Wire Wire Line
	8850 2450 9200 2450
Wire Wire Line
	8800 2550 8800 3850
Wire Wire Line
	8800 3850 9200 3850
Connection ~ 8800 2550
Wire Wire Line
	8800 2550 9200 2550
Wire Wire Line
	10200 1950 10200 2050
Wire Wire Line
	10200 3250 10200 3350
$Comp
L power:GND #PWR05
U 1 1 5FE33F63
P 6350 4150
F 0 "#PWR05" H 6350 3900 50  0001 C CNN
F 1 "GND" H 6355 3977 50  0000 C CNN
F 2 "" H 6350 4150 50  0001 C CNN
F 3 "" H 6350 4150 50  0001 C CNN
	1    6350 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_BCE Q5
U 1 1 5FE353D0
P 8150 3250
F 0 "Q5" H 8341 3296 50  0000 L CNN
F 1 "Q_NPN_BCE" H 8341 3205 50  0000 L CNN
F 2 "" H 8350 3350 50  0001 C CNN
F 3 "~" H 8150 3250 50  0001 C CNN
	1    8150 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_BCE Q6
U 1 1 5FE37215
P 8150 4600
F 0 "Q6" H 8341 4646 50  0000 L CNN
F 1 "Q_NPN_BCE" H 8341 4555 50  0000 L CNN
F 2 "" H 8350 4700 50  0001 C CNN
F 3 "~" H 8150 4600 50  0001 C CNN
	1    8150 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 3000 10350 3000
Wire Wire Line
	10350 3000 10350 2050
Wire Wire Line
	10350 2050 10200 2050
Connection ~ 10200 2050
Wire Wire Line
	10200 3350 10350 3350
Connection ~ 10200 3350
Wire Wire Line
	8250 4400 10350 4400
Wire Wire Line
	10350 4400 10350 3350
$Comp
L Device:R R9
U 1 1 5FE3BDC7
P 7700 3250
F 0 "R9" V 7493 3250 50  0000 C CNN
F 1 "R" V 7584 3250 50  0000 C CNN
F 2 "" V 7630 3250 50  0001 C CNN
F 3 "~" H 7700 3250 50  0001 C CNN
	1    7700 3250
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 5FE3C5E1
P 7700 4600
F 0 "R10" V 7493 4600 50  0000 C CNN
F 1 "R" V 7584 4600 50  0000 C CNN
F 2 "" V 7630 4600 50  0001 C CNN
F 3 "~" H 7700 4600 50  0001 C CNN
	1    7700 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	7850 4600 7950 4600
$Comp
L RF_Module:ESP-12F U1
U 1 1 5FE45E22
P 3550 2550
F 0 "U1" H 3200 3350 50  0000 C CNN
F 1 "ESP-12F" H 3500 2550 50  0000 C CNN
F 2 "RF_Module:ESP-12E" H 3550 2550 50  0001 C CNN
F 3 "http://wiki.ai-thinker.com/_media/esp8266/esp8266_series_modules_user_manual_v1.1.pdf" H 3200 2650 50  0001 C CNN
	1    3550 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5FE92727
P 5100 2900
F 0 "R7" V 5100 2850 50  0000 C CNN
F 1 "270R" V 5050 3100 50  0000 C CNN
F 2 "" V 5030 2900 50  0001 C CNN
F 3 "~" H 5100 2900 50  0001 C CNN
	1    5100 2900
	-1   0    0    1   
$EndComp
$Comp
L Device:LED D1
U 1 1 5FE93275
P 5100 3450
F 0 "D1" V 5139 3332 50  0000 R CNN
F 1 "LED" V 5048 3332 50  0000 R CNN
F 2 "" H 5100 3450 50  0001 C CNN
F 3 "~" H 5100 3450 50  0001 C CNN
	1    5100 3450
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5FE96787
P 5100 3700
F 0 "#PWR04" H 5100 3450 50  0001 C CNN
F 1 "GND" H 5105 3527 50  0000 C CNN
F 2 "" H 5100 3700 50  0001 C CNN
F 3 "" H 5100 3700 50  0001 C CNN
	1    5100 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3700 5100 3600
$Comp
L power:GND #PWR06
U 1 1 5FE246C2
P 8250 3550
F 0 "#PWR06" H 8250 3300 50  0001 C CNN
F 1 "GND" H 8255 3377 50  0000 C CNN
F 2 "" H 8250 3550 50  0001 C CNN
F 3 "" H 8250 3550 50  0001 C CNN
	1    8250 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5FE255B4
P 8250 4800
F 0 "#PWR07" H 8250 4550 50  0001 C CNN
F 1 "GND" H 8255 4627 50  0000 C CNN
F 2 "" H 8250 4800 50  0001 C CNN
F 3 "" H 8250 4800 50  0001 C CNN
	1    8250 4800
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATtiny:ATtiny2313-20PU U2
U 1 1 5FE30397
P 6350 2750
F 0 "U2" H 6100 3800 50  0000 C CNN
F 1 "ATtiny2313-20PU" H 6250 2800 50  0000 C CNN
F 2 "Package_DIP:DIP-20_W7.62mm" H 6350 2750 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2543-AVR-ATtiny2313_Datasheet.pdf" H 6350 2750 50  0001 C CNN
	1    6350 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 3450 6950 4600
Wire Wire Line
	6950 4600 7550 4600
Wire Wire Line
	6350 1650 6350 1350
Wire Wire Line
	6350 3850 6350 4150
$Comp
L Device:R R8
U 1 1 5FE6E40E
P 5650 1700
F 0 "R8" V 5443 1700 50  0000 C CNN
F 1 "R" V 5534 1700 50  0000 C CNN
F 2 "" V 5580 1700 50  0001 C CNN
F 3 "~" H 5650 1700 50  0001 C CNN
	1    5650 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 1850 5650 1950
Wire Wire Line
	5650 1950 5750 1950
Wire Wire Line
	5650 1550 5650 1350
Wire Wire Line
	5650 1350 6350 1350
Wire Wire Line
	6950 2850 7150 2850
Wire Wire Line
	7150 2850 7150 900 
Wire Wire Line
	7150 900  5350 900 
Wire Wire Line
	5350 900  5350 2050
Wire Wire Line
	4150 2050 5350 2050
Wire Wire Line
	4150 2950 4300 2950
Wire Wire Line
	5100 3050 5100 3300
Wire Wire Line
	5650 1350 4950 1350
Connection ~ 5650 1350
$Comp
L Device:Jumper JP1
U 1 1 5FEA5E64
P 4650 1350
F 0 "JP1" H 4650 1614 50  0000 C CNN
F 1 "Jumper" H 4650 1523 50  0000 C CNN
F 2 "" H 4650 1350 50  0001 C CNN
F 3 "~" H 4650 1350 50  0001 C CNN
	1    4650 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 1350 3550 1350
Wire Wire Line
	3550 1200 3550 1350
Connection ~ 3550 1350
Wire Wire Line
	3550 1350 3550 1750
Wire Wire Line
	7900 1950 7350 1950
Wire Wire Line
	7350 1950 7350 2650
Wire Wire Line
	7350 2650 6950 2650
Wire Wire Line
	7900 2050 7400 2050
Wire Wire Line
	7400 2050 7400 2550
Wire Wire Line
	7400 2550 6950 2550
Wire Wire Line
	7900 2150 7450 2150
Wire Wire Line
	7450 2150 7450 2450
Wire Wire Line
	7450 2450 6950 2450
Wire Wire Line
	7900 2250 7500 2250
Wire Wire Line
	7500 2250 7500 2350
Wire Wire Line
	7500 2350 6950 2350
Wire Wire Line
	7900 2350 7550 2350
Wire Wire Line
	7550 2350 7550 1900
Wire Wire Line
	7550 1900 7300 1900
Wire Wire Line
	7300 1900 7300 2250
Wire Wire Line
	7300 2250 6950 2250
Wire Wire Line
	7900 2450 7600 2450
Wire Wire Line
	7600 2450 7600 1850
Wire Wire Line
	7600 1850 7250 1850
Wire Wire Line
	7250 1850 7250 2150
Wire Wire Line
	7250 2150 6950 2150
Wire Wire Line
	7900 2550 7650 2550
Wire Wire Line
	7650 2550 7650 1800
Wire Wire Line
	7650 1800 7200 1800
Wire Wire Line
	7200 1800 7200 2050
Wire Wire Line
	7200 2050 6950 2050
$Comp
L Device:Q_NPN_BCE Q4
U 1 1 5FEACBDB
P 6150 5600
F 0 "Q4" H 6341 5646 50  0000 L CNN
F 1 "Q_NPN_BCE" H 6341 5555 50  0000 L CNN
F 2 "" H 6350 5700 50  0001 C CNN
F 3 "~" H 6150 5600 50  0001 C CNN
	1    6150 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_BCE Q3
U 1 1 5FEAD902
P 5650 6000
F 0 "Q3" H 5841 6046 50  0000 L CNN
F 1 "Q_NPN_BCE" H 5841 5955 50  0000 L CNN
F 2 "" H 5850 6100 50  0001 C CNN
F 3 "~" H 5650 6000 50  0001 C CNN
	1    5650 6000
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_BCE Q2
U 1 1 5FEADF59
P 5100 6400
F 0 "Q2" H 5291 6446 50  0000 L CNN
F 1 "Q_NPN_BCE" H 5291 6355 50  0000 L CNN
F 2 "" H 5300 6500 50  0001 C CNN
F 3 "~" H 5100 6400 50  0001 C CNN
	1    5100 6400
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_BCE Q1
U 1 1 5FEAE477
P 4500 6800
F 0 "Q1" H 4691 6846 50  0000 L CNN
F 1 "Q_NPN_BCE" H 4691 6755 50  0000 L CNN
F 2 "" H 4700 6900 50  0001 C CNN
F 3 "~" H 4500 6800 50  0001 C CNN
	1    4500 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 7000 4600 7200
Wire Wire Line
	4600 7200 5200 7200
Wire Wire Line
	5200 7200 5200 6600
Wire Wire Line
	5200 7200 5750 7200
Wire Wire Line
	5750 7200 5750 6200
Connection ~ 5200 7200
Wire Wire Line
	5750 7200 6250 7200
Wire Wire Line
	6250 7200 6250 5800
Connection ~ 5750 7200
$Comp
L Device:R R3
U 1 1 5FEB9BB4
P 4300 3450
F 0 "R3" V 4300 3500 50  0000 C CNN
F 1 "R" V 4350 3300 50  0000 C CNN
F 2 "" V 4230 3450 50  0001 C CNN
F 3 "~" H 4300 3450 50  0001 C CNN
	1    4300 3450
	-1   0    0    1   
$EndComp
$Comp
L Device:R R4
U 1 1 5FEBB728
P 4400 3450
F 0 "R4" V 4400 3500 50  0000 C CNN
F 1 "R" V 4450 3300 50  0000 C CNN
F 2 "" V 4330 3450 50  0001 C CNN
F 3 "~" H 4400 3450 50  0001 C CNN
	1    4400 3450
	-1   0    0    1   
$EndComp
$Comp
L Device:R R5
U 1 1 5FEBBABB
P 4500 3450
F 0 "R5" V 4500 3500 50  0000 C CNN
F 1 "R" V 4550 3300 50  0000 C CNN
F 2 "" V 4430 3450 50  0001 C CNN
F 3 "~" H 4500 3450 50  0001 C CNN
	1    4500 3450
	-1   0    0    1   
$EndComp
$Comp
L Device:R R6
U 1 1 5FEBBE76
P 4600 3450
F 0 "R6" V 4600 3500 50  0000 C CNN
F 1 "R" V 4650 3300 50  0000 C CNN
F 2 "" V 4530 3450 50  0001 C CNN
F 3 "~" H 4600 3450 50  0001 C CNN
	1    4600 3450
	-1   0    0    1   
$EndComp
$Comp
L Motor:Stepper_Motor_unipolar_5pin M1
U 1 1 5FED6C36
P 2850 4950
F 0 "M1" H 3038 5074 50  0000 L CNN
F 1 "Stepper_Motor_unipolar_5pin" H 3038 4983 50  0000 L CNN
F 2 "" H 2860 4940 50  0001 C CNN
F 3 "http://www.infineon.com/dgdl/Application-Note-TLE8110EE_driving_UniPolarStepperMotor_V1.1.pdf?fileId=db3a30431be39b97011be5d0aa0a00b0" H 2860 4940 50  0001 C CNN
	1    2850 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5FEDAE58
P 2650 4250
F 0 "R1" V 2650 4300 50  0000 C CNN
F 1 "R" V 2700 4100 50  0000 C CNN
F 2 "" V 2580 4250 50  0001 C CNN
F 3 "~" H 2650 4250 50  0001 C CNN
	1    2650 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	2650 4400 2650 4650
$Comp
L power:+12V #PWR01
U 1 1 5FEE0741
P 2650 3950
F 0 "#PWR01" H 2650 3800 50  0001 C CNN
F 1 "+12V" H 2665 4123 50  0000 C CNN
F 2 "" H 2650 3950 50  0001 C CNN
F 3 "" H 2650 3950 50  0001 C CNN
	1    2650 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 3950 2650 4100
$Comp
L power:+3V3 #PWR02
U 1 1 5FEE6EF2
P 3550 1200
F 0 "#PWR02" H 3550 1050 50  0001 C CNN
F 1 "+3V3" H 3565 1373 50  0000 C CNN
F 2 "" H 3550 1200 50  0001 C CNN
F 3 "" H 3550 1200 50  0001 C CNN
	1    3550 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3300 4300 2950
Wire Wire Line
	4150 2750 4400 2750
Wire Wire Line
	4400 2750 4400 3300
Wire Wire Line
	4150 2650 4500 2650
Wire Wire Line
	4500 2650 4500 3300
Wire Wire Line
	4150 2550 4600 2550
Wire Wire Line
	4600 2550 4600 3300
Wire Wire Line
	2750 4650 2750 4550
Wire Wire Line
	6250 4550 6250 5400
Wire Wire Line
	2550 4850 2450 4850
Wire Wire Line
	4300 3600 4300 6800
Wire Wire Line
	4400 6400 4900 6400
Wire Wire Line
	4400 3600 4400 6400
Wire Wire Line
	4500 3600 4500 6000
Wire Wire Line
	4500 6000 5450 6000
Wire Wire Line
	4600 5600 4600 3600
Wire Wire Line
	4600 5600 5950 5600
Wire Wire Line
	4600 6600 2450 6600
Wire Wire Line
	2450 4850 2450 6600
Wire Wire Line
	5200 6200 2550 6200
Wire Wire Line
	2550 6200 2550 5050
Wire Wire Line
	2950 4650 5750 4650
Wire Wire Line
	5750 4650 5750 5800
Wire Wire Line
	2750 4550 6250 4550
Wire Wire Line
	8250 3550 8250 3450
Wire Wire Line
	6950 3250 7550 3250
Wire Wire Line
	7850 3250 7950 3250
Wire Wire Line
	8250 3000 8250 3050
Wire Wire Line
	3550 3250 3550 3450
$Comp
L power:GND #PWR03
U 1 1 5FFBED4C
P 3550 3450
F 0 "#PWR03" H 3550 3200 50  0001 C CNN
F 1 "GND" H 3555 3277 50  0000 C CNN
F 2 "" H 3550 3450 50  0001 C CNN
F 3 "" H 3550 3450 50  0001 C CNN
	1    3550 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5FFC08A2
P 2900 1650
F 0 "R2" V 2900 1700 50  0000 C CNN
F 1 "R" V 2950 1500 50  0000 C CNN
F 2 "" V 2830 1650 50  0001 C CNN
F 3 "~" H 2900 1650 50  0001 C CNN
	1    2900 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	2900 1800 2900 1950
Wire Wire Line
	2900 1950 2950 1950
Wire Wire Line
	2900 1500 2900 1350
Wire Wire Line
	2900 1350 3550 1350
Wire Wire Line
	4150 2350 5100 2350
Wire Wire Line
	5100 2350 5100 2750
$EndSCHEMATC
