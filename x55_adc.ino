#if ADC_ENABLED

Task adcTask(ADC_INTERVAL, TASK_FOREVER, &adcConvertCb, &scheduler, true);
uint16_t adcCurrent;

void adcSetup() {
  adcCurrent = 0;
}

void adcConvertCb() {
  adcCurrent = analogRead(PIN_ADC);
  logValue("Analog value: ", adcCurrent);
  mqttPublish(MQTT_TOPIC_ADC, String(adcCurrent));
}

#endif
