#include "ThingSpeakManager.h"
#include "SensorManager.h"

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
        int delayTime = 5000 * attempts; // Delay progressively longer: 5s, 10s, 15s, etc.
        Serial.println("Retrying in " + String(delayTime / 1000) + " seconds...");
        delay(delayTime);
    }
    return false;
}

bool ThingSpeakManager::retrySendFields(int maxRetries) {
    int attempts = 0
    while (attempts < maxRetries) {
        if (ThingSpeak.writeFields(channelNumber, apiKey) == 200) {
            return true;
        }
        attempts++;
        int delayTime = 5000 * attempts; // Delay progressively longer: 5s, 10s, 15s, etc.
        Serial.println("Retrying in " + String(delayTime / 1000) + " seconds...");
        delay(delayTime);
    }
    return false;
}

bool ThingSpeakManager::sendData(SensorManager& sensorManager) {
    Serial.println("Sending sensor data to ThingSpeak...");
    bool success = true;

    ThingSpeak.setField(1, sensorManager.temperature);
    ThingSpeak.setField(2, sensorManager.humidity);
    ThingSpeak.setField(3, sensorManager.light);
    ThingSpeak.setField(4, sensorManager.moisture);
    success &= retrySendFields(3);

    Serial.println("Sensor data sent");
    return success;
}

bool ThingSpeakManager::sendSummaryData(SensorManager& sensorManager) {
    Serial.println("Sending summary data to ThingSpeak...");
    bool success = true;
    success &= retrySendField(5, sensorManager.temperature, 3);
    success &= retrySendField(6, sensorManager.humidity, 3);
    success &= retrySendField(7, sensorManager.light, 3);
    success &= retrySendField(8, sensorManager.moisture, 3);
    Serial.println("Summary data sent");
    return success;
}
