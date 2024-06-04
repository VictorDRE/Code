#include "SensorManager.h"
#include <Wire.h>

#define DHTPIN 15
#define DHTTYPE DHT22
#define MOISTURE_SENSOR 34

SensorManager::SensorManager() : dht(DHTPIN, DHTTYPE), tsl(TSL2561_ADDR_FLOAT, 12345) {}

void SensorManager::setup() {
  Serial.begin(115200); // Ensure this matches the serial monitor speed
  dht.begin();
  if (!tsl.begin()) {
    Serial.println("TSL2561 sensor not recognized");
    while (1);
  }
}

void SensorManager::readSensors() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 sensor read error!");
    temperature = NAN;
    humidity = NAN;
  }

  sensors_event_t event;
  tsl.getEvent(&event);
  if (event.light) {
    light = event.light;
  } else {
    Serial.println("Sensor overload");
  }

  moisture = 3508 - analogRead(MOISTURE_SENSOR);
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
  Serial.println(moisture);
  if (moisture < 0) {
    Serial.println("Malfunction");
  } else if (moisture < 400) {
    Serial.println("Very dry");
  } else if (moisture < 500) {
    Serial.println("Dry");
  } else if (moisture < 700) {
    Serial.println("Wet");
  } else if (moisture >= 700) {
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
