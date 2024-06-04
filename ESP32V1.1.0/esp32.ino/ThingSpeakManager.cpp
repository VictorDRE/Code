#include "ThingSpeakManager.h"

void ThingSpeakManager::setup() {
  ThingSpeak.begin(client);
}

void ThingSpeakManager::sendData(SensorManager& sensorManager) {
  Serial.println("Sending data to ThingSpeak...");
  ThingSpeak.setField(1, sensorManager.temperature);
  ThingSpeak.setField(2, sensorManager.humidity);
  ThingSpeak.setField(3, sensorManager.light);
  ThingSpeak.setField(4, sensorManager.moisture);
  int x = ThingSpeak.writeFields(channelNumber, apiKey);
  if (x == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Error sending data to ThingSpeak");
  }
}

void ThingSpeakManager::sendSummaryData(SensorManager& sensorManager) {
  Serial.println("Sending summary data to ThingSpeak...");
  ThingSpeak.setField(5, sensorManager.temperature);
  ThingSpeak.setField(6, sensorManager.humidity);
  ThingSpeak.setField(7, sensorManager.light);
  ThingSpeak.setField(8, sensorManager.moisture);
  int x = ThingSpeak.writeFields(channelNumber, apiKey);
  if (x == 200) {
    Serial.println("Summary data sent to ThingSpeak");
  } else {
    Serial.println("Error sending summary data to ThingSpeak");
  }
}
