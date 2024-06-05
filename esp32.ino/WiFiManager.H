#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
  void connectToWiFi();

private:
  const char* ssid = "Ekip"; // Nouveau SSID
  const char* password = "oooooooo"; // Nouveau mot de passe
};

#endif // WIFI_MANAGER_H
