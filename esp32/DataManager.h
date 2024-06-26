#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "ThingSpeakManager.h"
#include "SensorManager.h"
#include "NotificationManager.h"
#include "LEDManager.h"
#include <vector>

struct CriticalEvent {
    String event;
    String sensor;
    String timestamp;
    bool resolved;
};

class DataManager {
public:
    DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, NotificationManager* notificationManager, LEDManager* ledManager);
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
    NotificationManager* notificationManager; // Pointer to NotificationManager
    LEDManager* ledManager; // Pointer to LEDManager
    std::vector<CriticalEvent> criticalDailyEvents; // Vector to store critical events
    std::vector<CriticalEvent> criticalWeeklyEvents; // Vector to store critical events
    std::vector<CriticalEvent> criticalMonthlyEvents; // Vector to store critical events
    unsigned long lastSendTime = 0; // Last time data was sent
    unsigned long updateInterval = 20000; // Interval between data updates (20 seconds)
    unsigned long sleepCount = 0; // Counter for sleep cycles
    unsigned long lastMoistureMessageTime = 0; // Last time a moisture message was sent
    unsigned long moistureMessageInterval = 3600000; // Interval between moisture messages (1 hour)
    int lowMoistureCount = 0; // Count of low moisture readings
    int lowMoistureLimit = 5; // Limit of low moisture readings before notification
    bool errorOccurred; // Flag to indicate if an error occurred
    unsigned long errorStartTime; // Time when the error occurred
    int criticalDailyEventCount; // Counter for critical events
    int criticalWeeklyEventCount; // Counter for critical events
    int criticalMonthlyEventCount; // Counter for critical events

    String getCurrentTime(); // Method to get current time as a string
    void readAndSendSensorData(); // Method to read sensor data and send it to ThingSpeak
    void handleNotifications(); // Method to handle notifications
    bool checkAndSendNotification(bool condition, const String& event, const String& sensor, const String& message); // Check condition and send notification
};

#endif // DATA_MANAGER_H
