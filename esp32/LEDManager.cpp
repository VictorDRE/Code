#include "LEDManager.h"
#include <Arduino.h>

const int greenLEDPin = 12;
const int redLEDPin = 13;

void LEDManager::setNoInternet() {
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
}

void LEDManager::setNormalOperation() {
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
}

void LEDManager::setErrorState() {
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
}

void LEDManager::setCriticalError() {
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
}

void LEDManager::setWifiSearching() {
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
  
}
