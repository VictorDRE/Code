#include "WiFiManager.h"
#include "LEDManager.h"

extern LEDManager ledManager; // Use extern to reference the LEDManager instance

void WiFiManager::connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  ledManager.setNoInternet(); // Indicate connecting state (using no internet as an equivalent state)
}

void WiFiManager::checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastReconnectAttempt >= 10000) {
      lastReconnectAttempt = currentMillis;
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.begin(ssid, password);
      ledManager.setNoInternet(); // Indicate connecting state (using no internet as an equivalent state)
    }
  } else {
    ledManager.setNormalOperation(); // Indicate active state
  }
}
