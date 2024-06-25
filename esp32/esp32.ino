#include <WiFi.h>
#include "esp_sleep.h"
#include "SensorManager.h"
#include "ThingSpeakManager.h"
#include "DataManager.h"
#include "EmailManager.h"
#include "NotificationManager.h"
#include "LEDManager.h"
#include "WiFiManager.h"

//#define uS_TO_S_FACTOR 1000000
//#define TIME_TO_SLEEP  60

const int greenLEDPin = 12;
const int redLEDPin = 13;

// Instantiate sensor manager and ThingSpeak manager
SensorManager sensorManager;
ThingSpeakManager thingSpeakManager;
EmailManager emailManager;
LEDManager ledManager;  // Assurez-vous que cela est une instance globale
WiFiManager wifiManager;
NotificationManager notificationManager(&emailManager);
DataManager dataManager(&thingSpeakManager, &sensorManager, &notificationManager, &ledManager);

void setup() {
  Serial.begin(115200);
  delay(1000);  // Delay to allow the serial monitor to start

  Serial.println("Setup started");

  // Setup LED pins
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  digitalWrite(greenLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);
}


void loop() {
  // Connect to WiFi
  wifiManager.connectToWiFi();

  // Initialize EmailManager
  emailManager.setup();

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

  // Handle data
  dataManager.handleData();

  // Print the wakeup reason for debugging
  printWakeupReason();

  // Configure the timer to wake up the ESP32 every 100 seconds
  esp_sleep_enable_timer_wakeup(3600000000);  //TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Go to sleep now
  Serial.println("Going to sleep now");
  delay(1000);  // Allow time for messages to be sent
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
