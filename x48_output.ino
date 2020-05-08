
bool outputState;

OneButton outputButton(PIN_BTN, true);

void outputSetup() {
  pinMode(PIN_OUTPUT, OUTPUT);

  outputButton.setDebounceTicks(30);
  outputButton.setPressTicks(5000);
  outputButton.attachClick(outputHandleButton);

  outputSet(false);
}

void outputHandleButton() {
  outputSet(!outputState);
}

void outputSet(bool out) {
  logValue("Output value: ", out);
  outputState = out;
  digitalWrite(PIN_OUTPUT, out);

  String s = outputState ? "1" : "0";
  mqttPublish(MQTT_TOPIC_STATE, s, true);
}
