#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "ThingSpeakManager.h"
#include "SensorManager.h"
#include "NotificationManager.h"

// DataManager class to handle data operations
class DataManager {
public:
  DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, NotificationManager* notificationManager);
  void handleData();

private:
  ThingSpeakManager* thingSpeakManager;
  SensorManager* sensorManager;
  NotificationManager* notificationManager;
  unsigned long lastSendTime = 0;
  unsigned long updateInterval = 20000; // 20 seconds
  unsigned long lastSummarySendTime = 0;
  unsigned long summaryUpdateInterval = 900000; // 15 minutes in milliseconds
  unsigned long lastMoistureMessageTime = 0; // Last watering message time
  unsigned long moistureMessageInterval = 3600000; // 1 hour in milliseconds
  int lowMoistureCount = 0;
  int lowMoistureLimit = 5; // Number of low moisture readings before notification
};

#endif // DATA_MANAGER_H
