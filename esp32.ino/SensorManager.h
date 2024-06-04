#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <DHT.h>
#include <Adafruit_TSL2561_U.h>

class SensorManager {
public:
  SensorManager();
  void setup();
  void readSensors();
  void printSensorData();

  float temperature;
  float humidity;
  int light;
  int moisture;

private:
  DHT dht;
  Adafruit_TSL2561_Unified tsl;
};

#endif // SENSOR_MANAGER_H
