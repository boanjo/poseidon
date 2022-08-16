# Fishfeeder

I have built a number of feeders for my koi fishes over the year (first one in 2005 ;-)) but now i i finally found a way that i think is a good balance between price, quality and build effort. You need to have a 3D printer (i guess you can order them from a printshop too). There are PETG material needed for maybe $20-30, a simple DC continously rotating servo for ~$10, a wemos D1 mini for ~$8 some sewer pipe parts (or if you want your own tank) for another ~$15. So in total you should be able to produce it with ~$50. The pellets screw is designed for 3mm pellets (it might work with other sizes but i don't know since i only use 3mm myself).

The water control is built from a water level sensor (2 alternatives below) a few solenoids (one for the pond and pone for the garden plants micro drip system) for adding tap water and also measuring the water amount used. You need to print a few parts with your 3D printer. I'm using PETG with support on most of the printouts so that it can resist the outdoor UV. I.m just doing basic sanding (paper and file) and then put some spray paint on it to look nice.


The feeder next to the reed makes it more fun for the fishes (they are hunting like crazy in the reed) but also good to avoid getting too fast into the skimmer.
![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_fishfeeder_all.PNG?raw=true "The feeder")
![2](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_fishfeeder_result.JPG?raw=true "The feeder2")


Here are the parts for the feeder:
![3](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_fishfeeder_parts.PNG?raw=true "PVC pipes and some 3D prints")


Basic operation is like a pellets screw. The screw has a loose fit in the cylinder to reduce the fricton and there is no problem for the DC servo to rotate, pulling KOI pellets from the PVC tube.
![4](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_fishfeeder_assembly.PNG?raw=true "Basic operation")

The full unit except the PVC tube:
![5](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_fishfeeder_assembly_2.PNG?raw=true "Feeder kit")


I didn't rough the surface enough the first time, but now it's good. Ideally add some PVD primer or other coler with specialized adhesion.
![6](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_fishfeeder_tube.JPG?raw=true "PVC pipes and some 3D prints")


See stl_step folder models (download and edit STEP or just print STL):
![7](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_model_fishfeeder.PNG?raw=true "Fishfeeder model")


![8](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_fishfeeder_closeup.PNG?raw=true "Closeup")

Here is a YouTube video explaining the 3D models and code for the fishfeeder: https://youtu.be/Hfo5ArDc8C0
Here is the full Poseidon playlist: https://www.youtube.com/watch?v=6Q65v6jY_po&list=PL4uTTzIGocpXYQ4JUedOa0raxNH4VkoVa

The firmware that you need to upload to the Wemos D1 mini can be done from the same RPI using a micro USB cable (unless you want to install platformio gui or cli on your laptop/desktop). The source code is quite simple and mainly default sketch for connecting to your Wifi, connect to a MQTT server and then to just listen on incoming feeding requests where the DC servo is stepped accordingly. It also reports back when ready or listen to ack requests. In order to get a specific amount in milliliter or such then just tune that with your specific fishfood (as that might differ with different pellets) to see how many steps that match. With a fishfeeder you have the luxury of feeding less but more frequently :-). 9 steps on the DC servo is the equivalent of one tablespoon i.e. 15ml so rougly 1.5ml per step. The default value in the web_app when feeding interactively is only 3 (change if you want to (in the web_app/app.py)) so one teaspoon 5ml.
* Change the WIFI settings for your router in Credentials.h or you will get a compilation error (just remove the #error when done)
* Update the mqtt_server ip address in main.cpp or you will get a compilation error (just remove the #error when done)

You don't need any board and very little soldering. Basically the 3 wires on the wemos, see schematic. The rest is mounted on the printed mounting and a 2 pol terminal connector
![9](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_fishfeeder_schematic.JPG?raw=true "Schematic")
![10](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_fishfeeder_motor.JPG?raw=true "Mounting")

Poseidon - Pond Control 2.0 by Anders Johansson is marked with CC0 1.0. To view a copy of this license, visit http://creativecommons.org/publicdomain/zero/1.0