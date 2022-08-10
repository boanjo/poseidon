# Water Quality

The water quality is monitored with a set of sensors or probes from Atlas Scientific: Temperature, PH, ORP, DO and EC probes are connected to the carrier boards mounted on a Tentacle T1 MKII. The firmware is running on an Arduino UNO 

![0](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_atlas_sandwich.PNG?raw=true "Atlas sensors with mounting in the Sieve filter")

If you don't want to go all in (these sensors + carrier boards cost in the range of $150 to much more) i think the PH and ORP are the most valuable measurments to have. I rarely look at the EC or DO values. Bu that is also because i live in a place in Sweden where the tap water quality is very good. So hard to make any recommendations there but you know my thinking at least. The temperature sensor seems good even if you can make a cheaper solution yourself.

On the RPI there is a serial2mqtt.py application running and as the name indicates it bridges between the serial commands and the mqtt GW.

The Arduino UNO firmware continously collects all the sensor values every 3 seconds and the data is continously pushed to the DB via the MQTT GW, see dashed route red:
![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_water_quality_route.png?raw=true "Sensor route")

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

Here is a YouTube video from some of the building step and any programming needed.