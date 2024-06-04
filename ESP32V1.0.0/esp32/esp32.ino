#include <ThingSpeak.h>
#include <WiFi.h>
#include "sensors.h"

const char *ssid = "SII-BOURGES-SECOURS";
const char *password = "B0uRG35S11!!2019";
const char *apiKey = "C6JT20CYOZIYQJZQ";
const unsigned long channelNumber = 2565602;

WiFiClient client;
unsigned long lastUpdateTime = 0;
unsigned long lastSendTime = 0;
const unsigned long updateInterval = 15 * 1000;  // Intervalle de 15 secondes
const unsigned long summaryUpdateInterval = 10 * 60 * 1000; // Intervalle de 10 minutes

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  ThingSpeak.begin(client);
  setupSensors();
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastSendTime >= updateInterval) {
    readSensors();
    sendDataToThingSpeak();
    lastSendTime = currentTime;
  }

  // Si 10 minutes se sont écoulées depuis la dernière mise à jour des champs 5 à 8, envoyer les données
  if (currentTime - lastUpdateTime >= summaryUpdateInterval) {
    sendSummaryDataToThingSpeak();
    lastUpdateTime = currentTime;
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

void sendDataToThingSpeak() {
  Serial.println("Sending data to ThingSpeak...");
  // Envoyer les données des champs 1 à 4 en une seule requête
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, light);
  ThingSpeak.setField(4, moisture);
  int x = ThingSpeak.writeFields(channelNumber, apiKey);
  if (x == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Problem sending data to ThingSpeak. HTTP error code: " + String(x));
  }
}

void sendSummaryDataToThingSpeak() {
  Serial.println("Sending summary data to ThingSpeak...");
  // Envoyer les données des champs 5 à 8 en une seule requête
  ThingSpeak.setField(5, temperature);
  ThingSpeak.setField(6, humidity);
  ThingSpeak.setField(7, light);
  ThingSpeak.setField(8, moisture);
  int x = ThingSpeak.writeFields(channelNumber, apiKey);
  if (x == 200) {
    Serial.println("\n\x1B[32m---------- Summary data sent to ThingSpeak ----------\x1B[0m\n");
  } else {
    Serial.println("Problem sending summary data to ThingSpeak. HTTP error code: " + String(x));
  }
}
