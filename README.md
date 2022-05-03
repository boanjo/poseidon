# poseidon
Poseidon - is a modular control setup for your garden pond for sensors and switches. Monitoring is done with the TICK stack (Telegraf, Influxdb, Chronograf and Kapacitor) accompanied by a python Flask webserver for the control and quick overview. All is (in my setup)  running on a raspberry PI 3B. The influxdb creates some CPU spikes but there is plenty of power over to serve the relevant stuff and UI experience is smooth.

The application is built mainly of 5 parts (For a deeper description use the READMEs below):
* One [Fishfeeder](hhttps://github.com/boanjo/poseidon/tree/main/fishfeeder/README.md) 
* One [Water Control ](https://github.com/boanjo/poseidon/tree/main/water_control/README.md) with level and tap (including watering of the garden plants)
* One [Web Server](https://github.com/boanjo/poseidon/tree/main/web_app/README.md) instance
* One [Atlas scientific hydroponics kit](https://github.com/boanjo/poseidon/tree/main/atlas/README.md) customized to work towards local mqtt queue
* One [Pond Control](https://github.com/boanjo/poseidon/tree/main/pond_control/README.md) which is the "controller" in this setup 

![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_garden.JPG?raw=true "Pond")

![2](https://github.com/boanjo/boanjo.github.io/blob/master/pond_1.jpg?raw=true "Not all days are sunny!")