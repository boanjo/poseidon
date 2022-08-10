# poseidon
Poseidon - is a modular control setup for your garden pond for sensors and switches. Monitoring is done with the TICK stack (Telegraf, Influxdb, Chronograf and Kapacitor) accompanied by a python Flask webserver for the control and quick overview. All is (in my setup) running on a raspberry PI 4. The influxdb creates some CPU spikes but there is plenty of power over to serve the relevant stuff and UI experience is smooth. I used to run it on a RPI 3B but that was pushing the limits a bit (both memory and CPU) at least for the influxdb database.

The application is built mainly of 5 parts (For a deeper description use the READMEs below) but each individual service can be used standalone with some tweaking:
* [Fishfeeder](https://github.com/boanjo/poseidon/tree/main/fishfeeder/README.md) 
* [Water Control ](https://github.com/boanjo/poseidon/tree/main/water_control/README.md) with level and tap (including watering of the garden plants)
* [Web Server](https://github.com/boanjo/poseidon/tree/main/web_app/README.md) instance
* [Water Quality - Atlas scientific sensors](https://github.com/boanjo/poseidon/tree/main/water_quality/README.md) in custom casing and mounted in a Sieve filter
* [Pond Control](https://github.com/boanjo/poseidon/tree/main/pond_control/README.md) which is the "controller" in this setup 

The interaction with the pond is mainly done via the web app where you can control timers or interactive feeding, water on/off etc. For more details and long term trends you have that in the Chronograf or Grafana which visualize the DB data. Chronograf has a few nice predefined dashboards both for RPI resource (CPU, mem,...) usage and also the sensor values from the pond. Kapacitor is not really used at all and you can disable it if wanted as the pond_control is doing some of it's work. Telegraph is mainly used to collect the resource usage.

![0](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_overview.JPG?raw=true "Overview!")

All services are communicating through a MQTT (mosquitto) gateway (also running on the RPI). The Fishfeeder firmware is running on a Wemos D1 mini over wifi, the water quality on a Arduino UNO and the water control on an Arduino Nano 

The services are started using docker composer so you can update the docker-compose.yml to suite your needs. 

![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_garden.JPG?raw=true "Not all days are sunny!")

![2](https://github.com/boanjo/boanjo.github.io/blob/master/pond_1.jpg?raw=true "Pond!")

## Howto start
* First you need to decide if you want to have all the parts and sensors. All services should be ok with that no values at all are bing pushed but there might be hardcoded assumptions (like in the web app) that you at some point will get ORP and PH etc.
* If you are building sensors like the water_control or simply the fishfeeder now is the time to do so and to program it's firmware (see each README for details). The fishfeeder hooks up over wifi and the water_quality and water_control with Arduino UNO and Nano. One reason for selecting different Arduinos is to get determensitic names at boot (i.e. so they don't get mixed) due to race conditions. This could have been solved in other ways but it's a good bonus worth knowing about.
* Now you can build and start your services (you need to install docker and docker-compose on the RPI, use something like: https://dev.to/elalemanyo/how-to-install-docker-and-docker-compose-on-raspberry-pi-1mo):

<pre><code>
  docker-compose build   #build all services based on the yml file
  docker-compose -d up   #Here you start all the services in the docker-compose.yml file daemonized so even if you exit your shell they will continue to live and start on boot
  docker ps              #check all the services up and runnig
  docker-compose logs -f #Follow the logging from all the services
</code></pre>

## More details
I have 2 dashboards setup for Chronograf (one detailed/all data for the last 3 days and one downsampled for the last moth). It's quite easy to add your own graphs based on the influxdb data, but if you want to reuse them you can:
* Download the json files from the repo and drag and drop them in Chronograf under "Dashboards->Import Dashboards".
* Headless from your RPI you can install curl and then import one json a t a time with:
  * curl -i -X POST -H "Content-Type: application/json" http://localhost:8888/chronograf/v1/dashboards -d pc_dashboard.json
  * curl -i -X POST -H "Content-Type: application/json" http://localhost:8888/chronograf/v1/dashboards -d pc_dashboard_downsampled.json

Here is what you can get from the detailed dashboard (retention period 3 days):
![3](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_sensor_details.png?raw=true "Dashboard")



Here is what you can get from the resource (Click in Chronograf in the left column "Host List"):
![4](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_host_resources.png?raw=true "Host resource usage")
