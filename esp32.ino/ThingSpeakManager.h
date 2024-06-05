#ifndef THINGSPEAK_MANAGER_H
#define THINGSPEAK_MANAGER_H

#include <WiFi.h>
#include <ThingSpeak.h>
#include "SensorManager.h"

class ThingSpeakManager {
public:
  void setup();
  bool sendField(int fieldNumber, float value);
  void sendData(SensorManager& sensorManager);
  void sendSummaryData(SensorManager& sensorManager);

private:
  WiFiClient client;
  const char* apiKey = "C6JT20CYOZIYQJZQ";
  const unsigned long channelNumber = 2565602;
  bool retrySendField(int fieldNumber, float value, int maxRetries);
};

#endif // THINGSPEAK_MANAGER_H
