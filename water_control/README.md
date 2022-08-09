# Water Control

The water control is built from a water level sensor (2 alternatives below) a few solenoids (one for the pond and pone for the garden plants micro drip system) for adding tap water and also measuring the water amount used. You need to print a few parts with your 3D printer. I'm using PETG with support on most of the printouts so that it can resist the outdoor UV. I.m just doing basic sanding (paper and file) and then put some spray paint on it to look nice.


For the water level control i have 2 differnt solutions, where the first one is what i currently use:
## KUS liquid level sensor
The sensor has a magnet in the floating piece which controls reed switches inside the steel rod to output different resistance at differnt levels. I'm using European standard sensor where the resistance is between 0-180ohm and it's priced at ~20$. To make the two solutions compatible i've added a resistance to the outer two pins as the fixed reference in a voltage divider (see below). Even if the accuracy for the sensors is 21mm (the step between the reed switches) you can make the level full/low toggle on a +-5mm accuracy due the nature of the reed switches. I.e. when the level goes down the reed switch will continue to be activated for maybe 5mm until the next lower one gets activated. Same goes in the other direction so this means that the accuracy for the range where values toogle between is 10mm or less. So just set min and max between those levels and you are good to go. Of course if you want to use the sensor for detailed monitoring of the level you will not get better than 21mm accuracy and you should probably go for the etape solution.  
![0](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_kus.png?raw=true "KUS liquid level sensor")

## Milone technology etape
The sensor is quite sensitive and this is why i have move to the float switch solution instead. I love the etape due to it's accuracy but it has broken down on me a few time too many (resistance all of a sudden goes off the datasheet range like max 2.2k is all of a sudden 9k etc) and you of course need something robust when dealing with water.
![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_etape.JPG?raw=true "Etape")

![2](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_wc_parts.JPG?raw=true "3D printed parts")

## General for both solutions:

An Arduino Nano is used for the regulated loop and has some auto cut off for the water in some suspicious situations. Basically a high and low threshold is built in (and can be changed) and then the water control will try to keep that level. The levels are measured at a frequency of every 3:d second (can be changed) and passes through a ringbuffer of 8 where the average is calculated from. This applies to both solutions and even if you can set the ringbuffer to something smaller you risk getting ping-ponging in case of movements from the fish or strong winds. 

Some plumbing needed for the water, 2 solenoids (2x7$) and water flow sensor (~15$). To get a good pressure and a filter i've integrated it with a gardena micro drip system (~25$). And the plumbing parts are probably another 30-40$.
![3](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_plumbing.JPG?raw=true "Plumbing")


I've hidden the water control parts in 2 cement pots. This part of the system can be left out, and the only thing i do before winter time is to attach the compressor, open all the solenoids and blow the system "clean" of water. 
![4](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_concrete.JPG?raw=true "Concrete")


Pretty easy to hide
![5](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_hidden.JPG?raw=true "Hidden")

For programming of the firmware i've installed the platformio cli version https://docs.platformio.org/en/latest/core/index.html on the raspberry pi. I.e. i'm running an ssh connection with for example putty or mobixterm to the RPI and edit the SW using emacs, git, ino etc directly into the cloned repo. You need to stop the docker service that is communicating with the arduino to release the serial port (e.g. "docker stop poseidon_water_control_1"), then build and upload (see the CMD file for the sequence i use "source CMD" to execute) and then start the service again ((e.g. "docker start poseidon_water_control_1").

The firmware basically consists of and serial command input handler to be able to act on commands from the controller, setting levels etc. Then there is a small interrupt function that calculates any triggers from the water flow sensor and then in the loop a (quite sketchy ;-)) conversion of ticks into liters (you probably need to calibrate it to get as good flow accuracy). The percentage level of the KUS or etape sensor is done using a voltage divider. By using the ADC value (max 1024) you can then get the relative % (For more details see the source code). Once the level reaches a low point this triggers (if auto level is on) the water solenoid to open. Once the max is reached the water stops to fill again. The sensor blindly updates all values and states every sample period which is set to default every 3 seconds (or immediately if ack is requested).   
![6](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_voltage_divider.png?raw=true "Voltage Divider")

On the RPI there is a serial2mqtt.py application running and as the name indicates it bridges between the serial commands and the mqtt GW.


Here is a YouTube video from some of the building step and any programming needed.
