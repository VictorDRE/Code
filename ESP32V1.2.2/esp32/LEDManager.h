#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>

class LEDManager {
public:
  LEDManager(int greenPin, int redPin);
  void setup();
  void setActiveState();
  void setSleepState();
  void setUnconfiguredState();
  void setConnectingState();
  void setErrorState();
  void update();

private:
  int greenPin;
  int redPin;
  unsigned long previousMillis;
  unsigned long interval;
  bool ledState;

  void blinkLED(int pin, unsigned long interval);
  void turnOnLED(int pin);
  void turnOffLED(int pin);
};

#endif // LED_MANAGER_H
