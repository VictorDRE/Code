#ifndef THINGSPEAK_MANAGER_H // Include guard to prevent multiple inclusions
#define THINGSPEAK_MANAGER_H

#include <WiFi.h> // Include the WiFi library for network connectivity
#include <ThingSpeak.h> // Include the ThingSpeak library for sending data to ThingSpeak
#include "SensorManager.h" // Include the header file for the SensorManager class

// Define the ThingSpeakManager class
class ThingSpeakManager {
public:
  void setup(); // Method to set up ThingSpeak
  void sendData(SensorManager& sensorManager); // Method to send sensor data to ThingSpeak
  void sendSummaryData(SensorManager& sensorManager); // Method to send summary sensor data to ThingSpeak

private:
  WiFiClient client; // WiFiClient object for network communication
  const char* apiKey = "C6JT20CYOZIYQJZQ"; // API key for ThingSpeak
  const unsigned long channelNumber = 2565602; // Channel number for ThingSpeak
};

#endif // THINGSPEAK_MANAGER_H
