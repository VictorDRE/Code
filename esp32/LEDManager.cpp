#include "LEDManager.h"
#include <Arduino.h>


void LEDManager::setup() {
    /*
        Setup function to initialize LED pins    
    */
    
    pinMode(greenLEDPin, OUTPUT);
    pinMode(redLEDPin, OUTPUT);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
}


void LEDManager::setNoInternet() {
    /*
        Set LED to indicate no internet connection
        Green LED : OFF
        Red LED : ON
    */

    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
}

void LEDManager::setNormalOperation() {
    /*
        Set LED to indicate normal operation
        Green LED : ON
        Red LED : OFF
    */

    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
}

void LEDManager::setErrorState() {
    /*
        Set LED to indicate an error state
        Green LED : OFF
        Red LED : ON
    */

    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
}

void LEDManager::setCriticalError() {
    /*
        Set LED to indicate a critical error
        Green LED : OFF
        Red LED : ON
    */

    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
}

void LEDManager::setWifiSearching() {
    /*
        Set LED to indicate WiFi searching
        Green LED : OFF
        Red LED : OFF
    */

    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
}

void LEDManager::setLightSleep() {
    /*
        Set LED to indicate light sleep mode
        Green LED : ON
        Red LED : ON
    */
    
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, HIGH);
}
