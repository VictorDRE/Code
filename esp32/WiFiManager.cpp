#include "WiFiManager.h"
#include "LEDManager.h"

extern LEDManager ledManager; // Use extern to reference the LEDManager instance

void WiFiManager::connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  ledManager.setNoInternet(); // Indicate connecting state (using no internet as an equivalent state)

  // Attendre que le WiFi se connecte avec un timeout de 30 secondes
  unsigned long startAttemptTime = millis();

  // Tentative de connexion pendant 30 secondes
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Failed to connect to WiFi");
  }
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
