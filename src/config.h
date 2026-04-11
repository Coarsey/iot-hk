#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""

#define MQTT_SERVER "xxx.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_USER "xxx"
#define MQTT_PASS "xxx"


#define LED_ALERT   4   
#define LED_LIGHT   15  
#define BUZZER_PIN  25  
#define DHT_PIN     18  
#define LDR_PIN     34  
#define OLED_SDA    21  
#define OLED_SCL    22  

#define TEMP_LIMIT 35.0f
#define HUM_LIMIT  75.0f
#define LIGHT_THRESHOLD 3200

#define DHT_INTERVAL_MS 2000
#define BLINK_INTERVAL_MS 300
#define MQTT_INTERVAL_MS 3000

extern float currentTemp;
extern float currentHum;
extern int currentLight;

extern bool led27State; 
extern bool autoMode;
extern bool climateAlert;

extern bool blinkState;
extern unsigned long lastBlinkTime;

extern unsigned long lastMqttTime;

#endif