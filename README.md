# poseidon
Poseidon - is a modular control setup for your garden pond for sensors and switches. Monitoring is done with the TICK stack (Telegraf, Influxdb, Chronograf and Kapacitor) accompanied by a python Flask webserver for the control and quick overview. All is (in my setup) running on a raspberry PI 4. The influxdb creates some CPU spikes but there is plenty of power over to serve the relevant stuff and UI experience is smooth. I used to run it on a RPI 3B but that was pushing the limits a bit (both memory and CPU) at least for the influxdb database.

The application is built mainly of 5 parts (For a deeper description use the READMEs below) but each individual service can be used standalone with some tweaking:
* [Fishfeeder](hhttps://github.com/boanjo/poseidon/tree/main/fishfeeder/README.md) 
* [Water Control ](https://github.com/boanjo/poseidon/tree/main/water_control/README.md) with level and tap (including watering of the garden plants)
* [Web Server](https://github.com/boanjo/poseidon/tree/main/web_app/README.md) instance
* [Water Quality - Atlas scientific sensors](https://github.com/boanjo/poseidon/tree/main/water_quality/README.md) in custom casing and mounted in a Sieve filter
* [Pond Control](https://github.com/boanjo/poseidon/tree/main/pond_control/README.md) which is the "controller" in this setup 

![0](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_overview.JPG?raw=true "Overview!")

All services are communicating through a MQTT (mosquitto) gateway (also running on the RPI). The Fishfeeder firmware is running on a Wemos D1 mini over wifi, the water quality on a Arduino UNO and the water control on an Arduino Nano 

The services are started using docker composer so you can update the docker-compose.yml to suite your needs. 

![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_garden.JPG?raw=true "Not all days are sunny!")

![2](https://github.com/boanjo/boanjo.github.io/blob/master/pond_1.jpg?raw=true "Pond!")