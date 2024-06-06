#include "SensorManager.h"
#include <Wire.h>

#define DHTPIN 15
#define DHTTYPE DHT22
#define MOISTURE_SENSOR 34

SensorManager::SensorManager() 
  : dht(DHTPIN, DHTTYPE), 
    tsl(TSL2561_ADDR_FLOAT, 12345), 
    tslInitialized(false), 
    lastRetryTime(0), 
    retryCount(0) {}

void SensorManager::setup() {
  Serial.begin(115200);
  dht.begin();
  retryTSLSetup(); // Initial attempt to setup TSL2561
}

void SensorManager::retryTSLSetup() {
  if (!tsl.begin()) {
    Serial.println("TSL2561 sensor not recognized");
    tslInitialized = false;
    lastRetryTime = millis();
    retryCount++;
  } else {
    tslInitialized = true;
    Serial.println("TSL2561 sensor initialized successfully");
  }
}

void SensorManager::readSensors() {
  // Retry TSL2561 setup if it failed previously and the retry count has not exceeded max retries
  if (!tslInitialized && retryCount <= maxRetries && millis() - lastRetryTime >= retryInterval) {
    retryTSLSetup();
  }

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 sensor read error!");
    temperature = NAN;
    humidity = NAN;
  }

  if (tslInitialized) {
    sensors_event_t event;
    tsl.getEvent(&event);
    if (event.light) {
      light = event.light;
    } else {
      Serial.println("Sensor overload");
    }
  } else {
    light = -1; // Set a default error value or handle the error as needed
  }

  int rawMoisture = analogRead(MOISTURE_SENSOR);
  moisture = map(rawMoisture, 3508, 0, 0, 100); // Convert to percentage
  if (moisture < 0) moisture = 0;
  if (moisture > 100) moisture = 100;
}

void SensorManager::printSensorData() {
  Serial.println("-- Current Conditions --");

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
