# sonoffator

Dead simple firmware for Sonoff switches - just MQTT client, button handler and OTA. Suitable for HomeBridge integration.

## Compiling

You will need following to compile this sketch

* [ESP8266 Arduino Core](https://github.com/esp8266/Arduino) - tested versions 2.3.0 and 2.4.2
* [MQTT library](https://github.com/256dpi/arduino-mqtt)
* [TaskScheduler library](https://github.com/arkhipenko/TaskScheduler)
* [WiFiManager library](https://github.com/tzapu/WiFiManager)
* [OneButton library](https://github.com/mathertel/OneButton)
* [OneWire library](https://github.com/PaulStoffregen/OneWire) - only if you have DS18B20 connected

Use following settings in Arduino IDE

* Board: Generic ESP8266 module
* Flash mode: DIO
* Flash size: 1M (no or 64k SPIFFS)
* CPU Frequency: 80MHz
* Flash Frequency: 40MHz

You need to adjust your MQTT server settings in the code.

## Flashing

**Do not attempt to flash this firmware (or do any crazy stuff) when your Sonoff is connected to mains voltage!**

* Solder wires or pin header to programming connector/pads
* Connect wires to your favorite USB-Serial converter (VCC-3V3, RX-TX, TX-RX, GND-GND)
* Press button on Sonoff
* Connect USB-Serial to your PC (thus enabling ESP flash mode)
* Upload code with Arduino IDE

## Homebridge integration

Use [homebridge-mqttthing](https://github.com/arachnetech/homebridge-mqttthing) plugin.

Sample config for this firmware (replace chipId in topics with your chip ID):

```
"accessories": [
    {
        "accessory": "mqttthing",
        "type": "switch",
        "name": "Sonoffator",
        "integerValue": true,
        "topics": {
            "getOnline": "hb/sonoffator-3b8a49/online",
            "getOn": "hb/sonoffator-3b8a49/state",
            "setOn": "hb/sonoffator-3b8a49/set"
        }
      }
]
```
