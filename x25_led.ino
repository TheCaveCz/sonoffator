
#define LED_OFF 0
#define LED_FAST_BLINK 1
#define LED_SLOW_BLINK 2
#define LED_TWO_BLINKS 4
#define LED_ON 3

Ticker ledTicker; // using ticker because of blocking operations
uint8_t ledTickerProgress;
uint8_t ledState;

void ledSetup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  ledTickerProgress = 0;
  ledState = LED_OFF;
  ledTicker.attach(0.1, ledTick);
}

void ledSetState(uint8_t l) {
  if (l == ledState) return;

  logValue("Setting LED state: ", l);
  ledState = l;
}

void ledTick() {
  switch (ledState) {
    case LED_ON: digitalWrite(PIN_LED, LOW); break;
    case LED_FAST_BLINK: digitalWrite(PIN_LED, ledTickerProgress % 2); break;
    case LED_SLOW_BLINK: digitalWrite(PIN_LED, (ledTickerProgress % 20) < 10); break;
    case LED_TWO_BLINKS: digitalWrite(PIN_LED, ledTickerProgress < 8 ? (ledTickerProgress % 4) < 2 : HIGH); break;
    default: digitalWrite(PIN_LED, HIGH); break;
  }
  ledTickerProgress++;
  if (ledTickerProgress >= 20) ledTickerProgress = 0;
}

