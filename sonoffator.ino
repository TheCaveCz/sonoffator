//
// Sonoffator
// The Cave 2018, https://thecave.cz, https://github.com/thecavecz/
// Licensed under MIT License
//

//
// Dead simple firmware for Sonoff switches - just HTTP server, button handler and OTA. Suitable for HomeBridge integration
//
// Tested with ESP Arduino cores 2.3.0 and 2.4.2
//
// Using WiFiManager and OneButton libraries
//

//
// Flash with
// Generic ESP8266 module, Flash mode DIO, Flash size 1M (no or 64k SPIFFS), 80MHz CPU, 40MHz flash
//

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <OneButton.h>
#include <Ticker.h>
#include <TaskScheduler.h>
#include <MQTT.h>

// MQTT credentials
#define MQTT_HOST "192.168.85.1"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASS ""

// uncomment this if you want to measure temperature with DS18B20
//#define TEMP_ENABLED 1
#define TEMP_INTERVAL 60000UL
#define TEMP_MAX_SENSORS 4

// This pin assignment works for Sonoff Basic and Sonoff S20/S26
#define PIN_LED 13
#define PIN_BTN 0
#define PIN_OUTPUT 12
#define PIN_TEMP 14

// Logging over Serial
#define LOG_ENABLED 1

#define WIFI_AP_NAME "sonoffator-"


#define MQTT_CLIENT_ID "sonoffator-%s"

#define MQTT_RECONNECT_INTERVAL 10000UL

#define MQTT_PREFIX "hb/sonoffator-%s/"
#define MQTT_ONLINE_TOPIC "online"
#define MQTT_IP_TOPIC "ip"
#define MQTT_NAME_TOPIC "name"

#define MQTT_TOPIC_STATE "state"
#define MQTT_TOPIC_SET "set"
#define MQTT_TOPIC_TEMP "temp"

Scheduler scheduler;
String chipId = String(ESP.getChipId(), HEX);

