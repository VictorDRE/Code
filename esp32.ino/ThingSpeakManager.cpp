#include "ThingSpeakManager.h"

void ThingSpeakManager::setup() {
  ThingSpeak.begin(client);
  Serial.println("ThingSpeak setup completed");
}

// Method to send individual field to ThingSpeak
void ThingSpeakManager::sendField(int fieldNumber, float value) {
  Serial.println("Sending field " + String(fieldNumber) + " with value: " + String(value));
  ThingSpeak.setField(fieldNumber, value);
  int responseCode = ThingSpeak.writeFields(channelNumber, apiKey);
  if (responseCode == 200) {
    Serial.println("Field " + String(fieldNumber) + " sent successfully");
  } else {
    Serial.println("Error sending field " + String(fieldNumber) + ": " + String(responseCode));
    Serial.println("Response: " + ThingSpeak.getLastReadStatus());
  }
}

void ThingSpeakManager::sendData(SensorManager& sensorManager) {
  Serial.println("Sending sensor data to ThingSpeak...");
  sendField(1, sensorManager.temperature);
  delay(20000); // Augmentez le délai pour éviter les restrictions de taux
  sendField(2, sensorManager.humidity);
  delay(20000);
  sendField(3, sensorManager.light);
  delay(20000);
  sendField(4, sensorManager.moisture);
  Serial.println("Sensor data sent");
}

void ThingSpeakManager::sendSummaryData(SensorManager& sensorManager) {
  Serial.println("Sending summary data to ThingSpeak...");
  sendField(5, sensorManager.temperature);
  delay(20000);
  sendField(6, sensorManager.humidity);
  delay(20000);
  sendField(7, sensorManager.light);
  delay(20000);
  sendField(8, sensorManager.moisture);
  Serial.println("Summary data sent");
}
