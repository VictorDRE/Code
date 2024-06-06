#include "LEDManager.h"

LEDManager::LEDManager(int greenPin, int redPin) 
  : greenPin(greenPin), redPin(redPin), previousMillis(0), interval(2000), ledState(false), currentPin(-1) {}

void LEDManager::setup() {
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  turnOffLED(greenPin);
  turnOffLED(redPin);
}

void LEDManager::setNormalOperation() {
  turnOnLED(greenPin);
  turnOffLED(redPin);
  currentPin = -1; // Stop blinking
}

void LEDManager::setNoInternet() {
  interval = 2000;  // Blinks every 2 seconds
  blinkLED(redPin, interval);
  turnOffLED(greenPin);
}

void LEDManager::setErrorState() {
  turnOnLED(redPin);  // Continuous red LED
  turnOffLED(greenPin);
  currentPin = -1; // Stop blinking
}

void LEDManager::update() {
  unsigned long currentMillis = millis();
  if (currentPin != -1 && currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(currentPin, ledState);
  }
}

void LEDManager::blinkLED(int pin, unsigned long interval) {
  this->interval = interval;
  currentPin = pin;
  previousMillis = millis();
  ledState = false;
  digitalWrite(pin, ledState);
}

void LEDManager::turnOnLED(int pin) {
  digitalWrite(pin, HIGH);
  currentPin = -1; // Stop blinking
}

void LEDManager::turnOffLED(int pin) {
  digitalWrite(pin, LOW);
}
