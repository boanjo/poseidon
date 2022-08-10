# Water Quality

The water quality is monitored with a set of sensors or probes from Atlas Scientific: Temperature, PH, ORP, DO and EC probes are connected to the carrier boards mounted on a Tentacle T1 MKII. The firmware is running on an Arduino UNO 

![0](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_atlas_sandwich.PNG?raw=true "Atlas sensors with mounting in the Sieve filter")


On the RPI there is a serial2mqtt.py application running and as the name indicates it bridges between the serial commands and the mqtt GW.

See stl_step folder models (download and edit STEP or just print STL):
![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_model_tentacle.PNG?raw=true "Tentacle T1 MKII box model")
![2](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_model_atlas_sensors.PNG?raw=true "Atlas sensors mounting model")



Here is a YouTube video from some of the building step and any programming needed.
