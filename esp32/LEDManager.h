#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>

// LEDManager class to handle LED operations
class LEDManager {
public:
  LEDManager(int greenPin, int redPin);
  void setup(); // Setup LED pins
  void setNormalOperation(); // Set LEDs to indicate normal operation
  void setNoInternet(); // Set LEDs to indicate no internet connection
  void setErrorState(); // Set LEDs to indicate an error state
  void setCriticalError(); // Set LEDs to indicate a critical error
  void setWifiSearching(); // Set LEDs to indicate WiFi searching
  void update(); // Update LED state

private:
  int greenPin; // Pin for green LED
  int redPin; // Pin for red LED
  unsigned long previousMillis; // Previous time for blinking
  unsigned long interval; // Interval for blinking
  bool ledState; // Current state of the LED
  int currentPin; // Current pin being blinked

  void blinkLED(int pin, unsigned long interval); // Blink an LED
  void turnOnLED(int pin); // Turn on an LED
  void turnOffLED(int pin); // Turn off an LED
};

#endif // LED_MANAGER_H
