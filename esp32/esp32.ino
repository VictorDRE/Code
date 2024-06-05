#include "WiFiManager.h"
#include "ThingSpeakManager.h"
#include "SensorManager.h"
#include "WhatsAppManager.h"
#include "NotificationManager.h"
#include "DataManager.h"
#include "LEDManager.h"

// Define LED pins
const int greenLEDPin = 13;
const int redLEDPin = 12;

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
  Serial.println("Setup started");
  ledManager.setup();
  try {
    wifiManager.connectToWiFi();
    thingSpeakManager.setup();
    sensorManager.setup();
  } catch (const std::exception& e) {
    Serial.println(e.what());
  }
  Serial.println("Setup completed");
  ledManager.setNormalOperation(); // Indicate normal operation after setup
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    ledManager.setNoInternet(); // Indicate no internet connection
  } else {
    ledManager.setNormalOperation(); // Indicate normal operation
  }

  dataManager.handleData();

  // Check plant conditions and set error state if needed
  bool plantConditionError = false; // Replace with actual condition check
  if (plantConditionError) {
    ledManager.setErrorState(); // Indicate plant condition error
  }

  ledManager.update();
}
