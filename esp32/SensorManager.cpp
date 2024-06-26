#include "SensorManager.h"
#include <Wire.h>

// Pin definitions for the sensors
#define DHTPIN 15
#define DHTTYPE DHT22
#define MOISTURE_SENSOR 34

const int airValue = 3498;
const int waterValue = 1473;

// Constructor initializes the DHT and TSL2561 sensors
SensorManager::SensorManager() : dht(DHTPIN, DHTTYPE), tsl(TSL2561_ADDR_FLOAT, 12345) {}

void SensorManager::setup() {
  /*
    Setup function to initialize the sensors
  */

  Serial.begin(115200);
  dht.begin(); // Initialize DHT sensor

  // Initialize TSL2561 sensor and check if it is recognized
  if (!tsl.begin()) {
    Serial.println("TSL2561 sensor not recognized");
    while (1);
  }
}

void SensorManager::readSensors() {
  /*
    Read all sensors and verify for any errors
  */

  // Read temperature and humidity from DHT sensor
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Check for read errors from DHT sensor
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 sensor read error!");
    temperature = NAN;
    humidity = NAN;
  }

  // Read light level from TSL2561 sensor
  sensors_event_t event;
  tsl.getEvent(&event);
  if (event.light) {
    light = event.light;
  } else {
    Serial.println("Sensor overload");
  }

  // Read soil moisture level and convert to percentage
  int rawMoisture = analogRead(MOISTURE_SENSOR);
  Serial.println(rawMoisture);
  moisture = map(rawMoisture, waterValue, airValue, 0, 100); // Convert to percentage
  moisture = 100-moisture;
  if (moisture < 0) moisture = 0;
  if (moisture > 100) moisture = 100;
}

void SensorManager::printSensorData() {
  /* 
    Print all the sensors variable in the serial monitor with a descriptive text in case of detected problems for debugging purpose
  */

  Serial.println("-- Current Conditions --");

  // Print light level with descriptive text
  Serial.print("Light: ");
  Serial.print(light);
  Serial.print(" (");
  if (light < 0) {
    Serial.println("Malfunction)");
  } else if (light < 50) {
    Serial.println("Very low light)");
  } else if (light < 250) {
    Serial.println("Low light)");
  } else if (light < 500) {
    Serial.println("Moderate light)");
  } else {
    Serial.println("High light)");
  }
  Serial.println("-- End of light conditions --\n");

  // Print soil moisture level with descriptive text
  Serial.print("Soil moisture: ");
  Serial.print(moisture);
  Serial.println("%");
  if (moisture < 10) {
    Serial.println("Very dry");
  } else if (moisture < 30) {
    Serial.println("Dry");
  } else if (moisture < 60) {
    Serial.println("Wet");
  } else {
    Serial.println("Very wet");
  }
  Serial.println("-- End of soil conditions --\n");

  // Print air temperature with descriptive text
  Serial.print("Air temperature: ");
  if (temperature < -40 || temperature > 125) {
    Serial.println("Malfunction");
  } else {
    Serial.print(temperature);
    Serial.println(" °C");

    if (temperature < 15) {
      Serial.println("The room is too cold");
    } else if (temperature < 30) {
      Serial.println("The temperature is ideal");
    } else {
      Serial.println("The room is too warm");
    }
  }
  Serial.println("-- End of temperature conditions --\n");

  // Print air humidity with descriptive text
  Serial.print("Air humidity: ");
  if (humidity < 0 || humidity > 100) {
    Serial.println("Malfunction");
  } else {
    Serial.print(humidity);
    Serial.println(" %");

    if (humidity < 30) {
      Serial.println("The air is too dry");
    } else if (humidity < 60) {
      Serial.println("Humidity is ideal");
    } else {
      Serial.println("The air is too humid");
    }
  }
  Serial.println("-- End of humidity conditions --\n");
}