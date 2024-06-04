#ifndef SENSOR_MANAGER_H // Include guard to prevent multiple inclusions
#define SENSOR_MANAGER_H

#include <DHT.h> // Include the DHT sensor library
#include <Adafruit_TSL2561_U.h> // Include the TSL2561 light sensor library

// Define the SensorManager class
class SensorManager {
public:
  SensorManager(); // Constructor to initialize the sensors
  void setup(); // Method to set up the sensors
  void readSensors(); // Method to read data from the sensors
  void printSensorData(); // Method to print sensor data to the serial monitor

  float temperature; // Variable to store temperature data
  float humidity; // Variable to store humidity data
  int light; // Variable to store light intensity data
  int moisture; // Variable to store soil moisture data

private:
  DHT dht; // DHT sensor object for temperature and humidity
  Adafruit_TSL2561_Unified tsl; // TSL2561 sensor object for light intensity
};

#endif // SENSOR_MANAGER_H
