#include "WiFiManager.h"
#include "LEDManager.h"

extern LEDManager ledManager; // Use extern to reference the LEDManager instance

void WiFiManager::connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  ledManager.setNoInternet(); // Indicate connecting state (using no internet as an equivalent state)

  // Non-blocking attempt to connect for 30 seconds
  startAttemptTime = millis();
}

void WiFiManager::checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - startAttemptTime >= 30000) {
      Serial.println("Failed to connect to WiFi within 30 seconds");
      startAttemptTime = currentMillis; // Retry after 30 seconds
      WiFi.disconnect();
      WiFi.begin(ssid, password);
      ledManager.setNoInternet();
    }
  } else {
    ledManager.setNormalOperation(); // Indicate active state
  }
}
