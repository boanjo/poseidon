# Water Quality

The water quality is monitored with a set of sensors or probes from Atlas Scientific: Temperature, PH, ORP, DO and EC probes are connected to the carrier boards mounted on a Tentacle T1 MKII. The firmware is running on an Arduino UNO 

![0](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_atlas_sandwich.PNG?raw=true "Atlas sensors with mounting in the Sieve filter")

If you don't want to go all in (these sensors + carrier boards cost in the range of $150 to much more) i think the PH and ORP are the most valuable measurments to have. I rarely look at the EC or DO values. Bu that is also because i live in a place in Sweden where the tap water quality is very good. So hard to make any recommendations there but you know my thinking at least. The temperature sensor seems good even if you can make a cheaper solution yourself.

On the RPI there is a serial2mqtt.py application running and as the name indicates it bridges between the serial commands and the mqtt GW.

The Arduino UNO firmware continously collects all the sensor values every 3 seconds and the data is continously pushed to the DB via the MQTT GW, see dashed route red:
![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_water_quality_route.png?raw=true "Sensor route")

For programming of the firmware i've installed the platformio cli version https://docs.platformio.org/en/latest/core/index.html on the raspberry pi. I.e. i'm running an ssh connection with for example putty or mobixterm to the RPI and edit the SW using emacs, git, ino etc directly into the cloned repo. You need to stop the docker service that is communicating with the arduino to release the serial port (e.g. "docker stop poseidon_water_quality_1"), then build and upload (see the CMD file for the sequence i use "source CMD" to execute) and then start the service again ((e.g. "docker start poseidon_water_quality_1").

You can monitor the sensor values eiter in the web_gui which shows the sensor values in real time but also with min and max range on the right side. You can also follow longer trends in either Grafana or Chronograf. This is from my default dashboards you can also import from the poseidon root dir.
![2](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_sensor_details.png?raw=true "Sensor Details")


See stl_step folder models (download and edit STEP or just print STL):

The Tentacle T1 MKII is a great board for the carrier boards and the sensors and i really recommend it. I've made a small box (not water proof) with both a plexiglass front and a solid one.
![3](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_model_tentacle.PNG?raw=true "Tentacle T1 MKII box model")
![4](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_quality_box.PNG?raw=true "Tentacle T1 MKII box")
![5](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_tentacle_t1.PNG?raw=true "Hidden electronics")



The mounting is tailored for my Sieve filter and works really well. Some of the Atlas sensors need to have a water flow to give accurate readings so this is perfect. It's also a pretty good way to hide the cables
![6](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_model_atlas_sensors.PNG?raw=true "Atlas sensors mounting model")
![7](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_quality_mounting_live_2.PNG?raw=true "Sieve filter")

Here is a YouTube video explaining the sensors, 3D models and code for the water quality setup: https://youtu.be/wYSqa4tR1fA 
Here is the full Poseidon playlist: https://www.youtube.com/watch?v=6Q65v6jY_po&list=PL4uTTzIGocpXYQ4JUedOa0raxNH4VkoVa


Poseidon - Pond Control 2.0 by Anders Johansson is marked with CC0 1.0. To view a copy of this license, visit http://creativecommons.org/publicdomain/zero/1.0