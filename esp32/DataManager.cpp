#include "DataManager.h"
#include <ctime>

// Constructor initializes all manager pointers and event counters
DataManager::DataManager(ThingSpeakManager* thingSpeakManager, SensorManager* sensorManager, LEDManager* ledManager, WiFiManager* wifiManager)
    : thingSpeakManager(thingSpeakManager), sensorManager(sensorManager), ledManager(ledManager), wifiManager(wifiManager){
}

void DataManager::handleData() {
    /*
        Function that checks for internet connection, 
        read and send sensor data to ThingSpeak and, 
        send a summary email every day, week, month
    */

    unsigned long currentTime = millis();
    // Check WiFi connection status
    wifiManager->checkWiFiConnection();
    
    readAndSendSensorData();
}

void DataManager::readAndSendSensorData() {
    /*
        Read, print and send sensor data to ThingSpeak
    */

    try {
        sensorManager->readSensors(); // Read sensor data
        sensorManager->printSensorData(); // Print sensor data for debugging
        // Send data to ThingSpeak and handle potential errors
        if (!thingSpeakManager->sendData(*sensorManager)) {
            ledManager->setCriticalError(); // Set LED to indicate a critical error
        }
    } catch (const std::exception& e) {
        Serial.println(e.what()); // Print any exceptions
    }
}