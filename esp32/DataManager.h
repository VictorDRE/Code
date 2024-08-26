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
  
private:
    ThingSpeakManager* thingSpeakManager; // Pointer to ThingSpeakManager
    SensorManager* sensorManager; // Pointer to SensorManager
    LEDManager* ledManager; // Pointer to LEDManager
    WiFiManager* wifiManager;

    void readAndSendSensorData(); // Method to read sensor data and send it to ThingSpeak
};

#endif // DATA_MANAGER_H
