#include "WiFiManager.h"
#include "LEDManager.h"

extern LEDManager ledManager;

void WiFiManager::checkWiFiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost. Attempting to reconnect...");
        ledManager.setWifiSearching(); // Indicate WiFi searching

        int retries = 0;
        WiFi.disconnect();
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED && retries < 15) {
            WiFi.disconnect();
            WiFi.begin(ssid, password);
            delay(500);
            Serial.print(".");
            retries++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Reconnected to WiFi");
            ledManager.setNormalOperation();
        } else {
            Serial.println("Failed to reconnect to WiFi");
            ledManager.setNoInternet();
        }
    } else {
        ledManager.setNormalOperation();
    }
}

void WiFiManager::connectToWiFi() {
    Serial.println("Connecting to WiFi...");
    ledManager.setWifiSearching(); // Indicate WiFi searching
    WiFi.begin(ssid, password);
    sleep(5);
    checkWiFiConnection();
}
