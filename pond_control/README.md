# Pond Control

This is a small piece of python software that continously keep track of timeouts (alarms) to feed or turn on water etc. It also interacts with the influxdb and stores subscribed mqtt topics (all pc/# topic) into the database. This is something that you normally can make with the telegraph service but the configuration for it got too complicated so it was easier to implement in a few lines of python. So even if it is small it's the most central piece of the SW stack i.e. the controller. 


![1](https://github.com/boanjo/boanjo.github.io/blob/master/poseidon_overview.JPG?raw=true "Overview")
