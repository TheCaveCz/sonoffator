#if TEMP_ENABLED
#include <OneWire.h>

Task tempTask(1, TASK_FOREVER, &tempConvertCb, &scheduler, true);
OneWire tempWire(PIN_TEMP);
float tempCurrent;

void tempSetup() {
  tempCurrent = 0;
}

void tempConvertCb() {
  tempWire.reset();
  tempWire.skip();
  tempWire.write(0x44);

  tempTask.setCallback(&tempReadCb);
  tempTask.setInterval(1000);
}

// reading works only when sensor is set to 12bit precision
bool tempRead() {
  if (!tempWire.reset()) {
    logInfo("Unable to reset bus");
    return false;
  }

  tempWire.skip();
  tempWire.write(0xBE);

  uint8_t data[9];
  for (uint8_t i = 0; i < 9; i++) {
    data[i] = tempWire.read();
  }

  if (data[8] != OneWire::crc8(data, 8)) {
    logInfo("CRC mismatch");
    return false;
  }

  int16_t raw = (data[1] << 8) | data[0];
  tempCurrent = (float)raw / 16.0;
  logValue("Got temp: ", tempCurrent);

  mqttPublish(MQTT_TOPIC_TEMP, String(tempCurrent));
  return true;
}

void tempReadCb() {
  bool success = tempRead();

  tempTask.setInterval(success ? (60000-1000) : 1000);
  tempTask.setCallback(&tempConvertCb);
}

#endif
