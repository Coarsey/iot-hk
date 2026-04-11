#include <Arduino.h>
#include <DHT.h>
#include "../config.h"
#include "../communication/network.h"
#include "climate.h"

DHT dht(DHT_PIN, DHT22);
unsigned long lastDhtRead = 0;

bool lastBuzzerState = false; 

void climateInit() {
  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_ALERT, OUTPUT);
}

void climateUpdate() {
  if (millis() - lastDhtRead >= DHT_INTERVAL_MS) {
    lastDhtRead = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t)) currentTemp = t;
    if (!isnan(h)) currentHum = h;

    if (currentTemp > TEMP_LIMIT || currentHum > HUM_LIMIT) {
      climateAlert = true;
    } else {
      climateAlert = false;
    }
  }

  if (climateAlert) {
    

    if (millis() - lastBlinkTime >= BLINK_INTERVAL_MS) {
      lastBlinkTime = millis();
      blinkState = !blinkState;
      digitalWrite(LED_ALERT, blinkState ? HIGH : LOW);
    }

    if (!lastBuzzerState) {
      tone(BUZZER_PIN, 1000); 
      lastBuzzerState = true;
    }

  } else {
    digitalWrite(LED_ALERT, LOW);
    
    if (lastBuzzerState) {
      noTone(BUZZER_PIN); 
      lastBuzzerState = false;
    }
  }
}