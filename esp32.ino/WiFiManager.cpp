#include "WiFiManager.h"

// Method to connect to WiFi
void WiFiManager::connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  
  WiFi.begin(ssid, password);

  // Wait until the device is connected to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi, IP address: ");
  Serial.println(WiFi.localIP());
}
