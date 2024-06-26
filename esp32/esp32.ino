#include <WiFi.h>
#include "esp_sleep.h"
#include "SensorManager.h"
#include "ThingSpeakManager.h"
#include "DataManager.h"
#include "EmailManager.h"
#include "LEDManager.h"
#include "WiFiManager.h"

// Instantiate sensor manager and ThingSpeak manager
SensorManager sensorManager;
ThingSpeakManager thingSpeakManager;
EmailManager emailManager;
LEDManager ledManager;  
WiFiManager wifiManager;
DataManager dataManager(&thingSpeakManager, &sensorManager, &ledManager, &wifiManager);

void setup() {
    Serial.begin(115200);
    delay(1000); // Delay to allow the serial monitor to start
}

void loop() {
  // Connect to WiFi
  wifiManager.connectToWiFi();

  // Initialize EmailManager
  emailManager.setup();

  // Initialize LedManager
  ledManager.setup();

  // Check time
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "Time set: %Y-%m-%d %H:%M:%S");
  } else {
    Serial.println("Failed to obtain time");
  }

  // Initialize sensors
  sensorManager.setup();

  // Initialize ThingSpeakManager
  thingSpeakManager.setup();

  // Read and send data
  dataManager.handleData();

  // Print the wakeup reason for debugging
  printWakeupReason();

  // Configure the timer to wake up the ESP32
  esp_sleep_enable_timer_wakeup(3600 * 1000000U);  //TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Go to sleep now
  Serial.println("Entering light sleep...");
  
  ledManager.setLightSleep();
  esp_light_sleep_start();
}

void printWakeupReason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}
