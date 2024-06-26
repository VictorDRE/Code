#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "ThingSpeakManager.h"
#include "SensorManager.h"
#include "LEDManager.h"
#include "WiFiManager.h"
#include <vector>

struct CriticalEvent {
    String event;
    String sensor;
    String timestamp;
    bool resolved;
};

class DataManager {
public:
    DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, LEDManager* ledManager,WiFiManager* wifiManager);
    void handleData(); // Handle data collection and sending
    void logCriticalEvent(const String& event, const String& sensor); // Log a critical event
    void resolveCriticalEvent(const String& event, const String& sensor); // Resolve a critical event
    void sendDailySummary(); // Method to send daily summary
    void sendWeeklySummary(); // Method to send weekly summary
    void sendMonthlySummary(); // Method to send monthly summary
    void sendSummaryData(); // Method to send summary data to ThingSpeak

private:
    ThingSpeakManager* thingSpeakManager; // Pointer to ThingSpeakManager
    SensorManager* sensorManager; // Pointer to SensorManager
    LEDManager* ledManager; // Pointer to LEDManager
    WiFiManager* wifiManager;
    std::vector<CriticalEvent> criticalDailyEvents; // Vector to store critical events
    std::vector<CriticalEvent> criticalWeeklyEvents; // Vector to store critical events
    std::vector<CriticalEvent> criticalMonthlyEvents; // Vector to store critical events
    unsigned long sleepCount = 0; // Counter for sleep cycles
    int criticalDailyEventCount; // Counter for critical events
    int criticalWeeklyEventCount; // Counter for critical events
    int criticalMonthlyEventCount; // Counter for critical events

    String getCurrentTime(); // Method to get current time as a string
    void readAndSendSensorData(); // Method to read sensor data and send it to ThingSpeak
    void handleNotifications(); // Method to handle notifications
    bool checkAndSendNotification(bool condition, const String& event, const String& sensor, const String& message); // Check condition and send notification
};

#endif // DATA_MANAGER_H
