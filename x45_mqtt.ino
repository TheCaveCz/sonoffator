WiFiClient mqttNetClient;
MQTTClient mqttClient(512);

Task mqttTask(100, TASK_FOREVER, &mqttConnectCb, &scheduler, true);
char mqttTopicPrefix[64];
void (*mqttOnConnect)();
void (*mqttOnMessage)(const String& topic, const String& message);


void mqttSetup() {
  mqttOnConnect = NULL;
  mqttOnMessage = NULL;

  mqttNetClient.setTimeout(20000);
  mqttClient.begin(MQTT_HOST, MQTT_PORT, mqttNetClient);

  snprintf(mqttTopicPrefix, sizeof(mqttTopicPrefix), MQTT_PREFIX, chipId.c_str());
  logValue("MQTT topic prefix: ", mqttTopicPrefix);
}

void mqttMessageCb(String& topic, String& payload) {
  if (mqttOnMessage) mqttOnMessage(topic, payload);
}

void mqttConnect(const long interval) {
  logValue("MQTT reconnect in ", interval);

  mqttTask.setCallback(&mqttConnectCb);
  mqttTask.restartDelayed(interval);
}

void mqttPublish(const char * topicSuffix, const char * data, size_t len, bool retain) {
  char topic[64];
  strcpy(topic, mqttTopicPrefix);
  strcat(topic, topicSuffix);
  mqttClient.publish(topic, data, len, retain, 0);
}

void mqttPublish(const char * topicSuffix, const String& data, bool retain = false) {
  mqttPublish(topicSuffix, data.c_str(), data.length(), retain);
}

void mqttSubscribe(const char * topicSuffix) {
  char topic[64];
  strcpy(topic, mqttTopicPrefix);
  strcat(topic, topicSuffix);
  mqttClient.subscribe(topic);
}

void mqttWill(const char * topicSuffix, const char * data, bool retain = false) {
  char topic[64];
  strcpy(topic, mqttTopicPrefix);
  strcat(topic, topicSuffix);
  mqttClient.setWill(topic, data, retain, 0);
}


void mqttConnectCb() {
  if (!WiFi.isConnected()) {
    return;
  }

  logInfo("Connecting MQTT");
  if (mqttClient.connected()) {
    mqttClient.disconnect();
  }

  mqttWill(MQTT_ONLINE_TOPIC, "0", true);
  
  char clientId[64];
  snprintf(clientId, sizeof(clientId), MQTT_CLIENT_ID, chipId.c_str());
  if (!mqttClient.connect(clientId, MQTT_USER, MQTT_PASS)) {
    logValue("MQTT connect failed with error: ", mqttClient.lastError());
    mqttConnect(MQTT_RECONNECT_INTERVAL);
    return;
  }

  logInfo("MQTT connected");
  mqttTask.setCallback(mqttRunCb);

  mqttClient.onMessage(&mqttMessageCb);
  mqttPublish(MQTT_ONLINE_TOPIC, "1", true);
  
  mqttPublish(MQTT_IP_TOPIC, WiFi.localIP().toString(), true);
  mqttPublish(MQTT_NAME_TOPIC, clientId, strlen(clientId), true);
  
  if (mqttOnConnect) mqttOnConnect();
}

void mqttRunCb() {
  if (!mqttClient.loop()) {
    logValue("MQTT client last error: ", mqttClient.lastError());
    mqttConnect(MQTT_RECONNECT_INTERVAL);
  }
}
