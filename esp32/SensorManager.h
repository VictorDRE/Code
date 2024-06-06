#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <DHT.h> // Include the DHT sensor library
#include <Adafruit_TSL2561_U.h> // Include the TSL2561 light sensor library

// SensorManager class to handle sensor operations
class SensorManager {
public:
  SensorManager(); // Constructor to initialize the sensors
  void setup(); // Method to set up the sensors
  void readSensors(); // Method to read data from the sensors
  void printSensorData(); // Method to print sensor data to the serial monitor
  void retryTSLSetup(); // Method to retry TSL2561 setup

  float temperature; // Variable to store temperature data
  float humidity; // Variable to store humidity data
  int light; // Variable to store light intensity data
  int moisture; // Variable to store soil moisture data

private:
  DHT dht; // DHT sensor object for temperature and humidity
  Adafruit_TSL2561_Unified tsl; // TSL2561 sensor object for light intensity
  bool tslInitialized; // Flag to check if TSL2561 sensor was initialized successfully
  unsigned long lastRetryTime; // Last time a retry was attempted
  int retryCount; // Number of retries attempted
  const int maxRetries = 5; // Maximum number of retries
  const unsigned long retryInterval = 5000; // Interval between retries in milliseconds
};

#endif // SENSOR_MANAGER_H
