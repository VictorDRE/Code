#include "ThingSpeakManager.h"
#include "SensorManager.h"


void ThingSpeakManager::setup() {
    /*
        Setup function to initialize ThingSpeak library
    */

    ThingSpeak.begin(client);
    Serial.println("ThingSpeak setup completed");
}


bool ThingSpeakManager::sendField(int fieldNumber, float value) {
    /*
        Set and send a single field to ThingSpeak

        int fieldNumber : The number of the field on ThingSpeak you want to send the value on
        float value : The value you want to send to ThingSpeak

        Return true if success
        Return false if not sucess
    */

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
    /*
        Set and send a single field using sendField function and retry with exponential backoff in case of error

        int fieldNumber : The number of the field on ThingSpeak you want to send the value on
        float value : The value you want to send to ThingSpeak
        int maxRetries : The number of max sending retry you want

        Return true if success
        Return false if not sucess
    */

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
    /*
        Send all previously set fields and retry with exponential backoff in case of error

        int maxRetries : The number of max sending retry you want

        Return true if success
        Return false if not sucess
    */

    int attempts = 0;
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
    /*
        Set and sends all four known fields using retrySendFiels function

        SensorManager& sensorManager : The sensor manager class used to read and store sensors values

        Return true if success
        Return false if not sucess
    */

    Serial.println("Sending sensor data to ThingSpeak...");
    bool success = true;

    // Set fields with sensor data
    ThingSpeak.setField(1, sensorManager.temperature);
    ThingSpeak.setField(2, sensorManager.humidity);
    ThingSpeak.setField(3, sensorManager.light);
    ThingSpeak.setField(4, sensorManager.moisture);
    
    success &= retrySendFields(3); // Retry sending data if it fails

    Serial.println("Sensor data sent");
    return success;
}