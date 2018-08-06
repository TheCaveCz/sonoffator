
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

#ifdef NOTIFICATION_URL
HTTPClient notifyHttp;

void outputHandleNotify() {
  if (!outputNotify) return;

  if (WiFi.isConnected()) {
    outputNotify = false;
    notifyHttp.begin(NOTIFICATION_URL);
    notifyHttp.POST(outputState ? "{\"characteristic\":\"On\",\"value\":true}" : "{\"characteristic\":\"On\",\"value\":false}");
    notifyHttp.end();
  }
}
#else
void outputHandleNotify() {
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

