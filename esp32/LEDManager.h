#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>

class LEDManager {
public:
  LEDManager(int greenPin, int redPin);
  void setup();
  void setNormalOperation();       // LED verte allumée en continu
  void setSleepMode();             // LED verte clignotante
  void setNoInternet();            // LED rouge clignotante
  void setErrorState();            // LED rouge allumée en continu
  void update();                   // Mise à jour de l'état des LED

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
