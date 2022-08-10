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
L MCU_Module:WeMos_D1_mini U1
U 1 1 62F39A1A
P 2500 4100
F 0 "U1" H 2500 3211 50  0000 C CNN
F 1 "WeMos_D1_mini" H 2500 3120 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 2500 2950 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H 650 2950 50  0001 C CNN
	1    2500 4100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 62F3A1FA
P 5200 4200
F 0 "J2" H 5150 4300 50  0000 C CNN
F 1 "Conn_01x02_Male" H 5500 4400 50  0000 C CNN
F 2 "" H 5200 4200 50  0001 C CNN
F 3 "~" H 5200 4200 50  0001 C CNN
	1    5200 4200
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_Servo M1
U 1 1 62F3AC51
P 1550 2050
F 0 "M1" H 1544 2394 50  0000 C CNN
F 1 "Motor_Servo" H 1544 2303 50  0000 C CNN
F 2 "" H 1550 1860 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 1550 1860 50  0001 C CNN
	1    1550 2050
	-1   0    0    -1  
$EndComp
$Comp
L Device:CP1 C1
U 1 1 62F3C7A1
P 3600 4250
F 0 "C1" H 3715 4296 50  0000 L CNN
F 1 "CP1" H 3715 4205 50  0000 L CNN
F 2 "" H 3600 4250 50  0001 C CNN
F 3 "~" H 3600 4250 50  0001 C CNN
	1    3600 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 62F3D660
P 6850 4400
F 0 "#PWR02" H 6850 4150 50  0001 C CNN
F 1 "GND" H 6855 4227 50  0000 C CNN
F 2 "" H 6850 4400 50  0001 C CNN
F 3 "" H 6850 4400 50  0001 C CNN
	1    6850 4400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 62F3DEAD
P 6850 3950
F 0 "#PWR01" H 6850 3800 50  0001 C CNN
F 1 "+5V" H 6865 4123 50  0000 C CNN
F 2 "" H 6850 3950 50  0001 C CNN
F 3 "" H 6850 3950 50  0001 C CNN
	1    6850 3950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 62F40224
P 4800 4200
F 0 "J1" H 4700 4300 50  0000 R CNN
F 1 "Conn_01x02_Male" H 5150 4400 50  0000 R CNN
F 2 "" H 4800 4200 50  0001 C CNN
F 3 "~" H 4800 4200 50  0001 C CNN
	1    4800 4200
	-1   0    0    -1  
$EndComp
Text Notes 4400 4500 0    79   ~ 0
2pol standard terminal block
Text Notes 7250 4200 0    79   ~ 0
Standard (min 500mA) USB plug and cable
Wire Wire Line
	6850 3950 6850 4200
Wire Wire Line
	6850 4200 5400 4200
Wire Wire Line
	6850 4400 6850 4300
Wire Wire Line
	6850 4300 5400 4300
Wire Wire Line
	4600 4200 4300 4200
Wire Wire Line
	4600 4300 4300 4300
Wire Wire Line
	2500 4900 3600 4900
Connection ~ 3600 4900
Wire Wire Line
	2400 3300 2400 3000
Wire Wire Line
	3600 4900 4300 4900
Wire Wire Line
	4300 3000 3600 3000
Connection ~ 3600 3000
Wire Wire Line
	4300 3000 4300 4200
Wire Wire Line
	3600 3000 3600 4100
Wire Wire Line
	4300 4300 4300 4900
Wire Wire Line
	3600 4400 3600 4900
Wire Wire Line
	2400 3000 3600 3000
Wire Wire Line
	2900 4000 3100 4000
Wire Wire Line
	3100 4000 3100 1950
Wire Wire Line
	3100 1950 1850 1950
Wire Wire Line
	1850 2050 2400 2050
Wire Wire Line
	2400 2050 2400 3000
Connection ~ 2400 3000
Wire Wire Line
	2500 4900 1850 4900
Wire Wire Line
	1850 4900 1850 2150
Connection ~ 2500 4900
Text Notes 3700 4100 0    40   ~ 0
100uF or more
$EndSCHEMATC
