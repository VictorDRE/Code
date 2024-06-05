#include "WiFiManager.h"
#include "ThingSpeakManager.h"
#include "SensorManager.h"
#include "WhatsAppManager.h"
#include "NotificationManager.h"
#include "DataManager.h"

// Instantiate managers
WiFiManager wifiManager;
ThingSpeakManager thingSpeakManager;
SensorManager sensorManager;
WhatsAppManager whatsAppManager;
NotificationManager notificationManager(&whatsAppManager);
DataManager dataManager(&thingSpeakManager, &sensorManager, &notificationManager);

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started");
  wifiManager.connectToWiFi();
  thingSpeakManager.setup();
  sensorManager.setup();
  Serial.println("Setup completed");
}

void loop() {
  wifiManager.checkWiFiConnection();
  dataManager.handleData();
}
