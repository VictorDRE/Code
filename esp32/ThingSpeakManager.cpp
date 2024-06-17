#include "ThingSpeakManager.h"
#include "SensorManager.h"

void ThingSpeakManager::setup() {
    ThingSpeak.begin(client);
}

bool ThingSpeakManager::sendField(int fieldNumber, float value) {
    return retrySendField(fieldNumber, value, 3);
}

bool ThingSpeakManager::retrySendField(int fieldNumber, float value, int maxRetries) {
    int statusCode;
    for (int attempts = 0; attempts < maxRetries; attempts++) {
        statusCode = ThingSpeak.writeField(channelNumber, fieldNumber, value, apiKey);
        if (statusCode == 200) {
            return true;
        }
        delay(2000); // Wait 2 seconds before retrying
    }
    return false;
}

bool ThingSpeakManager::sendData(SensorManager& sensorManager) {
    bool success = true;
    success &= sendField(1, sensorManager.getTemperature());
    success &= sendField(2, sensorManager.getMoisture());
    success &= sendField(3, sensorManager.getLight());
    return success;
}

bool ThingSpeakManager::sendSummaryData(SensorManager& sensorManager) {
    bool success = true;
    success &= sendField(5, sensorManager.getTemperature());
    success &= sendField(6, sensorManager.getMoisture());
    success &= sendField(7, sensorManager.getLight());
    success &= sendField(8, sensorManager.getHumidity());
    return success;
}
