
void otaSetup() {
  String hn = WIFI_AP_NAME;
  hn += chipId;
  ArduinoOTA.setHostname(hn.c_str());
  ArduinoOTA.onStart([]() {
    outputSet(false);
    ledSetState(LED_ON);
    logInfo("OTA starting");
  });
  ArduinoOTA.onEnd([]() {
    logInfo("OTA finished");
  });
  ArduinoOTA.begin();
}

void setup() {
  Serial.begin(115200);
  logInfo("\n\nStarting Sonoffator");
  logValue("chipId: ", chipId);

  ledSetup();
  outputSetup();
  outputButton.attachLongPressStop(wifiReset);

  configSetup();
  otaSetup(); // so we can OTA when config portal is up
  wifiSetup(); // will block on config portal if not configured
  serverSetup(); // HTTP server has to be setup after wifiSetup (because WiFiManager has its own HTTP server)
  
  logInfo("Setup complete");
}

void loop() {
  outputButton.tick();
  server.handleClient();
  ArduinoOTA.handle();

  if (outputButton.isLongPressed()) {
    ledSetState(LED_ON);
  } else if (WiFi.isConnected()) {
    ledSetState(LED_OFF);
  } else if (millis() < 15000) {
    ledSetState(LED_FAST_BLINK);
  } else {
    ledSetState(LED_TWO_BLINKS);
  }

#if NOTIFICATION_ENABLED
  outputHandleNotify();
#endif
}
