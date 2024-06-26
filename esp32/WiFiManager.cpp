#include "WiFiManager.h"
#include "LEDManager.h"

extern LEDManager ledManager;

// Check WiFi connection status and reconnect if necessary
void WiFiManager::checkWiFiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost. Attempting to reconnect...");
        ledManager.setWifiSearching(); // Indicate WiFi searching

        int retries = 0;
        WiFi.disconnect();
        WiFi.begin(ssid, password);

        // Retry connecting to WiFi up to 15 times
        while (WiFi.status() != WL_CONNECTED && retries < 15) {
            delay(500); // Wait before retrying
            Serial.print(".");
            retries++;
        }

        // Check if connected after retries
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nReconnected to WiFi");
            ledManager.setNormalOperation(); // Indicate normal operation
        } else {
            Serial.println("\nFailed to reconnect to WiFi");
            ledManager.setNoInternet(); // Indicate no internet connection
        }
    } else {
        ledManager.setNormalOperation(); // Indicate normal operation if already connected
    }
}

// Connect to WiFi
void WiFiManager::connectToWiFi() {
    Serial.println("Connecting to WiFi...");
    ledManager.setWifiSearching(); // Indicate WiFi searching
    WiFi.begin(ssid, password);
    delay(5000); // Wait for connection attempt
    checkWiFiConnection(); // Check connection status
}
