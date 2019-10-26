/*   
 * RelitHA Alarm for Home Assistant and NodeMCU.
 * a Home-Automation-friendly ESP8266-based MQTT Alarm reed switches Controller
 * Licensed under the MIT License, Copyright (c) 2018
 * Based on GarHAge
 * http://relit.ca/home-assistant-alarm
*/

// Wifi Parameters

#define WIFI_SSID "wifi"
#define WIFI_PASSWORD "123456"

// Static IP Parameters

#define STATIC_IP false
#define IP 192,168,1,100
#define GATEWAY 192,168,1,1
#define SUBNET 255,255,255,0

// MQTT Parameters

#define MQTT_BROKER "192.168.1.2"
//note that port is a number and not string. no quotation marks
#define MQTT_PORT 1883
//Change Client ID to a different value if running multiple NodeMCUs
#define MQTT_CLIENTID "HouseAlarm"
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "123456"

// Sensor 1 Parameters
#define SENSOR1_ALIAS "Sensor 1"
//Change 'house' to something else if running multiple NodeMCUs
#define MQTT_SENSOR1_ACTION_TOPIC "alarm/house/1/action"
#define MQTT_SENSOR1_STATUS_TOPIC "alarm/house/1/status"
#define SENSOR1_STATUS_PIN D1

// Sensor 2 Parameters
#define SENSOR2_ALIAS "Sensor 2"
//Change 'house' to something else if running multiple NodeMCUs
#define MQTT_SENSOR2_ACTION_TOPIC "alarm/house/2/action"
#define MQTT_SENSOR2_STATUS_TOPIC "alarm/house/2/status"
#define SENSOR2_STATUS_PIN D2

// Sensor 3 Parameters
#define SENSOR3_ALIAS "Sensor 3"
//Change 'house' to something else if running multiple NodeMCUs
#define MQTT_SENSOR3_ACTION_TOPIC "alarm/house/3/action"
#define MQTT_SENSOR3_STATUS_TOPIC "alarm/house/3/status"
#define SENSOR3_STATUS_PIN D5

// Sensor 4 Parameters
#define SENSOR4_ALIAS "Sensor 4"
//Change 'house' to something else if running multiple NodeMCUs
#define MQTT_SENSOR4_ACTION_TOPIC "alarm/house/4/action"
#define MQTT_SENSOR4_STATUS_TOPIC "alarm/house/4/status"
#define SENSOR4_STATUS_PIN D6

// Sensor 5 Parameters
#define SENSOR5_ALIAS "Sensor 5"
//Change 'house' to something else if running multiple NodeMCUs
#define MQTT_SENSOR5_ACTION_TOPIC "alarm/house/5/action"
#define MQTT_SENSOR5_STATUS_TOPIC "alarm/house/5/status"
#define SENSOR5_STATUS_PIN D7
