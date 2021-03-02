#if TIMER_ENABLED
Task outputTask(1000, 1, outputCb, &scheduler);
#endif
bool outputState;

OneButton outputButton(PIN_BTN, true);

void outputSetup() {
  pinMode(PIN_OUTPUT, OUTPUT);

  outputButton.setDebounceTicks(30);
  outputButton.setPressTicks(5000);
  outputButton.attachClick(outputHandleButton);

  outputSet(false, true);
}

void outputHandleButton() {
  outputSet(!outputState);
}

void outputSet(bool out, bool ignore) {
  if (ignore == false && out == outputState) return;

  logValue("Output value: ", out);
  outputState = out;
  digitalWrite(PIN_OUTPUT, out);

#if TIMER_ENABLED
  if (out == false) {
    outputTask.disable();
    mqttPublish(MQTT_TOPIC_TIMER_REMAIN, "0");
  }
#endif

  String s = outputState ? "1" : "0";
  mqttPublish(MQTT_TOPIC_STATE, s, true);
}

void outputSet(bool out) {
  outputSet(out, false);
}

#if TIMER_ENABLED
void outputCb() {
  String s(outputTask.getIterations());
  mqttPublish(MQTT_TOPIC_TIMER_REMAIN, s);

  if (outputTask.isLastIteration()) {
    outputSet(false);
  }
}

void outputSetTimer(uint32_t seconds) {
  if (seconds == 0) {
    outputSet(false);
  } else {
    outputTask.setIterations(seconds);
    outputTask.restartDelayed(0);
    logValue("Timer set to: ", seconds);
    outputSet(true);

    String s(seconds);
    mqttPublish(MQTT_TOPIC_TIMER_REMAIN, s);
  }
}
#endif
