#include "DataManager.h"

DataManager::DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, NotificationManager* notificationManager) 
  : thingSpeakManager(thingSpeakManager), sensorManager(sensorManager), notificationManager(notificationManager) {}

void DataManager::handleData() {
  unsigned long currentTime = millis();

  // Send data to ThingSpeak every 20 seconds
  if (currentTime - lastSendTime >= updateInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reading sensors and sending data...");
      sensorManager->readSensors();
      sensorManager->printSensorData();

      try {
        thingSpeakManager->sendData(*sensorManager);
      } catch (const std::exception& e) {
        Serial.println(e.what());
      }

      lastSendTime = currentTime;

      // Send WhatsApp notification if needed
      try {
        // Temperature-based notifications
        if (sensorManager->temperature < 10) {
          Serial.println("Temperature is too low, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Il fait très froid !");
        } else if (sensorManager->temperature > 30) {
          Serial.println("Temperature is too high, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Température élevée !");
        }

        // Moisture-based notifications
        if (sensorManager->moisture < 10) {
          Serial.println("Moisture level is very low, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Besoin d'un arrosage automatique !");
        } else if (sensorManager->moisture > 70) {
          if (currentTime - lastMoistureMessageTime >= moistureMessageInterval) {
            Serial.println("Moisture level is very high, sending WhatsApp notification...");
            notificationManager->sendNotification("Arrosage automatique réalisé pour votre plante.");
            lastMoistureMessageTime = currentTime;
          }
        }

        // Light-based notifications
        if (sensorManager->light < 50) {
          Serial.println("Light level is too low, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Placez-la au soleil !");
        } else if (sensorManager->light > 15000) {
          Serial.println("Light level is too high, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Placez-la un peu plus à l'ombre !");
        }

        // Check soil moisture and update the counter
        if (sensorManager->moisture < 30) {
          lowMoistureCount++;
        } else {
          lowMoistureCount = 0;
        }

        // Send notification if soil moisture is low for too long
        if (lowMoistureCount >= lowMoistureLimit) {
          Serial.println("Soil moisture is low for too long, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Humidité du sol insuffisante pendant une période prolongée !");
          lowMoistureCount = 0;
        }
      } catch (const std::exception& e) {
        Serial.println(e.what());
      }
    } else {
      Serial.println("WiFi not connected, skipping data send.");
    }
  }

  // Send summary data every 15 minutes
  if (currentTime - lastSummarySendTime >= summaryUpdateInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      try {
        thingSpeakManager->sendSummaryData(*sensorManager);
      } catch (const std::exception& e) {
        Serial.println(e.what());
      }
      lastSummarySendTime = currentTime;
    } else {
      Serial.println("WiFi not connected, skipping summary data send.");
    }
  }
}
