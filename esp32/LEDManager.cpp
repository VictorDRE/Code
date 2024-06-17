#include "LEDManager.h"

LEDManager::LEDManager(int greenPin, int redPin) 
  : greenPin(greenPin), redPin(redPin), previousMillis(0), interval(2000), ledState(false), currentPin(-1) {}

// Setup LED pins
void LEDManager::setup() {
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  turnOffLED(greenPin);
  turnOffLED(redPin);
}

// Set LEDs to indicate normal operation
void LEDManager::setNormalOperation() {
  turnOnLED(greenPin);
  turnOffLED(redPin);
  currentPin = -1; // Stop blinking
}

// Set LEDs to indicate no internet connection
void LEDManager::setNoInternet() {
  interval = 1000;  // Blink every 1 second
  blinkLED(redPin, interval);
  turnOffLED(greenPin);
}

// Set LEDs to indicate an error state
void LEDManager::setErrorState() {
  turnOnLED(redPin);  // Continuous red LED
  turnOffLED(greenPin);
  currentPin = -1; // Stop blinking
}

// Set LEDs to indicate a critical error
void LEDManager::setCriticalError() {
  turnOnLED(redPin);  // Continuous red LED
  turnOffLED(greenPin);
  currentPin = -1; // Stop blinking
}

// Set LEDs to indicate WiFi searching
void LEDManager::setWifiSearching() {
  interval = 200;  // Blink every 200 ms
  blinkLED(greenPin, interval);
  turnOffLED(redPin);
}

// Update LED state
void LEDManager::update() {
  unsigned long currentMillis = millis();
  if (currentPin != -1 && currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(currentPin, ledState);
  }
}

// Blink an LED
void LEDManager::blinkLED(int pin, unsigned long interval) {
  this->interval = interval;
  currentPin = pin;
  previousMillis = millis();
  ledState = false;
  digitalWrite(pin, ledState);
}

// Turn on an LED
void LEDManager::turnOnLED(int pin) {
  digitalWrite(pin, HIGH);
  currentPin = -1; // Stop blinking
}

// Turn off an LED
void LEDManager::turnOffLED(int pin) {
  digitalWrite(pin, LOW);
}
