#include "ThingSpeakManager.h"

// Initialize ThingSpeak
void ThingSpeakManager::setup() {
  ThingSpeak.begin(client);
  Serial.println("ThingSpeak setup completed");
}

// Send a field to ThingSpeak
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

// Retry sending a field to ThingSpeak with progressive delays
bool ThingSpeakManager::retrySendField(int fieldNumber, float value, int maxRetries) {
  int attempts = 0;
  while (attempts < maxRetries) {
    if (sendField(fieldNumber, value)) {
      return true;
    }
    attempts++;
    int delayTime = 5000 * attempts; // Delay progressively longer: 5s, 10s, 15s, etc.
    Serial.println("Retrying in " + String(delayTime / 1000) + " seconds...");
    delay(delayTime);
  }
  return false;
}

// Send sensor data to ThingSpeak
void ThingSpeakManager::sendData(SensorManager& sensorManager) {
  Serial.println("Sending sensor data to ThingSpeak...");
  retrySendField(1, sensorManager.temperature, 3);
  retrySendField(2, sensorManager.humidity, 3);
  retrySendField(3, sensorManager.light, 3);
  retrySendField(4, sensorManager.moisture, 3);
  Serial.println("Sensor data sent");
}

// Send summary data to ThingSpeak
void ThingSpeakManager::sendSummaryData(SensorManager& sensorManager) {
  Serial.println("Sending summary data to ThingSpeak...");
  retrySendField(5, sensorManager.temperature, 3);
  retrySendField(6, sensorManager.humidity, 3);
  retrySendField(7, sensorManager.light, 3);
  retrySendField(8, sensorManager.moisture, 3);
  Serial.println("Summary data sent");
}
