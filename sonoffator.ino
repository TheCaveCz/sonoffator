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
#include <ESP8266HTTPClient.h>


// This pin assignment works for Sonoff Basic and Sonoff S20/S26
#define PIN_LED 13
#define PIN_BTN 0
#define PIN_OUTPUT 12

// Logging over Serial
#define LOG_ENABLED 1

#define WIFI_AP_NAME "sonoffator-"

// This URL gets pinged whenever user clicks the button. Useful for https://github.com/Supereg/homebridge-http-notification-server
// Uncomment to enable.
//#define NOTIFICATION_URL "http://homebridge.server:8080/notificationID"

String chipId = String(ESP.getChipId(), HEX);

