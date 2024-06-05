#include "SensorManager.h" // Include the header file for the SensorManager class
#include <Wire.h> // Include the Wire library for I2C communication

// Define sensor pins
#define DHTPIN 15 // Pin where the DHT22 sensor is connected
#define DHTTYPE DHT22 // Type of DHT sensor used (DHT22)
#define MOISTURE_SENSOR 34 // Pin where the soil moisture sensor is connected

// Constructor for the SensorManager class
SensorManager::SensorManager() : dht(DHTPIN, DHTTYPE), tsl(TSL2561_ADDR_FLOAT, 12345) {}

// Setup method to initialize sensors
void SensorManager::setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  dht.begin(); // Initialize the DHT22 sensor
  if (!tsl.begin()) { // Check if the TSL2561 sensor is recognized
    Serial.println("TSL2561 sensor not recognized"); // Print an error message if the sensor is not recognized
    while (1); // Infinite loop in case of error
  }
}

// Method to read sensor data
void SensorManager::readSensors() {
  temperature = dht.readTemperature(); // Read temperature from the DHT22 sensor
  humidity = dht.readHumidity(); // Read humidity from the DHT22 sensor

  // Check if reading temperature or humidity failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 sensor read error!"); // Print an error message if there's a problem
    temperature = NAN; // Set values to NAN in case of error
    humidity = NAN;
  }

  sensors_event_t event; // Create an event to store light sensor data
  tsl.getEvent(&event); // Read data from the TSL2561 sensor
  if (event.light) {
    light = event.light; // If light data is valid, store it
  } else {
    Serial.println("Sensor overload"); // Print a message if the sensor is overloaded
  }

  moisture = 3508 - analogRead(MOISTURE_SENSOR); // Read and adjust data from the soil moisture sensor
}

// Method to print sensor data
void SensorManager::printSensorData() {
  Serial.println("-- Current Conditions --");

  // Print light sensor data
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

  // Print soil moisture data
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

  // Print air temperature data
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

  // Print air humidity data
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
