#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
  void connectToWiFi();

private:
  const char* ssid = "Ekip";
  const char* password = "oooooooo";
};

#endif // WIFI_MANAGER_H
