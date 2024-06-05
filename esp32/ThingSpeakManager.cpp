#include "ThingSpeakManager.h"

void ThingSpeakManager::setup() {
  ThingSpeak.begin(client);
  Serial.println("ThingSpeak setup completed");
}

bool ThingSpeakManager::sendField(int fieldNumber, float value) {
  Serial.println("Sending field " + String(fieldNumber) + " with value: " + String(value));
  ThingSpeak.setField(fieldNumber, value);
  int responseCode = ThingSpeak.writeFields(channelNumber, apiKey);
  if (responseCode == 200) {
    Serial.println("Field " + String(fieldNumber) + " sent successfully");
    return true;
  } else {
    Serial.println("Error sending field " + String(fieldNumber) + ": " + String(responseCode));
    Serial.println("Response: " + ThingSpeak.getLastReadStatus());
    return false;
  }
}

bool ThingSpeakManager::retrySendField(int fieldNumber, float value, int maxRetries) {
  int attempts = 0;
  while (attempts < maxRetries) {
    if (sendField(fieldNumber, value)) {
      return true;
    }
    attempts++;
    int delayTime = 5000 * attempts;
    Serial.println("Retrying in " + String(delayTime / 1000) + " seconds...");
    delay(delayTime);
  }
  return false;
}

void ThingSpeakManager::sendData(SensorManager& sensorManager) {
  Serial.println("Sending sensor data to ThingSpeak...");
  retrySendField(1, sensorManager.temperature, 3);
  retrySendField(2, sensorManager.humidity, 3);
  retrySendField(3, sensorManager.light, 3);
  retrySendField(4, sensorManager.moisture, 3); // Sending percentage
  Serial.println("Sensor data sent");
}

void ThingSpeakManager::sendSummaryData(SensorManager& sensorManager) {
  Serial.println("Sending summary data to ThingSpeak...");
  retrySendField(5, sensorManager.temperature, 3);
  retrySendField(6, sensorManager.humidity, 3);
  retrySendField(7, sensorManager.light, 3);
  retrySendField(8, sensorManager.moisture, 3); // Sending percentage
  Serial.println("Summary data sent");
}