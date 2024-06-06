#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "ThingSpeakManager.h"
#include "SensorManager.h"
#include "NotificationManager.h"
#include <vector>

struct CriticalEvent {
  String event;
  String sensor;
  String timestamp;
  bool resolved;
};

class DataManager {
public:
  DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, NotificationManager* notificationManager);
  void handleData();
  void logCriticalEvent(const String& event, const String& sensor);
  void resolveCriticalEvent(const String& event, const String& sensor);

private:
  ThingSpeakManager* thingSpeakManager;
  SensorManager* sensorManager;
  NotificationManager* notificationManager;
  std::vector<CriticalEvent> criticalEvents; // Vector to store critical events
  unsigned long lastSendTime = 0;
  unsigned long updateInterval = 20000; // 20 seconds
  unsigned long lastSummarySendTime = 0;
  unsigned long summaryUpdateInterval = 900000; // 15 minutes in milliseconds
  unsigned long lastDailySummaryTime = 0;
  unsigned long dailySummaryInterval = 86400000; // 24 hours in milliseconds
  unsigned long lastMoistureMessageTime = 0; // Last watering message time
  unsigned long moistureMessageInterval = 3600000; // 1 hour in milliseconds
  int lowMoistureCount = 0;
  int lowMoistureLimit = 5; // Number of low moisture readings before notification
  bool errorOccurred; // Flag to indicate if an error occurred
  unsigned long errorStartTime; // Time when the error occurred
  int criticalEventCount; // Counter for critical events

  void sendDailySummary(); // Method to send daily summary
  String getCurrentTime(); // Method to get current time as a string
};

#endif // DATA_MANAGER_H
