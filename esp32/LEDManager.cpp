#include "LEDManager.h"
#include <Arduino.h>

// Setup function to initialize LED pins
void LEDManager::setup() {
  pinMode(greenLEDPin, OUTPUT); // Set green LED pin as output
  pinMode(redLEDPin, OUTPUT); // Set red LED pin as output
  digitalWrite(greenLEDPin, LOW); // Initialize green LED to off
  digitalWrite(redLEDPin, LOW); // Initialize red LED to off
}

// Set LED to indicate no internet connection
void LEDManager::setNoInternet() {
    digitalWrite(greenLEDPin, LOW); // Turn off green LED
    digitalWrite(redLEDPin, HIGH); // Turn on red LED
}

// Set LED to indicate normal operation
void LEDManager::setNormalOperation() {
    digitalWrite(greenLEDPin, HIGH); // Turn on green LED
    digitalWrite(redLEDPin, LOW); // Turn off red LED
}

// Set LED to indicate an error state
void LEDManager::setErrorState() {
    digitalWrite(greenLEDPin, LOW); // Turn off green LED
    digitalWrite(redLEDPin, HIGH); // Turn on red LED
}

// Set LED to indicate a critical error
void LEDManager::setCriticalError() {
    digitalWrite(greenLEDPin, LOW); // Turn off green LED
    digitalWrite(redLEDPin, HIGH); // Turn on red LED
}

// Set LED to indicate WiFi searching
void LEDManager::setWifiSearching() {
    digitalWrite(greenLEDPin, LOW); // Turn off green LED
    digitalWrite(redLEDPin, LOW); // Turn off red LED
}

// Set LED to indicate light sleep mode
void LEDManager::setLightSleep() {
    digitalWrite(greenLEDPin, HIGH); // Turn on green LED
    digitalWrite(redLEDPin, HIGH); // Turn on red LED
}
