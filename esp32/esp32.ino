#include <WiFi.h>
#include "WiFiManager.h"
#include "ThingSpeakManager.h"
#include "SensorManager.h"
#include "WhatsAppManager.h"
#include "NotificationManager.h"
#include "DataManager.h"
#include "LEDManager.h"

// Define LED pins
const int greenLEDPin = 12;
const int redLEDPin = 13;

// Instantiate managers
WiFiManager wifiManager;
ThingSpeakManager thingSpeakManager;
SensorManager sensorManager;
WhatsAppManager whatsAppManager;
NotificationManager notificationManager(&whatsAppManager);
DataManager dataManager(&thingSpeakManager, &sensorManager, &notificationManager);
LEDManager ledManager(greenLEDPin, redLEDPin);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for Serial to be ready
  }

  Serial.println("Setup started");

  ledManager.setup();
  wifiManager.connectToWiFi();
  thingSpeakManager.setup();
  sensorManager.setup();

  Serial.println("Setup completed");
  ledManager.setNormalOperation(); // Indicate normal operation after setup
}

void loop() {
  wifiManager.checkWiFiConnection();

  if (WiFi.status() != WL_CONNECTED) {
    ledManager.setNoInternet(); // Indicate no internet connection
    Serial.println("No Internet connection");
  } else {
    ledManager.setNormalOperation(); // Indicate normal operation
  }

  try {
    dataManager.handleData();
  } catch (const std::exception& e) {
    Serial.println(e.what());
  }

  // Removing sleep mode

  // Check plant conditions and set error state if needed
  bool plantConditionError = false; // Replace with actual condition check
  if (plantConditionError) {
    ledManager.setErrorState(); // Indicate plant condition error
    Serial.println("Plant condition error detected");
  }

  ledManager.update();
}
