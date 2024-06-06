#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>

class LEDManager {
public:
  LEDManager(int greenPin, int redPin);
  void setup();
  void setNormalOperation();       // Green LED on continuously
  void setNoInternet();            // Red LED blinking
  void setErrorState();            // Red LED on continuously
  void update();                   // Update LED state

private:
  int greenPin;
  int redPin;
  unsigned long previousMillis;
  unsigned long interval;
  bool ledState;
  int currentPin;

  void blinkLED(int pin, unsigned long interval);
  void turnOnLED(int pin);
  void turnOffLED(int pin);
};

#endif // LED_MANAGER_H
