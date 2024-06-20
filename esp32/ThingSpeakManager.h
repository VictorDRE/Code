#ifndef THINGSPEAK_MANAGER_H
#define THINGSPEAK_MANAGER_H

#include <WiFi.h>
#include <ThingSpeak.h>
#include "SensorManager.h"

// ThingSpeakManager class to handle data sending to ThingSpeak
class ThingSpeakManager {
public:
  void setup(); // Initialize ThingSpeak communication
  bool sendField(int fieldNumber, float value); // Send data to a specific ThingSpeak field
  bool sendData(SensorManager& sensorManager); // Send sensor data to ThingSpeak
  bool sendSummaryData(SensorManager& sensorManager); // Send summary data to ThingSpeak

private:
  WiFiClient client; // WiFi client for ThingSpeak
  const char* apiKey = "C6JT20CYOZIYQJZQ"; // API key for ThingSpeak
  const unsigned long channelNumber = 2565602; // Channel number for ThingSpeak
  bool retrySendField(int fieldNumber, float value, int maxRetries); // Retry sending data to a ThingSpeak field until success or max retries
};

#endif // THINGSPEAK_MANAGER_H
