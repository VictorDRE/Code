#include "DataManager.h"
#include "EmailManager.h"
#include <ctime>

// Constructor initializes all manager pointers and event counters
DataManager::DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, LEDManager* ledManager, WiFiManager* wifiManager)
    : thingSpeakManager(thingSpeakManager), sensorManager(sensorManager), ledManager(ledManager), wifiManager(wifiManager),
      criticalDailyEventCount(0), criticalWeeklyEventCount(0), criticalMonthlyEventCount(0), sleepCount(0),
      lastMoistureMessageTime(0) {
}

// Main data handling function, called periodically
void DataManager::handleData() {
    unsigned long currentTime = millis();
    // Check WiFi connection status
    wifiManager->checkWiFiConnection();
    
    // Increment sleep count on each call
    sleepCount++;

    // Send daily, weekly, and monthly summaries at respective intervals
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

// Function to read sensor data and send it to ThingSpeak
void DataManager::readAndSendSensorData() {
    try {
        sensorManager->readSensors(); // Read sensor data
        sensorManager->printSensorData(); // Print sensor data for debugging
        // Send data to ThingSpeak and handle potential errors
        if (!thingSpeakManager->sendData(*sensorManager)) {
            ledManager->setCriticalError(); // Set LED to indicate a critical error
        }
        handleNotifications(); // Handle any notifications based on sensor data
    } catch (const std::exception& e) {
        Serial.println(e.what()); // Print any exceptions
    }
}

// Function to handle notifications based on sensor data
void DataManager::handleNotifications() {
    bool messageSent = false;

    // Check sensor conditions and send notifications accordingly
    messageSent |= checkAndSendNotification(sensorManager->temperature < 10, "Temperature is too low", "Temperature sensor", "Attention to your plant. It's very cold!");
    messageSent |= checkAndSendNotification(sensorManager->temperature > 30, "Temperature is too high", "Temperature sensor", "Attention to your plant. High temperature!");
    messageSent |= checkAndSendNotification(sensorManager->moisture < 10, "Moisture level is very low", "Moisture sensor", "Attention to your plant. Needs automatic watering!");
    messageSent |= checkAndSendNotification(sensorManager->light < 50, "Light level is too low", "Light sensor", "Attention to your plant. Place it in the sun!");
    messageSent |= checkAndSendNotification(sensorManager->light > 15000, "Light level is too high", "Light sensor", "Attention to your plant. Place it in the shade!");

    // Update LED state based on whether any messages were sent
    if (messageSent) {
        ledManager->setErrorState(); // Set LED to error state
    } else {
        ledManager->setNormalOperation(); // Set LED to normal operation
    }
}

// Function to check conditions and send notifications if necessary
bool DataManager::checkAndSendNotification(bool condition, const String& event, const String& sensor, const String& message) {
    if (condition) {
        Serial.println(event + ", sending email notification..."); // Log event
        logCriticalEvent(event, sensor); // Log critical event
        return true;
    } else {
        resolveCriticalEvent(event, sensor); // Resolve critical event if condition no longer true
        return false;
    }
}

// Log critical events with details
void DataManager::logCriticalEvent(const String& event, const String& sensor) {
    String timestamp = getCurrentTime(); // Get current time
    criticalDailyEvents.push_back({event, sensor, timestamp, false}); // Log event in daily list
    criticalWeeklyEvents.push_back({event, sensor, timestamp, false}); // Log event in weekly list
    criticalMonthlyEvents.push_back({event, sensor, timestamp, false}); // Log event in monthly list

    criticalDailyEventCount++; // Increment daily event count
    criticalWeeklyEventCount++; // Increment weekly event count
    criticalMonthlyEventCount++; // Increment monthly event count

    // Send email notification for critical event
    EmailManager::sendCriticalEventEmail(event + " detected by " + sensor + " at " + timestamp);
}

void DataManager::resolveCriticalEvent(const String& event, const String& sensor) {
    // Mark event as resolved in daily events list
    for (auto& criticalEvent : criticalDailyEvents) {
        if (criticalEvent.event == event && criticalEvent.sensor == sensor && !criticalEvent.resolved) {
            criticalEvent.resolved = true;
        }
    }
    // Mark event as resolved in weekly events list
    for (auto& criticalEvent : criticalWeeklyEvents) {
        if (criticalEvent.event == event && criticalEvent.sensor == sensor && !criticalEvent.resolved) {
            criticalEvent.resolved = true;
        }
    }
    // Mark event as resolved in monthly events list
    for (auto& criticalEvent : criticalMonthlyEvents) {
        if (criticalEvent.event == event && criticalEvent.sensor == sensor && !criticalEvent.resolved) {
            criticalEvent.resolved = true;
        }
    }
}

// Send daily summary email
void DataManager::sendDailySummary() {
    String summaryMessage = "Résumé des dernières 24 heures :\n";
    summaryMessage += "Heure actuelle : " + getCurrentTime() + "\n";
    summaryMessage += "Événements critiques : " + String(criticalDailyEventCount) + "\n";
    // Add critical event details to summary
    for (const auto& criticalEvent : criticalDailyEvents) {
        summaryMessage += "- " + criticalEvent.event + " (" + criticalEvent.sensor + ") à " + criticalEvent.timestamp;
        if (criticalEvent.resolved) {
            summaryMessage += " [Résolu]";
        }
        summaryMessage += "\n";
    }
    // If no events, indicate none
    if (criticalDailyEvents.empty()) {
        summaryMessage = "Aucun événement critique.";
    }

    // Send email with daily summary
    EmailManager::sendDailySummaryEmail(summaryMessage);
    criticalDailyEvents.clear(); // Clear daily events list
    criticalDailyEventCount = 0; // Reset daily event count
}

// Send weekly summary email
void DataManager::sendWeeklySummary() {
    String summaryMessage = "Résumé des 7 derniers jours :\n";
    summaryMessage += "Heure actuelle : " + getCurrentTime() + "\n";
    summaryMessage += "Événements critiques : " + String(criticalWeeklyEventCount) + "\n";
    // Add critical event details to summary
    for (const auto& criticalEvent : criticalWeeklyEvents) {
        summaryMessage += "- " + criticalEvent.event + " (" + criticalEvent.sensor + ") à " + criticalEvent.timestamp;
        if (criticalEvent.resolved) {
            summaryMessage += " [Résolu]";
        }
        summaryMessage += "\n";
    }
    // If no events, indicate none
    if (criticalWeeklyEvents.empty()) {
        summaryMessage = "Aucun événement critique.";
    }

    // Send email with weekly summary
    EmailManager::sendWeeklySummaryEmail(summaryMessage);
    criticalWeeklyEvents.clear(); // Clear weekly events list
    criticalWeeklyEventCount = 0; // Reset weekly event count
}

// Send monthly summary email
void DataManager::sendMonthlySummary() {
    String summaryMessage = "Résumé des 30 derniers jours :\n";
    summaryMessage += "Heure actuelle : " + getCurrentTime() + "\n";
    summaryMessage += "Événements critiques : " + String(criticalMonthlyEventCount) + "\n";
    // Add critical event details to summary
    for (const auto& criticalEvent : criticalMonthlyEvents) {
        summaryMessage += "- " + criticalEvent.event + " (" + criticalEvent.sensor + ") à " + criticalEvent.timestamp;
        if (criticalEvent.resolved) {
            summaryMessage += " [Résolu]";
        }
        summaryMessage += "\n";
    }
    // If no events, indicate none
    if (criticalMonthlyEvents.empty()) {
        summaryMessage = "Aucun événement critique.";
    }

    // Send email with monthly summary
    EmailManager::sendMonthlySummaryEmail(summaryMessage);
    criticalMonthlyEvents.clear(); // Clear monthly events list
    criticalMonthlyEventCount = 0; // Reset monthly event count
}

// Function to get the current local time as a string
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

// Function to send summary data to ThingSpeak
void DataManager::sendSummaryData() {
    try {
        thingSpeakManager->sendSummaryData(*sensorManager); // Send summary data
    } catch (const std::exception& e) {
        Serial.println(e.what()); // Print any exceptions
    }
}