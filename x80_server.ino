
ESP8266WebServer server(80);

#define RESPONSE_START() String __response = ""
#define RESPONSE_ADD(_key,_value) __response += _key "="; __response += _value; __response += "\n"
#define RESPONSE_END(_code,_content) server.send(_code, _content, __response)

void serverHandleRoot() {
  RESPONSE_START();
  RESPONSE_ADD("id", chipId);
  RESPONSE_ADD("mac", WiFi.macAddress());
  RESPONSE_ADD("uptime", millis() / 1000);
  RESPONSE_ADD("output", outputState ? '1' : '0');
  RESPONSE_END(200, "text/plain");
}

void serverGetOutputJson() {
  server.send(200, "application/json", outputState ? "{\"output\":true}\n" : "{\"output\":false}\n");
}

void serverGetOutputRaw() {
  server.send(200, "text/plain", outputState ? "1" : "0");
}

void serverToggleOutput() {
  if (server.arg("n").toInt() == 1) outputNotify = true;
  outputSet(!outputState);
  serverGetOutputRaw();
}

void serverSetOutput() {
  if (!server.hasArg("o")) {
    server.send(400, "text/plain", "Missing o param.\n");
    return;
  }

  if (server.arg("n").toInt() == 1) outputNotify = true;
  outputSet(server.arg("o").toInt() == 1);
  serverGetOutputRaw();
}

void serverSetup() {
  server.on("/", serverHandleRoot);
  server.on("/output", serverGetOutputJson);
  server.on("/outputRaw", serverGetOutputRaw);
  server.on("/setOutput", serverSetOutput);
  server.on("/toggle", serverToggleOutput);
  server.begin();
}

