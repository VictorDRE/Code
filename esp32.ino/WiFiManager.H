#ifndef WIFI_MANAGER_H // Include guard to prevent multiple inclusions
#define WIFI_MANAGER_H

#include <WiFi.h> // Include the WiFi library for network connectivity

// Define the WiFiManager class
class WiFiManager {
public:
  void connectToWiFi(); // Method to connect to WiFi

private:
  const char* ssid = "Ekip"; // SSID (network name) for the WiFi connection
  const char* password = "oooooooo"; // Password for the WiFi connection
};

#endif // WIFI_MANAGER_H
