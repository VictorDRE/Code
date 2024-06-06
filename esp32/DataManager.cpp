#include "DataManager.h"
#include "LEDManager.h"
#include <ctime>

extern LEDManager ledManager; // Use extern to reference the LEDManager instance

DataManager::DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, NotificationManager* notificationManager)
  : thingSpeakManager(thingSpeakManager), sensorManager(sensorManager), notificationManager(notificationManager), errorOccurred(false), errorStartTime(0), criticalEventCount(0) {}

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
          notificationManager->sendNotification("Attention to your plant. It's very cold!");
          messageSent = true;
          logCriticalEvent("Temperature too low.", "Temperature sensor");
        } else {
          resolveCriticalEvent("Temperature too low.", "Temperature sensor");
        }

        if (sensorManager->temperature > 30) {
          Serial.println("Temperature is too high, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention to your plant. High temperature!");
          messageSent = true;
          logCriticalEvent("Temperature too high.", "Temperature sensor");
        } else {
          resolveCriticalEvent("Temperature too high.", "Temperature sensor");
        }

        if (sensorManager->moisture < 10) {
          Serial.println("Moisture level is very low, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention to your plant. Needs automatic watering!");
          messageSent = true;
          logCriticalEvent("Moisture level very low.", "Moisture sensor");
        } else {
          resolveCriticalEvent("Moisture level very low.", "Moisture sensor");
        }

        if (sensorManager->moisture > 70) {
          if (currentTime - lastMoistureMessageTime >= moistureMessageInterval) {
            Serial.println("Moisture level is very high, sending WhatsApp notification...");
            notificationManager->sendNotification("Automatic watering done for your plant.");
            lastMoistureMessageTime = currentTime;
          }
          messageSent = true;
          logCriticalEvent("Moisture level very high.", "Moisture sensor");
        } else {
          resolveCriticalEvent("Moisture level very high.", "Moisture sensor");
        }

        if (sensorManager->light < 50) {
          Serial.println("Light level is too low, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention to your plant. Place it in the sun!");
          messageSent = true;
          logCriticalEvent("Light level too low.", "Light sensor");
        } else {
          resolveCriticalEvent("Light level too low.", "Light sensor");
        }

        if (sensorManager->light > 15000) {
          Serial.println("Light level is too high, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention to your plant. Place it in the shade!");
          messageSent = true;
          logCriticalEvent("Light level too high.", "Light sensor");
        } else {
          resolveCriticalEvent("Light level too high.", "Light sensor");
        }

        if (sensorManager->moisture < 30) {
          lowMoistureCount++;
        } else {
          lowMoistureCount = 0;
        }

        if (lowMoistureCount >= lowMoistureLimit) {
          Serial.println("Soil moisture is low for too long, sending WhatsApp notification...");
          notificationManager->sendNotification("Attention to your plant. Insufficient soil moisture for a prolonged period!");
          lowMoistureCount = 0;
          messageSent = true;
          logCriticalEvent("Soil moisture low for too long.", "Moisture sensor");
        } else {
          resolveCriticalEvent("Soil moisture low for too long.", "Moisture sensor");
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

  if (currentTime - lastDailySummaryTime >= dailySummaryInterval) {
    sendDailySummary();
    lastDailySummaryTime = currentTime;
  }

  // Check if the error state should continue
  if (errorOccurred && (currentTime - errorStartTime >= 10000)) {
    errorOccurred = false;
    ledManager.setNormalOperation(); // Return to normal operation after 10 seconds
  }
}

void DataManager::logCriticalEvent(const String& event, const String& sensor) {
  String timestamp = getCurrentTime();
  criticalEvents.push_back({event, sensor, timestamp, false});
  criticalEventCount++;
}

void DataManager::resolveCriticalEvent(const String& event, const String& sensor) {
  for (auto& criticalEvent : criticalEvents) {
    if (criticalEvent.event == event && criticalEvent.sensor == sensor && !criticalEvent.resolved) {
      criticalEvent.resolved = true;
    }
  }
}

void DataManager::sendDailySummary() {
  String summaryMessage = "Dans les résumés des dernières 24h il y a eu " + String(criticalEventCount) + " événements critiques:\n";
  for (const auto& criticalEvent : criticalEvents) {
    summaryMessage += "- " + criticalEvent.event + " (" + criticalEvent.sensor + ") à " + criticalEvent.timestamp;
    if (criticalEvent.resolved) {
      summaryMessage += " [Résolu]";
    }
    summaryMessage += "\n";
  }
  if (criticalEvents.empty()) {
    summaryMessage = "Dans les résumés des dernières 24h il n'y a eu aucun événement critique.";
  }

  notificationManager->sendNotification(summaryMessage);
  criticalEvents.clear(); // Clear the logs after sending the summary
  criticalEventCount = 0; // Reset the counter
}

String DataManager::getCurrentTime() {
  time_t now = time(nullptr);
  struct tm* tm_info = localtime(&now);
  char buffer[26];
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  return String(buffer);
}
