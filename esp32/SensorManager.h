#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

class SensorManager {
public:
    SensorManager();
    void setup(); // Initialize sensors
    void readSensors(); // Read data from sensors
    void printSensorData(); // Print sensor data to Serial

    float getTemperature() const; // Get temperature value
    float getMoisture() const; // Get moisture value
    float getLight() const; // Get light value
    float getHumidity() const; // Get humidity value

private:
    DHT dht;
    Adafruit_TSL2561_Unified tsl;
    float temperature; // Member to store temperature
    float moisture; // Member to store moisture
    float light; // Member to store light
    float humidity; // Member to store humidity
};

#endif // SENSOR_MANAGER_H
