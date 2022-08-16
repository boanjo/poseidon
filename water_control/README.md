# Water Control

The water control is built from a water level sensor (2 alternatives below) a few solenoids (one for the pond and pone for the garden plants micro drip system) for adding tap water and also measuring the water amount used. You need to print a few parts with your 3D printer. I'm using PETG with support on most of the printouts so that it can resist the outdoor UV. I.m just doing basic sanding (paper and file) and then put some spray paint on it to look nice.


For the water level control i have 2 differnt solutions, where the first one is what i currently use (see alternative 2 at the bottom - most parts are common):
## Alternative 1: KUS liquid level sensor
The sensor has a magnet in the floating piece which controls reed switches inside the steel rod to output different resistance at differnt levels. I'm using European standard sensor where the resistance is between 0-180ohm and it's priced at ~20$. To make the two solutions compatible i've added a resistance to the outer two pins as the fixed reference in a voltage divider (see below). Even if the accuracy for the sensors is 21mm (the step between the reed switches) you can make the level full/low toggle on a +-5mm accuracy due the nature of the reed switches. I.e. when the level goes down the reed switch will continue to be activated for maybe 5mm until the next lower one gets activated. Same goes in the other direction so this means that the accuracy for the range where values toogle between is 10mm or less. So just set min and max between those levels and you are good to go. Of course if you want to use the sensor for detailed monitoring of the level you will not get better than 21mm accuracy and you should probably go for the etape solution.  
![0](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_kus.png?raw=true "KUS liquid level sensor")

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


See stl_step folder models (download and edit STEP or just print STL):
![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_model_KUS.PNG?raw=true "Model to mount KUS level sensor")


On the RPI there is a serial2mqtt.py application running and as the name indicates it bridges between the serial commands and the mqtt GW.

You need a small proto baord (the Adafruit Perma-Proto 1/2 Sized Breadboard is a great fit) for all the connections and the Arduino nano, schematics here. The RPI needs a standard RPI4 power supply (i've cut mine with a pair of GX12 2 pin connectors to get it into the box). You also need a 12 transformer for the solenoids (cut in the same way but with a 3 pin to not make any misstake). Then there is one 4 pin GX12 connector for the KUS or etape.
![8](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_water_control_schematic.JPG?raw=true "Schematic")

Notice the mounted resistor of 100ohm (needed for the voltage divider) inside the KUS level sensor to make it pin compatible with the etape (Pin 1 & 4 for Rref and middle 2,3 for Rsense i.e. the variable part)
![9](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_water_control_resistor.JPG?raw=true "Resistor")

Everything is hosted in the same box as the RPI4
![9](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_water_control_box.JPG?raw=true "Box")

Here is a video covering the Water Control with coding and usage: https://youtu.be/l2im_r2Qxn0

Here is a YouTube video explaining the KUS liquid level setup: https://youtu.be/7Df8thEyxGE
Here is the full Poseidon playlist: https://www.youtube.com/watch?v=6Q65v6jY_po&list=PL4uTTzIGocpXYQ4JUedOa0raxNH4VkoVa

## Alternative 2: Milone technology etape
The sensor is quite sensitive and this is why i have move to the float switch solution instead. I love the etape due to it's accuracy but it has broken down on me a few time too many (resistance all of a sudden goes off the datasheet range like max 2.2k is all of a sudden 9k etc) and you of course need something robust when dealing with water.
![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_etape.JPG?raw=true "Etape")

![2](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_wc_parts.JPG?raw=true "3D printed parts")

See stl_step folder models (download and edit STEP or just print STL):
![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_model_etape.PNG?raw=true "Model to mount etape level sensor")

Here is a YouTube video explaining the etape liquid level setup: https://youtu.be/HrQJPjmJSWM


Poseidon - Pond Control 2.0 by Anders Johansson is marked with CC0 1.0. To view a copy of this license, visit http://creativecommons.org/publicdomain/zero/1.0