#include "LEDManager.h"

LEDManager::LEDManager(int greenPin, int redPin) : greenPin(greenPin), redPin(redPin), previousMillis(0), interval(2000), ledState(false) {}

void LEDManager::setup() {
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  turnOffLED(greenPin);
  turnOffLED(redPin);
}

void LEDManager::setActiveState() {
  interval = 2000;
  blinkLED(greenPin, interval);
  turnOffLED(redPin);
}

void LEDManager::setSleepState() {
  interval = 15000;
  blinkLED(greenPin, interval);
  turnOffLED(redPin);
}

void LEDManager::setUnconfiguredState() {
  interval = 15000;
  blinkLED(redPin, interval);
  turnOffLED(greenPin);
}

void LEDManager::setConnectingState() {
  interval = 2000;
  blinkLED(redPin, interval);
  turnOffLED(greenPin);
}

void LEDManager::setErrorState() {
  turnOnLED(redPin);
  turnOffLED(greenPin);
}

void LEDManager::update() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(greenPin, ledState);
    digitalWrite(redPin, ledState);
  }
}

void LEDManager::blinkLED(int pin, unsigned long interval) {
  this->interval = interval;
  digitalWrite(pin, ledState);
}

void LEDManager::turnOnLED(int pin) {
  digitalWrite(pin, HIGH);
}

void LEDManager::turnOffLED(int pin) {
  digitalWrite(pin, LOW);
}
