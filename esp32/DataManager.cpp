#include "DataManager.h"
#include "EmailManager.h"
#include <ctime>

DataManager::DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, NotificationManager* notificationManager, LEDManager* ledManager)
    : thingSpeakManager(thingSpeakManager), sensorManager(sensorManager), notificationManager(notificationManager), ledManager(ledManager),
      errorOccurred(false), errorStartTime(0), criticalDailyEventCount(0), criticalWeeklyEventCount(0), criticalMonthlyEventCount(0), sleepCount(0),
      lastMoistureMessageTime(0), lowMoistureCount(0) {
}

void DataManager::handleData() {
    unsigned long currentTime = millis();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Reading sensors and sending data...");
        readAndSendSensorData();
        lastSendTime = currentTime;
    } else {
        Serial.println("WiFi not connected, skipping data send.");
        ledManager->setNoInternet();
    }
    
    sleepCount++;

    if (errorOccurred && (currentTime - errorStartTime >= 600000)) {
        errorOccurred = false;
        ledManager->setNormalOperation();
    }
    if (sleepCount % 24 == 0) { // Every 24 sleep cycles
        sendDailySummary();
    }

    if (sleepCount % 168 == 0) { // Every 168 sleep cycles (weekly)
        sendWeeklySummary();
    }

    if (sleepCount % 720 == 0) { // Every 720 sleep cycles (monthly)
        sendMonthlySummary();
    }
    
}

void DataManager::readAndSendSensorData() {
    try {
        sensorManager->readSensors();
        sensorManager->printSensorData();
        if (!thingSpeakManager->sendData(*sensorManager)) {
            ledManager->setCriticalError();
        }
        handleNotifications();
    } catch (const std::exception& e) {
        Serial.println(e.what());
    }
}

void DataManager::handleNotifications() {
    bool messageSent = false;

    // Comment or remove these lines to prevent sending emails for temperature and light levels
    messageSent |= checkAndSendNotification(sensorManager->temperature < 10, "Temperature is too low", "Temperature sensor", "Attention to your plant. It's very cold!");
    messageSent |= checkAndSendNotification(sensorManager->temperature > 30, "Temperature is too high", "Temperature sensor", "Attention to your plant. High temperature!");
    messageSent |= checkAndSendNotification(sensorManager->moisture < 10, "Moisture level is very low", "Moisture sensor", "Attention to your plant. Needs automatic watering!");
    messageSent |= checkAndSendNotification(sensorManager->light < 50, "Light level is too low", "Light sensor", "Attention to your plant. Place it in the sun!");
    messageSent |= checkAndSendNotification(sensorManager->light > 15000, "Light level is too high", "Light sensor", "Attention to your plant. Place it in the shade!");

    if (sensorManager->moisture > 70 && (millis() - lastMoistureMessageTime >= moistureMessageInterval)) {
        Serial.println("Moisture level is very high, sending email notification...");
        // Comment this line to prevent sending notification email
        // notificationManager->sendNotification("Automatic watering done for your plant.");
        lastMoistureMessageTime = millis();
        messageSent = true;
        logCriticalEvent("Moisture level very high.", "Moisture sensor");
    } else {
        resolveCriticalEvent("Moisture level very high.", "Moisture sensor");
    }

    if (sensorManager->moisture < 30) {
        lowMoistureCount++;
    } else {
        lowMoistureCount = 0;
    }

    if (lowMoistureCount >= lowMoistureLimit) {
        Serial.println("Soil moisture is low for too long, sending email notification...");
        // Comment this line to prevent sending notification email
        // notificationManager->sendNotification("Attention to your plant. Insufficient soil moisture for a prolonged period!");
        lowMoistureCount = 0;
        messageSent = true;
        logCriticalEvent("Soil moisture low for too long.", "Moisture sensor");
    } else {
        resolveCriticalEvent("Soil moisture low for too long.", "Moisture sensor");
    }

    if (messageSent) {
        errorOccurred = true;
        errorStartTime = millis();
        ledManager->setErrorState();
    } else {
        ledManager->setNormalOperation();
    }
}



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

void DataManager::logCriticalEvent(const String& event, const String& sensor) {
    String timestamp = getCurrentTime();
    criticalDailyEvents.push_back({event, sensor, timestamp, false});
    criticalWeeklyEvents.push_back({event, sensor, timestamp, false});
    criticalMonthlyEvents.push_back({event, sensor, timestamp, false});

    criticalDailyEventCount++;
    criticalWeeklyEventCount++;
    criticalMonthlyEventCount++;

    // Send critical event email with details
    EmailManager::sendCriticalEventEmail(event + " detected by " + sensor + " at " + timestamp);
}


void DataManager::resolveCriticalEvent(const String& event, const String& sensor) {
    for (auto& criticalEvent : criticalDailyEvents) {
        if (criticalEvent.event == event && criticalEvent.sensor == sensor && !criticalEvent.resolved) {
            criticalEvent.resolved = true;
        }
    }
    for (auto& criticalEvent : criticalWeeklyEvents) {
        if (criticalEvent.event == event && criticalEvent.sensor == sensor && !criticalEvent.resolved) {
            criticalEvent.resolved = true;
        }
    }
    for (auto& criticalEvent : criticalMonthlyEvents) {
        if (criticalEvent.event == event && criticalEvent.sensor == sensor && !criticalEvent.resolved) {
            criticalEvent.resolved = true;
        }
    }
    
}

void DataManager::sendDailySummary() {
    String summaryMessage = "Résumé des dernières 24 heures :\n";
    summaryMessage += "Heure actuelle : " + getCurrentTime() + "\n";
    summaryMessage += "Événements critiques : " + String(criticalDailyEventCount) + "\n";
    for (const auto& criticalEvent : criticalDailyEvents) {
        summaryMessage += "- " + criticalEvent.event + " (" + criticalEvent.sensor + ") à " + criticalEvent.timestamp;
        if (criticalEvent.resolved) {
            summaryMessage += " [Résolu]";
        }
        summaryMessage += "\n";
    }
    if (criticalDailyEvents.empty()) {
        summaryMessage = "Aucun événement critique.";
    }

    EmailManager::sendDailySummaryEmail(summaryMessage);
    criticalDailyEvents.clear();
    criticalDailyEventCount = 0;
}

void DataManager::sendWeeklySummary() {
    String summaryMessage = "Résumé des 7 derniers jours :\n";
    summaryMessage += "Heure actuelle : " + getCurrentTime() + "\n";
    summaryMessage += "Événements critiques : " + String(criticalWeeklyEventCount) + "\n";
    for (const auto& criticalEvent : criticalWeeklyEvents) {
        summaryMessage += "- " + criticalEvent.event + " (" + criticalEvent.sensor + ") à " + criticalEvent.timestamp;
        if (criticalEvent.resolved) {
            summaryMessage += " [Résolu]";
        }
        summaryMessage += "\n";
    }
    if (criticalWeeklyEvents.empty()) {
        summaryMessage = "Aucun événement critique.";
    }

    EmailManager::sendWeeklySummaryEmail(summaryMessage);
    criticalWeeklyEvents.clear();
    criticalWeeklyEventCount = 0;
}

void DataManager::sendMonthlySummary() {
    String summaryMessage = "Résumé des 30 derniers jours :\n";
    summaryMessage += "Heure actuelle : " + getCurrentTime() + "\n";
    summaryMessage += "Événements critiques : " + String(criticalMonthlyEventCount) + "\n";
    for (const auto& criticalEvent : criticalMonthlyEvents) {
        summaryMessage += "- " + criticalEvent.event + " (" + criticalEvent.sensor + ") à " + criticalEvent.timestamp;
        if (criticalEvent.resolved) {
            summaryMessage += " [Résolu]";
        }
        summaryMessage += "\n";
    }
    if (criticalMonthlyEvents.empty()) {
        summaryMessage = "Aucun événement critique.";
    }

    EmailManager::sendMonthlySummaryEmail(summaryMessage);
    criticalMonthlyEvents.clear();
    criticalMonthlyEventCount = 0;
}

String DataManager::getCurrentTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return "N/A";
    }
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}



void DataManager::sendSummaryData() {
    try {
        thingSpeakManager->sendSummaryData(*sensorManager);
    } catch (const std::exception& e) {
        Serial.println(e.what());
    }
}
