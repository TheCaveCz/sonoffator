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
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <OneButton.h>
#include <Ticker.h>
#include <EEPROM.h>

// uncomment this if you want to measure temperature with DS18B20
//#define TEMP_ENABLED 1

// uncomment this if you want to send notification to http endpoint when switch state changes
//#define NOTIFICATION_ENABLED 1

// This pin assignment works for Sonoff Basic and Sonoff S20/S26
#define PIN_LED 13
#define PIN_BTN 0
#define PIN_OUTPUT 12

// Logging over Serial
#define LOG_ENABLED 1

#define WIFI_AP_NAME "sonoffator-"

#if TEMP_ENABLED
#include <OneWire.h>
#define PIN_TEMP 14
#endif

#if NOTIFICATION_ENABLED
#include <ESP8266HTTPClient.h>

#define NOTIFICATION_APPEND_STATE_TO_URL 0
#define NOTIFICATION_APPEND_STATE_ON "true"
#define NOTIFICATION_APPEND_STATE_OFF "false"

#define NOTIFICATION_USE_POST 1
#define NOTIFICATION_POST_PAYLOAD_ON "{\"characteristic\":\"On\",\"value\":true}"
#define NOTIFICATION_POST_PAYLOAD_OFF "{\"characteristic\":\"On\",\"value\":false}"

#endif


String chipId = String(ESP.getChipId(), HEX);

