#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "../config.h"
#include "network.h"

WiFiClientSecure mqttClient;
PubSubClient client(mqttClient);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String cmd = "";

  for (unsigned int i = 0; i < length; i++) {
    cmd += (char)payload[i];
  }
  cmd.trim();

  Serial.print("Nhan topic: ");
  Serial.println(topic);
  Serial.print("Lenh: ");
  Serial.println(cmd);

  if (String(topic) == "esp/light") {
    if (cmd == "1" || cmd == "ON" || cmd == "on" || cmd == "true") {
      autoMode = false;
      led27State = true;
    }
    else if (cmd == "0" || cmd == "OFF" || cmd == "off" || cmd == "false") {
      autoMode = false;
      led27State = false;
    }
    else if (cmd == "AUTO" || cmd == "auto") {
      autoMode = true;
    }

    digitalWrite(LED_LIGHT, led27State ? HIGH : LOW);
  }
}

void connectWifi() {
  Serial.print("Dang ket noi WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nDa ket noi WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void connectMqtt() {
  while (!client.connected()) {
    Serial.print("Dang ket noi MQTT...");

    if (client.connect("ESP32_Client", MQTT_USER, MQTT_PASS)) {
      Serial.println("OK");
      client.subscribe("esp/light"); 
      Serial.println("Sub: esp/light");
    } else {
      Serial.print("Loi MQTT: ");
      Serial.println(client.state());
      delay(3000);
    }
  }
}

void networkInit() {
  connectWifi();

  mqttClient.setInsecure(); 

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqttCallback);
}

void networkUpdate() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  if (!client.connected()) {
    connectMqtt();
  }

  client.loop(); 

  if (millis() - lastMqttTime >= MQTT_INTERVAL_MS) {
    lastMqttTime = millis();

    String envJson = "{\"temp\":" + String(currentTemp, 1) +
                     ",\"hum\":" + String(currentHum, 1) + "}";
    client.publish("esp/environment", envJson.c_str());

    client.publish("esp/lighting", String(currentLight).c_str());

  }
}