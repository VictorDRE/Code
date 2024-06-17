#include "DataManager.h"
#include "LEDManager.h"
#include <ctime>

extern LEDManager ledManager; // Use extern to reference the LEDManager instance

DataManager::DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, NotificationManager* notificationManager)
  : thingSpeakManager(thingSpeakManager), sensorManager(sensorManager), notificationManager(notificationManager), errorOccurred(false), errorStartTime(0), criticalEventCount(0) {}

// Handle data collection and sending
void DataManager::handleData() {
    unsigned long currentTime = millis();

    // Send data immediately after waking up
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Reading sensors and sending data...");
        readAndSendSensorData();
        lastSendTime = currentTime;
    } else {
        Serial.println("WiFi not connected, skipping data send.");
        ledManager.setNoInternet(); // Indicate no internet connection
    }

    if (currentTime - lastSummarySendTime >= summaryUpdateInterval) {
        if (WiFi.status() == WL_CONNECTED) {
            sendSummaryData();
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

    if (errorOccurred && (currentTime - errorStartTime >= 600000)) { // 10 minutes
        errorOccurred = false;
        ledManager.setNormalOperation(); // Return to normal operation after 10 minutes
    }
}

// Read sensors and send data
void DataManager::readAndSendSensorData() {
    try {
        sensorManager->readSensors();
        sensorManager->printSensorData();
        if (!thingSpeakManager->sendData(*sensorManager)) {
            ledManager.setCriticalError(); // Indicate critical error after failed attempts
        }
        handleNotifications();
    } catch (const std::exception& e) {
        Serial.println(e.what());
    }
}

// Handle notifications based on sensor data
void DataManager::handleNotifications() {
    bool messageSent = false;

    messageSent |= checkAndSendNotification(sensorManager->getTemperature() < 10, "Temperature is too low", "Temperature sensor", "Attention to your plant. It's very cold!");
    messageSent |= checkAndSendNotification(sensorManager->getTemperature() > 30, "Temperature is too high", "Temperature sensor", "Attention to your plant. High temperature!");
    messageSent |= checkAndSendNotification(sensorManager->getMoisture() < 10, "Moisture level is very low", "Moisture sensor", "Attention to your plant. Needs automatic watering!");
    messageSent |= checkAndSendNotification(sensorManager->getLight() < 50, "Light level is too low", "Light sensor", "Attention to your plant. Place it in the sun!");
    messageSent |= checkAndSendNotification(sensorManager->getLight() > 15000, "Light level is too high", "Light sensor", "Attention to your plant. Place it in the shade!");

    if (sensorManager->getMoisture() > 70 && (millis() - lastMoistureMessageTime >= moistureMessageInterval)) {
        Serial.println("Moisture level is very high, sending email notification...");
        notificationManager->sendNotification("Automatic watering done for your plant.");
        lastMoistureMessageTime = millis();
        messageSent = true;
        logCriticalEvent("Moisture level very high.", "Moisture sensor");
    } else {
        resolveCriticalEvent("Moisture level very high.", "Moisture sensor");
    }

    if (sensorManager->getMoisture() < 30) {
        lowMoistureCount++;
    } else {
        lowMoistureCount = 0;
    }

    if (lowMoistureCount >= lowMoistureLimit) {
        Serial.println("Soil moisture is low for too long, sending email notification...");
        notificationManager->sendNotification("Attention to your plant. Insufficient soil moisture for a prolonged period!");
        lowMoistureCount = 0;
        messageSent = true;
        logCriticalEvent("Soil moisture low for too long.", "Moisture sensor");
    } else {
        resolveCriticalEvent("Soil moisture low for too long.", "Moisture sensor");
    }

    if (messageSent) {
        errorOccurred = true;
        errorStartTime = millis();
        ledManager.setErrorState(); // Indicate error state
    } else {
        ledManager.setNormalOperation(); // Indicate normal operation if no errors
    }
}

// Check condition and send notification
bool DataManager::checkAndSendNotification(bool condition, const String& event, const String& sensor, const String& message) {
    if (condition) {
        Serial.println(event + ", sending email notification...");
        notificationManager->sendNotification(message);
        logCriticalEvent(event, sensor);
        return true;
    } else {
        resolveCriticalEvent(event, sensor);
        return false;
    }
}

// Log a critical event
void DataManager::logCriticalEvent(const String& event, const String& sensor) {
    String timestamp = getCurrentTime();
    criticalEvents.push_back({event, sensor, timestamp, false});
    criticalEventCount++;
}

// Resolve a critical event
void DataManager::resolveCriticalEvent(const String& event, const String& sensor) {
    for (auto& criticalEvent : criticalEvents) {
        if (criticalEvent.event == event && criticalEvent.sensor == sensor && !criticalEvent.resolved) {
            criticalEvent.resolved = true;
        }
    }
}

// Send a daily summary of critical events
void DataManager::sendDailySummary() {
    String summaryMessage = "Résumé des dernières 15 minutes :\n";
    summaryMessage += "Heure actuelle : " + getCurrentTime() + "\n";
    summaryMessage += "Événements critiques : " + String(criticalEventCount) + "\n";
    for (const auto& criticalEvent : criticalEvents) {
        summaryMessage += "- " + criticalEvent.event + " (" + criticalEvent.sensor + ") à " + criticalEvent.timestamp;
        if (criticalEvent.resolved) {
            summaryMessage += " [Résolu]";
        }
        summaryMessage += "\n";
    }
    if (criticalEvents.empty()) {
        summaryMessage = "Aucun événement critique.";
    }

    notificationManager->sendNotification(summaryMessage);
    criticalEvents.clear(); // Clear the logs after sending the summary
    criticalEventCount = 0; // Reset the counter
}

// Get the current time as a string
String DataManager::getCurrentTime() {
    time_t now = time(nullptr);
    struct tm* tm_info = localtime(&now);
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    return String(buffer);
}

// Send summary data to ThingSpeak
void DataManager::sendSummaryData() {
    try {
        thingSpeakManager->sendSummaryData(*sensorManager);
    } catch (const std::exception& e) {
        Serial.println(e.what());
    }
}
