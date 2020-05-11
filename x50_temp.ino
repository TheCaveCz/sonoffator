#if TEMP_ENABLED
#include <OneWire.h>

Task tempTask(1, TASK_FOREVER, &tempConvertCb, &scheduler, true);
OneWire tempWire(PIN_TEMP);


struct TempRecord {
  uint8_t addr[8];
  float temp;
};
TempRecord tempRecords[TEMP_MAX_SENSORS];
uint8_t tempCount;

void tempSetup() {
  memset(tempRecords, 0, sizeof(tempRecords));
  tempCount = 0;
}

void tempConvertCb() {
  uint8_t addr[8];

  tempCount = 0;
  tempWire.reset_search();
  while (tempWire.search(addr)) {
    if (addr[7] != OneWire::crc8(addr, 7)) {
      logInfo("CRC mismatch");
      continue;
    }

    memcpy(tempRecords[tempCount].addr, addr, sizeof(addr));
    tempRecords[tempCount].temp = 0;
    tempCount++;
    if (tempCount >= TEMP_MAX_SENSORS) {
      break;
    }
  }

  tempWire.reset();
  tempWire.skip();
  tempWire.write(0x44);

  tempTask.setCallback(&tempReadCb);
  tempTask.setInterval(1000);
}

// reading works only when sensor is set to 12bit precision
bool tempRead(uint8_t id) {
  if (!tempWire.reset()) {
    logInfo("Unable to reset bus");
    return false;
  }

  tempWire.select(tempRecords[id].addr);
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
  tempRecords[id].temp = (float)raw / 16.0;
  logValue("Got temp: ", tempRecords[id].temp);

  char buffer[64];
  sprintf(buffer, "%s/%02x%02x%02x%02x%02x%02x%02x%02x", MQTT_TOPIC_TEMP, tempRecords[id].addr[0], tempRecords[id].addr[1], tempRecords[id].addr[2], tempRecords[id].addr[3], tempRecords[id].addr[4], tempRecords[id].addr[5], tempRecords[id].addr[6], tempRecords[id].addr[7]);
  mqttPublish(buffer, String(tempRecords[id].temp));
  return true;
}

void tempReadCb() {
  bool success = true;
  for (uint8_t i = 0; i < tempCount; i++) {
    if (!tempRead(i)) {
      success = false;
    }
  }

  tempTask.setInterval(success ? (TEMP_INTERVAL - 1000UL) : 1000UL);
  tempTask.setCallback(&tempConvertCb);
}

#endif
