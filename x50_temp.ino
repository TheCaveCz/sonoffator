#if TEMP_ENABLED

OneWire tempWire(PIN_TEMP);
float tempCurrent;
uint32_t tempTime;
uint8_t tempState;

void tempSetup() {
  tempCurrent = 0;
  tempTime = 0;
  tempState = 0;
}

void tempConvert() {
  tempWire.reset();
  tempWire.skip();
  tempWire.write(0x44);
}

// reading works only when sensor is set to 12bit precision
void tempRead() {
  if (!tempWire.reset()) {
    logInfo("Unable to reset bus");
    return;
  }

  tempWire.skip();
  tempWire.write(0xBE);

  uint8_t data[9];
  for (uint8_t i = 0; i < 9; i++) {
    data[i] = tempWire.read();
  }

  if (data[8] != OneWire::crc8(data, 8)) {
    logInfo("CRC mismatch");
    return;
  }

  int16_t raw = (data[1] << 8) | data[0];
  tempCurrent = (float)raw / 16.0;
  logValue("Got temp: ", tempCurrent);

  tempTime += 29000ul; // delay the measure if successful
}

void tempTick() {
  if (millis() < tempTime) return;

  tempTime = millis() + 1000ul;
  if (tempState == 0) {
    tempConvert();
    tempState = 1;
  } else {
    tempRead();
    tempState = 0;
  }
}

#endif
