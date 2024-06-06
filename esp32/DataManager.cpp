#include "DataManager.h"
#include "LEDManager.h"

extern LEDManager ledManager; // Use extern to reference the LEDManager instance

DataManager::DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, NotificationManager* notificationManager) 
  : thingSpeakManager(thingSpeakManager), sensorManager(sensorManager), notificationManager(notificationManager), errorOccurred(false), errorStartTime(0) {}

void DataManager::handleData() {
  unsigned long currentTime = millis();

  if (currentTime - lastSendTime >= updateInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reading sensors and sending data...");
      try {
        sensorManager->readSensors();
        sensorManager->printSensorData();
        thingSpeakManager->sendData(*sensorManager);
      } catch (const std::exception& e) {
        Serial.println(e.what());
      }

      lastSendTime = currentTime;

      try {
        bool messageSent = false; // Flag to check if a message has been sent

        if (sensorManager->temperature < 10) {
          Serial.println("Temperature is too low, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Il fait très froid !");
          messageSent = true;
        } else if (sensorManager->temperature > 30) {
          Serial.println("Temperature is too high, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Température élevée !");
          messageSent = true;
        }

        if (sensorManager->moisture < 10) {
          Serial.println("Moisture level is very low, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Besoin d'un arrosage automatique !");
          messageSent = true;
        } else if (sensorManager->moisture > 70) {
          if (currentTime - lastMoistureMessageTime >= moistureMessageInterval) {
            Serial.println("Moisture level is very high, sending WhatsApp notification...");
            notificationManager->sendNotification("Arrosage automatique réalisé pour votre plante.");
            lastMoistureMessageTime = currentTime;
          }
          messageSent = true;
        }

        if (sensorManager->light < 50) {
          Serial.println("Light level is too low, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Placez-la au soleil !");
          messageSent = true;
        } else if (sensorManager->light > 15000) {
          Serial.println("Light level is too high, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Placez-la un peu plus à l'ombre !");
          messageSent = true;
        }

        if (sensorManager->moisture < 30) {
          lowMoistureCount++;
        } else {
          lowMoistureCount = 0;
        }

        if (lowMoistureCount >= lowMoistureLimit) {
          Serial.println("Soil moisture is low for too long, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention à votre plante. Humidité du sol insuffisante pendant une période prolongée !");
          lowMoistureCount = 0;
          messageSent = true;
        }

        if (messageSent) {
          errorOccurred = true;
          errorStartTime = currentTime;
          ledManager.setErrorState(); // Indicate error state
        } else {
          ledManager.setNormalOperation(); // Indicate normal operation if no errors
        }

      } catch (const std::exception& e) {
        Serial.println(e.what());
      }
    } else {
      Serial.println("WiFi not connected, skipping data send.");
      ledManager.setNoInternet(); // Indicate no internet connection
    }
  }

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
      ledManager.setNoInternet(); // Indicate no internet connection
    }
  }

  // Check if the error state should continue
  if (errorOccurred && (currentTime - errorStartTime >= 10000)) {
    errorOccurred = false;
    ledManager.setNormalOperation(); // Return to normal operation after 10 seconds
  }
}
