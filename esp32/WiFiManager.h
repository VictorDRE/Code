#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

// WiFiManager class to handle WiFi connections
class WiFiManager {
public:
  void connectToWiFi();
  void checkWiFiConnection(); // Method to check WiFi connection and attempt non-blocking reconnect

private:
  const char* ssid = "SII-BOURGES-SECOURS"; // WiFi SSID
  const char* password = "B0uRG35S11!!2019"; // WiFi password
  unsigned long startAttemptTime = 0; // To manage reconnect attempts
};

#endif // WIFI_MANAGER_H
