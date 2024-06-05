#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
  void connectToWiFi();
  void checkWiFiConnection(); // Méthode pour vérifier la connexion WiFi et tenter une reconnexion non bloquante

private:
  const char* ssid = "Ekip"; // Nouveau SSID
  const char* password = "oooooooo"; // Nouveau mot de passe
  unsigned long lastReconnectAttempt = 0; // Pour gérer les tentatives de reconnexion
};

#endif // WIFI_MANAGER_H
