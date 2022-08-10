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
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 60BF7527
P 5400 3600
F 0 "A1" H 5400 2511 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 5400 2420 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 5400 3600 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 5400 3600 50  0001 C CNN
	1    5400 3600
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:TIP120 Q1
U 1 1 60BFBFBD
P 4700 5300
F 0 "Q1" H 4907 5346 50  0000 L CNN
F 1 "TIP120" H 4907 5255 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4900 5225 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/TIP120-D.PDF" H 4700 5300 50  0001 L CNN
	1    4700 5300
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 60BFF19D
P 2650 2550
F 0 "J2" H 2650 2850 50  0000 R CNN
F 1 "Conn_01x02_Male" H 2800 2700 50  0000 R CNN
F 2 "" H 2650 2550 50  0001 C CNN
F 3 "~" H 2650 2550 50  0001 C CNN
	1    2650 2550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x08_Male J3
U 1 1 60BFFC2B
P 9300 5350
F 0 "J3" H 9450 5900 50  0000 R CNN
F 1 "Conn_01x08_Male" H 9500 5800 50  0000 R CNN
F 2 "" H 9300 5350 50  0001 C CNN
F 3 "~" H 9300 5350 50  0001 C CNN
	1    9300 5350
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Male J1
U 1 1 60C0124F
P 2650 1550
F 0 "J1" H 2758 1831 50  0000 C CNN
F 1 "Conn_01x03_Male" H 2758 1740 50  0000 C CNN
F 2 "" H 2650 1550 50  0001 C CNN
F 3 "~" H 2650 1550 50  0001 C CNN
	1    2650 1550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 60C07D86
P 3200 1750
F 0 "#PWR02" H 3200 1500 50  0001 C CNN
F 1 "GND" H 3205 1577 50  0000 C CNN
F 2 "" H 3200 1750 50  0001 C CNN
F 3 "" H 3200 1750 50  0001 C CNN
	1    3200 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 1650 3200 1650
Wire Wire Line
	3200 1650 3200 1750
$Comp
L Device:R R1
U 1 1 60C09F42
P 3850 4350
F 0 "R1" H 3780 4304 50  0000 R CNN
F 1 "2k" H 3780 4395 50  0000 R CNN
F 2 "" V 3780 4350 50  0001 C CNN
F 3 "~" H 3850 4350 50  0001 C CNN
	1    3850 4350
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 60C0B6A3
P 4150 4350
F 0 "R2" H 4080 4304 50  0000 R CNN
F 1 "2k" H 4080 4395 50  0000 R CNN
F 2 "" V 4080 4350 50  0001 C CNN
F 3 "~" H 4150 4350 50  0001 C CNN
	1    4150 4350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 60C16395
P 4800 5600
F 0 "#PWR04" H 4800 5350 50  0001 C CNN
F 1 "GND" H 4805 5427 50  0000 C CNN
F 2 "" H 4800 5600 50  0001 C CNN
F 3 "" H 4800 5600 50  0001 C CNN
	1    4800 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 4200 4150 3600
Wire Wire Line
	4150 3600 4900 3600
Wire Wire Line
	3850 4200 3850 3500
Wire Wire Line
	3850 3500 4900 3500
Wire Wire Line
	4150 4500 4150 5300
Wire Wire Line
	4150 5300 4500 5300
Wire Wire Line
	4800 5500 4800 5600
Wire Wire Line
	4950 6000 3850 6000
Wire Wire Line
	3850 6000 3850 4500
Wire Wire Line
	5250 6200 5250 6300
Wire Wire Line
	5250 5800 5250 5750
Wire Wire Line
	5250 5750 7700 5750
Wire Wire Line
	4800 5100 4800 5050
Wire Wire Line
	4800 5050 7700 5050
Wire Wire Line
	7700 5050 7700 5450
$Comp
L Device:D D2
U 1 1 60C1B556
P 8150 5650
F 0 "D2" H 8100 5500 50  0000 C CNN
F 1 "D" H 8200 5450 50  0000 C CNN
F 2 "" H 8150 5650 50  0001 C CNN
F 3 "~" H 8150 5650 50  0001 C CNN
	1    8150 5650
	-1   0    0    -1  
$EndComp
$Comp
L Device:D D1
U 1 1 60C1CEEA
P 8150 5450
F 0 "D1" H 8150 5667 50  0000 C CNN
F 1 "D" H 8150 5576 50  0000 C CNN
F 2 "" H 8150 5450 50  0001 C CNN
F 3 "~" H 8150 5450 50  0001 C CNN
	1    8150 5450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8000 5450 7700 5450
Connection ~ 7700 5450
Wire Wire Line
	7700 5450 7700 5550
Wire Wire Line
	8000 5650 7700 5650
Wire Wire Line
	7700 5650 7700 5750
Connection ~ 7700 5750
$Comp
L power:+12V #PWR09
U 1 1 60C1ECDF
P 8350 5300
F 0 "#PWR09" H 8350 5150 50  0001 C CNN
F 1 "+12V" H 8365 5473 50  0000 C CNN
F 2 "" H 8350 5300 50  0001 C CNN
F 3 "" H 8350 5300 50  0001 C CNN
	1    8350 5300
	1    0    0    -1  
$EndComp
Connection ~ 8350 5450
Wire Wire Line
	8350 5450 8300 5450
$Comp
L Transistor_BJT:TIP120 Q2
U 1 1 60BF9145
P 5150 6000
F 0 "Q2" H 5357 6046 50  0000 L CNN
F 1 "TIP120" H 5357 5955 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 5350 5925 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/TIP120-D.PDF" H 5150 6000 50  0001 L CNN
	1    5150 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 60C16B30
P 5250 6300
F 0 "#PWR05" H 5250 6050 50  0001 C CNN
F 1 "GND" H 5255 6127 50  0000 C CNN
F 2 "" H 5250 6300 50  0001 C CNN
F 3 "" H 5250 6300 50  0001 C CNN
	1    5250 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 5750 9100 5750
Wire Wire Line
	7700 5550 9100 5550
Wire Wire Line
	8350 5450 9100 5450
$Comp
L power:+12V #PWR010
U 1 1 60C27646
P 8550 5300
F 0 "#PWR010" H 8550 5150 50  0001 C CNN
F 1 "+12V" H 8565 5473 50  0000 C CNN
F 2 "" H 8550 5300 50  0001 C CNN
F 3 "" H 8550 5300 50  0001 C CNN
	1    8550 5300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR011
U 1 1 60C27FE7
P 8900 4850
F 0 "#PWR011" H 8900 4700 50  0001 C CNN
F 1 "+5V" H 8915 5023 50  0000 C CNN
F 2 "" H 8900 4850 50  0001 C CNN
F 3 "" H 8900 4850 50  0001 C CNN
	1    8900 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 60C28A84
P 8900 5300
F 0 "#PWR012" H 8900 5050 50  0001 C CNN
F 1 "GND" H 8800 5300 50  0000 C CNN
F 2 "" H 8900 5300 50  0001 C CNN
F 3 "" H 8900 5300 50  0001 C CNN
	1    8900 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 60C29326
P 6950 4750
F 0 "R3" H 7200 4750 50  0000 R CNN
F 1 "6k8" H 6880 4795 50  0000 R CNN
F 2 "" V 6880 4750 50  0001 C CNN
F 3 "~" H 6950 4750 50  0001 C CNN
	1    6950 4750
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 60C2D2C5
P 6950 4550
F 0 "#PWR06" H 6950 4400 50  0001 C CNN
F 1 "+5V" H 6965 4723 50  0000 C CNN
F 2 "" H 6950 4550 50  0001 C CNN
F 3 "" H 6950 4550 50  0001 C CNN
	1    6950 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 5050 8900 5050
Wire Wire Line
	8900 5050 8900 4850
Wire Wire Line
	9100 5150 8800 5150
Wire Wire Line
	8800 5150 8800 4950
Wire Wire Line
	8800 4950 6950 4950
Wire Wire Line
	6950 4950 6950 4900
Wire Wire Line
	6950 4950 4500 4950
Wire Wire Line
	4500 4950 4500 3200
Wire Wire Line
	4500 3200 4900 3200
Connection ~ 6950 4950
Wire Wire Line
	6950 4550 6950 4600
Wire Wire Line
	9100 5250 8900 5250
Wire Wire Line
	8900 5250 8900 5300
$Comp
L power:+5V #PWR07
U 1 1 60C32179
P 7400 2600
F 0 "#PWR07" H 7400 2450 50  0001 C CNN
F 1 "+5V" H 7415 2773 50  0000 C CNN
F 2 "" H 7400 2600 50  0001 C CNN
F 3 "" H 7400 2600 50  0001 C CNN
	1    7400 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 60C3270F
P 7400 3550
F 0 "#PWR08" H 7400 3300 50  0001 C CNN
F 1 "GND" H 7405 3377 50  0000 C CNN
F 2 "" H 7400 3550 50  0001 C CNN
F 3 "" H 7400 3550 50  0001 C CNN
	1    7400 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 5650 8550 5650
Wire Wire Line
	8350 5300 8350 5450
Wire Wire Line
	8550 5650 8550 5300
Connection ~ 8550 5650
Wire Wire Line
	8550 5650 9100 5650
$Comp
L power:GND #PWR?
U 1 1 60C38D1C
P 5950 4650
F 0 "#PWR?" H 5950 4400 50  0001 C CNN
F 1 "GND" H 5955 4477 50  0000 C CNN
F 2 "" H 5950 4650 50  0001 C CNN
F 3 "" H 5950 4650 50  0001 C CNN
	1    5950 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 60C3913C
P 5600 2450
F 0 "#PWR?" H 5600 2300 50  0001 C CNN
F 1 "+5V" H 5615 2623 50  0000 C CNN
F 2 "" H 5600 2450 50  0001 C CNN
F 3 "" H 5600 2450 50  0001 C CNN
	1    5600 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2600 5600 2450
$Comp
L power:+12V #PWR?
U 1 1 60C3B09F
P 3200 1400
F 0 "#PWR?" H 3200 1250 50  0001 C CNN
F 1 "+12V" H 3215 1573 50  0000 C CNN
F 2 "" H 3200 1400 50  0001 C CNN
F 3 "" H 3200 1400 50  0001 C CNN
	1    3200 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 2600 5300 2450
Wire Wire Line
	5500 4600 5950 4600
Wire Wire Line
	5950 4600 5950 4650
Wire Wire Line
	2850 1450 3200 1450
Wire Wire Line
	3200 1450 3200 1400
$Comp
L power:+5V #PWR?
U 1 1 62F3CBAE
P 3200 2450
F 0 "#PWR?" H 3200 2300 50  0001 C CNN
F 1 "+5V" H 3215 2623 50  0000 C CNN
F 2 "" H 3200 2450 50  0001 C CNN
F 3 "" H 3200 2450 50  0001 C CNN
	1    3200 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62F3DE25
P 3200 2800
F 0 "#PWR?" H 3200 2550 50  0001 C CNN
F 1 "GND" H 3205 2627 50  0000 C CNN
F 2 "" H 3200 2800 50  0001 C CNN
F 3 "" H 3200 2800 50  0001 C CNN
	1    3200 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 2550 3200 2550
Wire Wire Line
	3200 2550 3200 2450
Wire Wire Line
	3200 2800 3200 2650
Wire Wire Line
	3200 2650 2850 2650
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 62F408FD
P 9200 3050
F 0 "J?" H 9550 3400 50  0000 R CNN
F 1 "Conn_01x04_Male" H 9550 3300 50  0000 R CNN
F 2 "" H 9200 3050 50  0001 C CNN
F 3 "~" H 9200 3050 50  0001 C CNN
	1    9200 3050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5900 3600 6750 3600
Wire Wire Line
	6750 3600 6750 3250
Wire Wire Line
	6750 3050 9000 3050
Wire Wire Line
	9000 3150 7400 3150
Wire Wire Line
	7400 3150 7400 3550
Wire Wire Line
	9000 3250 6750 3250
Connection ~ 6750 3250
Wire Wire Line
	6750 3250 6750 3050
Wire Wire Line
	9000 2950 7400 2950
Wire Wire Line
	7400 2950 7400 2600
Text Notes 9400 3100 0    50   ~ 0
GX12 - 4 wire\nKUS/etape level sensor
Text Notes 9550 5400 0    50   ~ 0
GX12 - 8 wire\nSolenoids and flow sensor
Text Notes 1050 1550 0    50   ~ 0
GX12 - 3 wire (2 needed)\nFrom standard 12V transformer\n
Text Notes 1050 2700 0    50   ~ 0
GX12 - 2 wire\nTo RPI 4 (2.5-3A) adapter
$EndSCHEMATC
