
bool outputState;
bool outputNotify;

OneButton outputButton(PIN_BTN, true);

void outputSetup() {
  pinMode(PIN_OUTPUT, OUTPUT);

  outputButton.setDebounceTicks(30);
  outputButton.setPressTicks(5000);
  outputButton.attachClick(outputHandleButton);

  outputNotify = false;
  outputSet(false);
}

#if NOTIFICATION_ENABLED
HTTPClient notifyHttp;

void outputHandleNotify() {
  if (!outputNotify) return;
  if (!WiFi.isConnected()) return;
  if (strlen(config.notifyUrl) == 0) return;

  outputNotify = false;

  String url = String(config.notifyUrl);
#if NOTIFICATION_APPEND_STATE_TO_URL
  url += outputState ? NOTIFICATION_APPEND_STATE_ON : NOTIFICATION_APPEND_STATE_OFF;
#endif

  logValue("Sending notify to ", url);

  notifyHttp.begin(url);
#if NOTIFICATION_USE_POST
  notifyHttp.POST(outputState ? NOTIFICATION_POST_PAYLOAD_ON : NOTIFICATION_POST_PAYLOAD_OFF);
#else
  notifyHttp.GET();
#endif
  notifyHttp.end();
}
#endif

void outputHandleButton() {
  outputSet(!outputState);
  outputNotify = true;
}

void outputSet(bool out) {
  logValue("Output value: ", out);
  outputState = out;
  digitalWrite(PIN_OUTPUT, out);
}

