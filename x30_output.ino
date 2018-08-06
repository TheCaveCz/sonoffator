
bool outputState;

OneButton outputButton(PIN_BTN, true);

void outputSetup() {
  pinMode(PIN_OUTPUT, OUTPUT);

  outputButton.setDebounceTicks(30);
  outputButton.setPressTicks(5000);
  outputButton.attachClick(outputToggle);

  outputSet(false);
}

void outputToggle() {
  outputSet(!outputState);
}

void outputSet(bool out) {
  logValue("Output value: ", out);
  outputState = out;
  digitalWrite(PIN_OUTPUT, out);
}

