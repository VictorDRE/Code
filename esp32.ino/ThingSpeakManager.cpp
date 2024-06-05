#include "ThingSpeakManager.h"

void ThingSpeakManager::setup() {
  ThingSpeak.begin(client);
}

// Method to send individual field to ThingSpeak
void ThingSpeakManager::sendField(int fieldNumber, float value) {
  ThingSpeak.setField(fieldNumber, value);
  int responseCode = ThingSpeak.writeFields(channelNumber, apiKey);
  if (responseCode == 200) {
    Serial.println("Field " + String(fieldNumber) + " sent successfully");
  } else {
    Serial.println("Error sending field " + String(fieldNumber) + ": " + String(responseCode));
  }
}

void ThingSpeakManager::sendData(SensorManager& sensorManager) {
  sendField(1, sensorManager.temperature);
  delay(5000); // Delay to avoid packet loss
  sendField(2, sensorManager.humidity);
  delay(5000);
  sendField(3, sensorManager.light);
  delay(5000);
  sendField(4, sensorManager.moisture);
}

void ThingSpeakManager::sendSummaryData(SensorManager& sensorManager) {
  Serial.println("Sending summary data to ThingSpeak...");
  sendField(5, sensorManager.temperature);
  delay(5000);
  sendField(6, sensorManager.humidity);
  delay(5000);
  sendField(7, sensorManager.light);
  delay(5000);
  sendField(8, sensorManager.moisture);
}
