#include <WiFi.h>
#include "wifi_utils.h"

const char* ssid = "Ekip";
const char* password = "oooooooo";

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}
