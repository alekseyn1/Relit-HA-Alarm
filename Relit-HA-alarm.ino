/*   
 * RelitHA Alarm for Home Assistant and NodeMCU.
 * a Home-Automation-friendly ESP8266-based MQTT Alarm reed switches Controller
 * Licensed under the MIT License, Copyright (c) 2018
 * Based on GarHAge
 * http://relit.ca/home-assistant-alarm
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// Mapping NodeMCU Ports to Arduino GPIO Pins
// Allows use of NodeMCU Port nomenclature in config.h
#define D0 16 //this pin does not work properly with NodeMCU. I recommend not to use it.
#define D1 5
#define D2 4
#define D3 0  //works independently, but NodeMCU does not boot when this is HIGH
#define D4 2  //works independently, but NodeMCU does not boot when this is HIGH
#define D5 14
#define D6 12 
#define D7 13
#define D8 15  //not tested this as I do not have more sensors to connect

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const boolean static_ip = STATIC_IP;
IPAddress ip(IP);
IPAddress gateway(GATEWAY);
IPAddress subnet(SUBNET);

const char* mqtt_broker = MQTT_BROKER;
const int mqtt_port = MQTT_PORT;
const char* mqtt_clientId = MQTT_CLIENTID;
const char* mqtt_username = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASSWORD;

const char* sensor1_alias = SENSOR1_ALIAS;
const char* mqtt_sensor1_action_topic = MQTT_SENSOR1_ACTION_TOPIC;
const char* mqtt_sensor1_status_topic = MQTT_SENSOR1_STATUS_TOPIC;
const int sensor1_statusPin = SENSOR1_STATUS_PIN;

const char* sensor2_alias = SENSOR2_ALIAS;
const char* mqtt_sensor2_action_topic = MQTT_SENSOR2_ACTION_TOPIC;
const char* mqtt_sensor2_status_topic = MQTT_SENSOR2_STATUS_TOPIC;
const int sensor2_statusPin = SENSOR2_STATUS_PIN;

const char* sensor3_alias = SENSOR3_ALIAS;
const char* mqtt_sensor3_action_topic = MQTT_SENSOR3_ACTION_TOPIC;
const char* mqtt_sensor3_status_topic = MQTT_SENSOR3_STATUS_TOPIC;
const int sensor3_statusPin = SENSOR3_STATUS_PIN;

const char* sensor4_alias = SENSOR4_ALIAS;
const char* mqtt_sensor4_action_topic = MQTT_SENSOR4_ACTION_TOPIC;
const char* mqtt_sensor4_status_topic = MQTT_SENSOR4_STATUS_TOPIC;
const int sensor4_statusPin = SENSOR4_STATUS_PIN;

const char* sensor5_alias = SENSOR5_ALIAS;
const char* mqtt_sensor5_action_topic = MQTT_SENSOR5_ACTION_TOPIC;
const char* mqtt_sensor5_status_topic = MQTT_SENSOR5_STATUS_TOPIC;
const int sensor5_statusPin = SENSOR5_STATUS_PIN;


const int relayActiveTime = 500;
int sensor1_lastStatusValue = 2;
int sensor2_lastStatusValue = 2;
int sensor3_lastStatusValue = 2;
int sensor4_lastStatusValue = 2;
int sensor5_lastStatusValue = 2;
unsigned long sensor1_lastSwitchTime = 0;
unsigned long sensor2_lastSwitchTime = 0;
unsigned long sensor3_lastSwitchTime = 0;
unsigned long sensor4_lastSwitchTime = 0;
unsigned long sensor5_lastSwitchTime = 0;
int debounceTime = 2000;

String availabilityBase = MQTT_CLIENTID;
String availabilitySuffix = "/availability";
String availabilityTopicStr = availabilityBase + availabilitySuffix;
const char* availabilityTopic = availabilityTopicStr.c_str();
const char* birthMessage = "online";
const char* lwtMessage = "offline";

WiFiClient espClient;
PubSubClient client(espClient);

// Wifi setup function
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (static_ip) {
    WiFi.config(ip, gateway, subnet);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print(" WiFi connected - IP address: ");
  Serial.println(WiFi.localIP());
}

// Callback when MQTT message is received; calls triggerDoorAction(), passing topic and payload as parameters
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  Serial.println();

  String topicToProcess = topic;
  payload[length] = '\0';
  String payloadToProcess = (char*)payload;
  triggerDoorAction(topicToProcess, payloadToProcess);
}

// Functions that check door status and publish an update when called
void publish_sensor1_status() {
  if (digitalRead(sensor1_statusPin) == LOW) {
      Serial.print(sensor1_alias);
      Serial.print(" closed! Publishing to ");
      Serial.print(mqtt_sensor1_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor1_status_topic, "closed", true);
  }
  else {
      Serial.print(sensor1_alias);
      Serial.print(" open! Publishing to ");
      Serial.print(mqtt_sensor1_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor1_status_topic, "open", true);
  }
}

void publish_sensor2_status() {
  if (digitalRead(sensor2_statusPin) == LOW) {
      Serial.print(sensor2_alias);
      Serial.print(" closed! Publishing to ");
      Serial.print(mqtt_sensor2_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor2_status_topic, "closed", true);
  }
  else {
      Serial.print(sensor2_alias);
      Serial.print(" open! Publishing to ");
      Serial.print(mqtt_sensor2_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor2_status_topic, "open", true);
  }
}

void publish_sensor3_status() {
  if (digitalRead(sensor3_statusPin) == LOW) {
      Serial.print(sensor3_alias);
      Serial.print(" closed! Publishing to ");
      Serial.print(mqtt_sensor3_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor3_status_topic, "closed", true);
  }
  else {
      Serial.print(sensor3_alias);
      Serial.print(" open! Publishing to ");
      Serial.print(mqtt_sensor3_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor3_status_topic, "open", true);
  }
}

void publish_sensor4_status() {
  if (digitalRead(sensor4_statusPin) == LOW) {
      Serial.print(sensor4_alias);
      Serial.print(" closed! Publishing to ");
      Serial.print(mqtt_sensor4_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor4_status_topic, "closed", true);
  }
  else {
      Serial.print(sensor4_alias);
      Serial.print(" open! Publishing to ");
      Serial.print(mqtt_sensor4_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor4_status_topic, "open", true);
  }
}

void publish_sensor5_status() {
  if (digitalRead(sensor5_statusPin) == LOW) {
      Serial.print(sensor5_alias);
      Serial.print(" closed! Publishing to ");
      Serial.print(mqtt_sensor5_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor5_status_topic, "closed", true);
  }
  else {
      Serial.print(sensor5_alias);
      Serial.print(" open! Publishing to ");
      Serial.print(mqtt_sensor5_status_topic);
      Serial.println("...");
      client.publish(mqtt_sensor5_status_topic, "open", true);
  }
}

// Functions that run in loop() to check each loop if door status (open/closed) has changed and call publish_doorX_status() to publish any change if so
void check_sensor1_status() {
  int currentStatusValue = digitalRead(sensor1_statusPin);
  if (currentStatusValue != sensor1_lastStatusValue) {
    unsigned int currentTime = millis();
    if (currentTime - sensor1_lastSwitchTime >= debounceTime) {
      publish_sensor1_status();
      sensor1_lastStatusValue = currentStatusValue;
      sensor1_lastSwitchTime = currentTime;
    }
  }
}

void check_sensor2_status() {
  int currentStatusValue = digitalRead(sensor2_statusPin);
  if (currentStatusValue != sensor2_lastStatusValue) {
    unsigned int currentTime = millis();
    if (currentTime - sensor2_lastSwitchTime >= debounceTime) {
      publish_sensor2_status();
      sensor2_lastStatusValue = currentStatusValue;
      sensor2_lastSwitchTime = currentTime;
    }
  }
}

void check_sensor3_status() {
  int currentStatusValue = digitalRead(sensor3_statusPin);
  if (currentStatusValue != sensor3_lastStatusValue) {
    unsigned int currentTime = millis();
    if (currentTime - sensor3_lastSwitchTime >= debounceTime) {
      publish_sensor3_status();
      sensor3_lastStatusValue = currentStatusValue;
      sensor3_lastSwitchTime = currentTime;
    }
  }
}

void check_sensor4_status() {
  int currentStatusValue = digitalRead(sensor4_statusPin);
  if (currentStatusValue != sensor4_lastStatusValue) {
    unsigned int currentTime = millis();
    if (currentTime - sensor4_lastSwitchTime >= debounceTime) {
      publish_sensor4_status();
      sensor4_lastStatusValue = currentStatusValue;
      sensor4_lastSwitchTime = currentTime;
    }
  }
}

void check_sensor5_status() {
  int currentStatusValue = digitalRead(sensor5_statusPin);
  if (currentStatusValue != sensor5_lastStatusValue) {
    unsigned int currentTime = millis();
    if (currentTime - sensor5_lastSwitchTime >= debounceTime) {
      publish_sensor5_status();
      sensor5_lastStatusValue = currentStatusValue;
      sensor5_lastSwitchTime = currentTime;
    }
  }
}


// Function that publishes birthMessage
void publish_birth_message() {
  // Publish the birthMessage
  Serial.print("Publishing birth message \"");
  Serial.print(birthMessage);
  Serial.print("\" to ");
  Serial.print(availabilityTopic);
  Serial.println("...");
  client.publish(availabilityTopic, birthMessage, true);
}

// Function called by callback() when a message is received 
// Passes the message topic as the "requestedDoor" parameter and the message payload as the "requestedAction" parameter

void triggerDoorAction(String requestedDoor, String requestedAction) {
  if (requestedDoor == mqtt_sensor1_action_topic && requestedAction == "STATE") {
    Serial.print("Publishing on-demand status update for ");
    Serial.print(sensor1_alias);
    Serial.println("!");
    publish_birth_message();
    publish_sensor1_status();
  }
  else if (requestedDoor == mqtt_sensor2_action_topic && requestedAction == "STATE") {
    Serial.print("Publishing on-demand status update for ");
    Serial.print(sensor2_alias);
    Serial.println("!");
    publish_birth_message();
    publish_sensor2_status();
  }  
  else if (requestedDoor == mqtt_sensor3_action_topic && requestedAction == "STATE") {
    Serial.print("Publishing on-demand status update for ");
    Serial.print(sensor3_alias);
    Serial.println("!");
    publish_birth_message();
    publish_sensor3_status();
  }
  else if (requestedDoor == mqtt_sensor4_action_topic && requestedAction == "STATE") {
    Serial.print("Publishing on-demand status update for ");
    Serial.print(sensor4_alias);
    Serial.println("!");
    publish_birth_message();
    publish_sensor4_status();
  }  
   else if (requestedDoor == mqtt_sensor5_action_topic && requestedAction == "STATE") {
    Serial.print("Publishing on-demand status update for ");
    Serial.print(sensor5_alias);
    Serial.println("!");
    publish_birth_message();
    publish_sensor5_status();
  }  
  else { Serial.println("Unrecognized action payload... taking no action!");
  }
}

// Function that runs in loop() to connect/reconnect to the MQTT broker, and publish the current door statuses on connect
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_clientId, mqtt_username, mqtt_password, availabilityTopic, 0, true, lwtMessage)) {
      Serial.println("Connected!");

      // Publish the birth message on connect/reconnect
      publish_birth_message();

      // Subscribe to the action topics to listen for action messages
      Serial.print("Subscribing to ");
      Serial.print(mqtt_sensor1_action_topic);
      Serial.println("...");
      client.subscribe(mqtt_sensor1_action_topic);
      
      Serial.print("Subscribing to ");
      Serial.print(mqtt_sensor2_action_topic);
      Serial.println("...");
      client.subscribe(mqtt_sensor2_action_topic);

      Serial.print("Subscribing to ");
      Serial.print(mqtt_sensor3_action_topic);
      Serial.println("...");
      client.subscribe(mqtt_sensor3_action_topic);

      Serial.print("Subscribing to ");
      Serial.print(mqtt_sensor4_action_topic);
      Serial.println("...");
      client.subscribe(mqtt_sensor4_action_topic);

      Serial.print("Subscribing to ");
      Serial.print(mqtt_sensor5_action_topic);
      Serial.println("...");
      client.subscribe(mqtt_sensor5_action_topic);

      //Publish the current door status on connect/reconnect to ensure status is synced with whatever happened while disconnected
      publish_sensor1_status();
      publish_sensor2_status();
      publish_sensor3_status();
      publish_sensor4_status();
      publish_sensor5_status();
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // Setup the output and input pins used in the sketch
  // Set the lastStatusValue variables to the state of the status pins at setup time

  // Set input pin to use internal pullup resistor
  pinMode(sensor1_statusPin, INPUT_PULLUP);
  pinMode(sensor2_statusPin, INPUT_PULLUP);
  pinMode(sensor3_statusPin, INPUT_PULLUP);
  pinMode(sensor4_statusPin, INPUT_PULLUP);
  pinMode(sensor5_statusPin, INPUT_PULLUP);
          
  // Update variable with current door state
  sensor1_lastStatusValue = digitalRead(sensor1_statusPin);
  sensor2_lastStatusValue = digitalRead(sensor2_statusPin);
  sensor3_lastStatusValue = digitalRead(sensor3_statusPin);
  sensor4_lastStatusValue = digitalRead(sensor4_statusPin);
  sensor5_lastStatusValue = digitalRead(sensor5_statusPin);
  
  // Setup serial output, connect to wifi, connect to MQTT broker, set MQTT message callback
  Serial.begin(115200);
  Serial.println("Starting RelitHA...");

  setup_wifi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  // Connect/reconnect to the MQTT broker and listen for messages
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Check door open/closed status each loop and publish changes
  check_sensor1_status();
  check_sensor2_status();
  check_sensor3_status();
  check_sensor4_status();
  check_sensor5_status();
}
