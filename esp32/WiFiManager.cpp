#include "WiFiManager.h"
#include "LEDManager.h"

extern LEDManager ledManager; // Use extern to reference the LEDManager instance

// Connect to WiFi network
void WiFiManager::connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  ledManager.setWifiSearching(); // Indicate WiFi searching

  // Wait WiFi for 30 secs before time out 
  unsigned long startAttemptTime = millis();

  // Try to connect during 30 secs
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
    ledManager.setNormalOperation(); // Indicate normal operation
  } else {
    Serial.println("Failed to connect to WiFi");
    ledManager.setNoInternet(); // Indicate no internet connection
  }
}

// Check WiFi connection and attempt reconnect if disconnected
void WiFiManager::checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastReconnectAttempt >= 10000) {
      lastReconnectAttempt = currentMillis;
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.begin(ssid, password);
      ledManager.setWifiSearching(); // Indicate WiFi searching
    }
  } else {
    ledManager.setNormalOperation(); // Indicate normal operation
  }
}
