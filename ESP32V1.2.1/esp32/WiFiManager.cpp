#include "WiFiManager.h"

// Connect to WiFi
void WiFiManager::connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
}

// Check and attempt to reconnect to WiFi if disconnected
void WiFiManager::checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    unsigned long currentMillis = millis();
    // Attempt to reconnect every 10 seconds
    if (currentMillis - lastReconnectAttempt >= 10000) {
      lastReconnectAttempt = currentMillis;
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.begin(ssid, password);
    }
  }
}
