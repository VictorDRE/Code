#include "sensors.h"
#include <Wire.h>

// Sensor objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

// Sensor data variables
int moisture = 0;
int light = 0;
float temperature = 0;
float humidity = 0;

void setupSensors() {
  Serial.begin(115200); // Assurez-vous que cette vitesse correspond à celle du moniteur série
  dht.begin();
  if (!tsl.begin()) {
    Serial.println("Capteur TSL2561 pas reconnu");
    while (1);
  }
}

void readSensors() {
  // Reading sensor values
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Checking reading errors for DHT22
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erreur de lecture du capteur DHT22 !");
    temperature = NAN;
    humidity = NAN;
  }

  // Reading light sensor value
  sensors_event_t event;
  tsl.getEvent(&event);
  if (event.light) {
    light = event.light;
  } else {
    Serial.println("Surcharge du capteur");
  }

  // Reading moisture sensor value
  moisture = 4095 - analogRead(MOISTURE_SENSOR);
}

void printSensorData() {
  Serial.println("-- Conditions actuelles --");

  // Display luminosity conditions
  Serial.print("Luminosité : ");
  Serial.print(light);
  Serial.print(" (");
  if (light < 0) {
    Serial.println("Cas de dysfonctionnement)");
  } else if (light < 50) {
    Serial.println("Très faible luminosité)");
  } else if (light < 250) {
    Serial.println("Faible luminosité)");
  } else if (light < 500) {
    Serial.println("Luminosité modérée)");
  } else {
    Serial.println("Haute luminosité)");
  }

  Serial.println("-- Fin des conditions de luminosité --\n");

  // Display soil moisture conditions
  Serial.print("Humidité du sol : ");
  Serial.println(moisture);
  if (moisture < 0) {
    Serial.println("Cas de dysfonctionnement");
  } else if (moisture < 400) {
    Serial.println("Zone très humide");
  } else if (moisture < 500) {
    Serial.println("Zone humide");
  } else if (moisture < 700) {
    Serial.println("Zone sèche");
  } else if (moisture >= 700) {
    Serial.println("Zone très sèche");
  } else if (moisture > 1023) {
    Serial.println("Cas de dysfonctionnement");
  }
  Serial.println("-- Fin des conditions du sol --\n");

  // Air conditions display
  Serial.print("Température de l'air : ");
  if (temperature < -40 || temperature > 125) {
    Serial.println("Cas de dysfonctionnement");
  } else {
    Serial.print(temperature);
    Serial.println(" °C");

    if (temperature < 15) {
      Serial.println("La pièce est trop froide");
    } else if (temperature < 25) {
      Serial.println("La température de la pièce est confortable");
    } else {
      Serial.println("La pièce est trop chaude");
    }
  }

  Serial.print("Humidité de l'air : ");
  if (humidity < 0 || humidity > 100) {
    Serial.println("Cas de dysfonctionnement");
  } else {
    Serial.print(humidity);
    Serial.println(" %");

    if (humidity < 30) {
      Serial.println("Air trop sec");
    } else if (humidity < 60) {
      Serial.println("Air confortable");
    } else {
      Serial.println("Air trop humide");
    }
  }
  Serial.println("-- Fin des conditions de l'air --\n");
}
