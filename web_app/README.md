# Poseidon Web App

The web app is built upon python flask that is running on the RPI4. This is more than enough for a low number of users (i assume you don't expose your pond control for thousands of users ;-). Otherwise flask should be fronted with a proper WSGI webserver like nginx.

The web app uses both the influxdb and mqtt to gather the data presented in the GUI. While active it listens to new sensor values (i.e. real time) but also shows the min and max values today for all sensors. 

If you press the Debug button in the right top corner you will be adding all the raw MQTT messages that are coming on the bus. Press it once more to toggle off or your web gui will be sluggish as it just appends divs to the page. The chronograf dashboards can be reached via the top left button

To open the dashboard just navigate to the ipaddress and the port you choose to expose in the docker-compose.yml (under web_app ports).  i.e. http://localhost:8080
![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_web_app_dashboard.JPG?raw=true "Dashboard")

As the button says "Timers" i guess it's no surprise that here you can modify the timers/alarms for feeding the fishes, starting sprinklers etc.
![2](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_web_app_timers.JPG?raw=true "Timers")

Feel free to update the web app to your liking. I wanted it clean so there are not so much visual bells and whistles. See it as a boilerplate that does the job.

Here is a YouTube video giving an overview of the code for the web_app: https://youtu.be/K_MTeVfbo9E
Here is the full Poseidon playlist: https://www.youtube.com/watch?v=6Q65v6jY_po&list=PL4uTTzIGocpXYQ4JUedOa0raxNH4VkoVa

