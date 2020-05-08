
WiFiEventHandler wifiIpHandler;

void wifiSetup() {
  if (WiFi.getAutoConnect()) WiFi.setAutoConnect(false);
  if (WiFi.getMode() != WIFI_STA) WiFi.mode(WIFI_STA);

  logInfo("Connecting...");
  ledSetState(LED_FAST_BLINK);

  wifiIpHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP & evt) {
    // this executes when module reconnects and gets IP from DHCP
    // can be called multiple times

    logValue("Got IP: ", evt.ip);
  });

  if (WiFi.SSID() == "") {
    logInfo("No saved credentials");
    ledSetState(LED_SLOW_BLINK);

    WiFiManager wifiManager;

    String ap = WIFI_AP_NAME;
    ap += chipId;
    if (!wifiManager.startConfigPortal(ap.c_str())) {
      logInfo("Failed to connect or setup, rebooting...");
      delay(1000);
      ESP.reset();
      while (1) delay(1);
    }

  } else if (!WiFi.isConnected()) { // calling WiFi.begin when already connected does all kind of weird stuff
    logValue("Stored SSID: ", WiFi.SSID());
    WiFi.begin();
  }
}

void wifiReset() {
  WiFi.disconnect(true);
  delay(1000);
  ESP.reset();
  while (1) delay(1);
}
