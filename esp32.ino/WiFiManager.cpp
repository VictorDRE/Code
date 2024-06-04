#include "WiFiManager.h" // Include the header file for the WiFiManager class

// Method to connect to WiFi
void WiFiManager::connectToWiFi() {
  Serial.println("Connecting to WiFi..."); // Print a message indicating the start of the connection process
  
  WiFi.begin(ssid, password); // Start the connection process using the SSID and password

  // Wait until the device is connected to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Wait for 1 second
    Serial.println("Connecting..."); // Print a message indicating that the connection is still in progress
  }

  Serial.println("Connected to WiFi"); // Print a message indicating that the device is successfully connected to WiFi
}
