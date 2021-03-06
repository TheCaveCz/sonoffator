
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

void onConnectCb() {
  mqttSubscribe(MQTT_TOPIC_SET);
#if TIMER_ENABLED
  mqttSubscribe(MQTT_TOPIC_TIMER);
#endif
  outputSet(outputState, true); // send current switch state
#if TEMP_ENABLED
  tempSendValues();
#endif
#if ADC_ENABLED
  adcSendValues();
#endif
}

void onMessageCb(const String& topic, const String& message) {
  if (topic.endsWith(MQTT_TOPIC_SET)) {
    if (message == "0") {
      outputSet(false);
    } else if (message == "1") {
      outputSet(true);
    }
  }
#if TIMER_ENABLED
  else if (topic.endsWith(MQTT_TOPIC_TIMER)) {
     outputSetTimer(message.toInt());
  }
#endif
}

void setup() {
  Serial.begin(115200);
  logInfo("\n\nStarting Sonoffator");
  logValue("chipId: ", chipId);

  ledSetup();
  outputSetup();
  outputButton.attachLongPressStop(wifiReset);

#if TEMP_ENABLED
  tempSetup();
#endif
#if ADC_ENABLED
  adcSetup();
#endif

  otaSetup(); // so we can OTA when config portal is up
  wifiSetup(); // will block on config portal if not configured
  mqttSetup();
  mqttOnConnect = onConnectCb;
  mqttOnMessage = onMessageCb;

  logInfo("Setup complete");
}

void loop() {
  outputButton.tick();
  scheduler.execute();
  ArduinoOTA.handle();
}
