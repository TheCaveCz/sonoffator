Task ledstateTask(1, TASK_FOREVER, &ledstateCb, &scheduler, true);

void ledstateCb() {
  if (outputButton.isLongPressed()) {
    ledSetState(LED_ON);
  } else if (WiFi.isConnected()) {
    ledSetState(LED_OFF);
  } else if (millis() < 15000) {
    ledSetState(LED_FAST_BLINK); // indicate WiFi connection attempt after startup (first 15 seconds)
  } else {
    ledSetState(LED_TWO_BLINKS); // indicate no WiFi available
  }
}
