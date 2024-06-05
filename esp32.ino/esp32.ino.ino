#include "WiFiManager.h"
#include "ThingSpeakManager.h"
#include "SensorManager.h"
#include "WhatsAppManager.h"

// Instantiate managers
WiFiManager wifiManager;
ThingSpeakManager thingSpeakManager;
SensorManager sensorManager;
WhatsAppManager whatsAppManager;

// Global variables
unsigned long lastSendTime = 0;
unsigned long updateInterval = 20000; // 20 seconds
float temperatureThreshold = 30.0; // Temperature threshold in degrees Celsius
int lightThreshold = 200; // Light threshold
int moistureThreshold = 300; // Soil moisture threshold
int lowMoistureCount = 0;
int lowMoistureLimit = 5; // Number of low moisture readings before notification
unsigned long lastSummarySendTime = 0;
unsigned long summaryUpdateInterval = 900000; // 15 minutes in milliseconds
unsigned long lastMoistureMessageTime = 0; // Last watering message time
unsigned long moistureMessageInterval = 3600000; // 1 hour in milliseconds

void setup() {
  Serial.begin(115200);
  wifiManager.connectToWiFi();
  thingSpeakManager.setup();
  sensorManager.setup();
}

void loop() {
  unsigned long currentTime = millis();

  // Envoi des données toutes les 20 secondes pour les champs 1-4
  if (currentTime - lastSendTime >= updateInterval) {
    sensorManager.readSensors();
    sensorManager.printSensorData();
    thingSpeakManager.sendData(sensorManager);
    lastSendTime = currentTime;

    // Send WhatsApp notification if needed
    if (sensorManager.temperature < 10) {
      whatsAppManager.sendMessage("Attention à votre plante. Il fait très froid !");
    } else if (sensorManager.temperature > 30) {
      whatsAppManager.sendMessage("Attention à votre plante. Température élevée !");
    }

    if (sensorManager.moisture < 200) {
      whatsAppManager.sendMessage("Attention à votre plante. Besoin d'un arrosage automatique !");
    } else if (sensorManager.moisture > 1000) {
      if (currentTime - lastMoistureMessageTime >= moistureMessageInterval) {
        whatsAppManager.sendMessage("Arrosage automatique réalisé pour votre plante.");
        lastMoistureMessageTime = currentTime;
      }
    }

    if (sensorManager.light < 50) {
      whatsAppManager.sendMessage("Attention à votre plante. Placez-la au soleil !");
    } else if (sensorManager.light > 15000) {
      whatsAppManager.sendMessage("Attention à votre plante. Placez-la un peu plus à l'ombre !");
    }

    // Check soil moisture and update the counter
    if (sensorManager.moisture < moistureThreshold) {
      lowMoistureCount++;
    } else {
      lowMoistureCount = 0;
    }

    // Send notification if soil moisture is low for too long
    if (lowMoistureCount >= lowMoistureLimit) {
      whatsAppManager.sendMessage("Attention à votre plante. Humidité du sol insuffisante pendant une période prolongée !");
      lowMoistureCount = 0;
    }
  }

  // Envoi des données toutes les 15 minutes pour les champs 5-8
  if (currentTime - lastSummarySendTime >= summaryUpdateInterval) {
    thingSpeakManager.sendSummaryData(sensorManager);
    lastSummarySendTime = currentTime;
  }
}
