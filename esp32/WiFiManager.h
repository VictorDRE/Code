#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

// WiFiManager class to handle WiFi connections
class WiFiManager {
public:
  void connectToWiFi();
  void checkWiFiConnection(); // Method to check WiFi connection and attempt non-blocking reconnect

private:
  const char* ssid = "Ekip"; // WiFi SSID
  const char* password = "oooooooo"; // WiFi password
  unsigned long lastReconnectAttempt = 0; // To manage reconnect attempts
};

#endif // WIFI_MANAGER_H
