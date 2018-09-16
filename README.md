There are a few versions of the alarm systems for <a href="https://www.home-assistant.io/" target="_blank" rel="noopener">Home Assistant</a>. This is my take on things. I needed something to report if a door or window is open or closed. All other functionality (even in the garage to open or close a garage door is done from Home Assistant using other means)

I basically disconnected the standard north american house alarm and just left the sensors in place. Those sensors are essentially NO (normally open) reed switches in 99% of cases.

<a href="http://relit.ca/wp-content/uploads/2018/09/20180916_120212.jpg"><img class="alignnone size-1200 wp-image-4353" src="http://relit.ca/wp-content/uploads/2018/09/20180916_120212-1200x900.jpg" alt="" width="1200" height="900" /></a>

You will need a NodeMCU for this (I assume that you already have an instance of Home Assistant with MQTT running in your house)

I was looking for a simple solution and all I found was either old (with syntax for old Arduino libraries) or had no simple functionality that I needed. Or did not enough sensors for me to use. So I had to develop my own.

This version is based on <a href="https://github.com/marthoc/GarHAge" target="_blank" rel="noopener">GarHAge</a> project. Please configure Arduino software as instructed there and download the same libraries/boards (pubsubclient is the library we'll use in this project as well)

A few notes:
<ul>
 	<li>I configured only 5 sensors per NodeMCU (each Dx contact is connected to one wire of a sensor and all others connect to GND on NodeMCU)</li>
 	<li>If you need more, you will need one more NodeMCU
<ul>
 	<li>you will need to change the config.h to be unique: instance ID has to be different in every NodeMCU you will configure and MQTT subject will need to be unique as well for every NodeMCU you configure. There are notes in config.h to assist you.</li>
</ul>
</li>
 	<li>Some pins do not work as intended when all sensors are connected. NodeMCU does not boot if you have pins D0, D3 and D4 connected. So I assigned other pins and those work OK.</li>
 	<li>No additional resistors are required as NodeMCU has those built in and in the standard average town house those are sufficient for a direct connection to the pins of NodeMCU</li>
</ul>
C++ is not my strongest language, so if someone would rewrite it to use proper loops - please be my guest

Sample code for configuration.yaml:
````
binary_sensor:
  - platform: mqtt
    state_topic: "alarm/house/1/status"
    availability_topic: "HouseAlarm/availability"
    name: "Front Door"
    qos: 1
    payload_on: "open"
    payload_off: "closed"
    device_class: opening
````
