#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include <Adafruit_TSL2561_U.h>

// Pin definition for sensors
#define DHTPIN 15 // Digital pin for DHT22 (modifié pour l'ESP32)
#define DHTTYPE DHT22 // DHT sensor type
#define MOISTURE_SENSOR 34 // Analog pin for moisture sensor V1.2 (modifié pour l'ESP32)

// Sensor objects
extern DHT dht;
extern Adafruit_TSL2561_Unified tsl;

// Sensor data variables
extern int moisture;
extern int light;
extern float temperature;
extern float humidity;

// Function prototypes
void setupSensors();
void readSensors();
void printSensorData();

#endif // SENSORS_H
