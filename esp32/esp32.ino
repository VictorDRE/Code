#include <WiFi.h>
#include "esp_sleep.h"
#include "SensorManager.h"
#include "ThingSpeakManager.h"
#include "DataManager.h"
#include "EmailManager.h"
#include "NotificationManager.h"
#include "LEDManager.h"

#define uS_TO_S_FACTOR 100000000  
#define TIME_TO_SLEEP  100      

// Define LED pins
const int greenLEDPin = 12;
const int redLEDPin = 13;

// WiFi credentials
const char* ssid = "SII-BOURGES-SECOURS";
const char* password = "B0uRG35S11!!2019";

// Instantiate sensor manager and ThingSpeak manager
SensorManager sensorManager;
ThingSpeakManager thingSpeakManager;
EmailManager emailManager;
LEDManager ledManager; // Assurez-vous que cela est une instance globale
NotificationManager notificationManager(&emailManager);
DataManager dataManager(&thingSpeakManager, &sensorManager, &notificationManager, &ledManager);

void setup() {
    Serial.begin(115200);
    delay(1000); // Delay to allow the serial monitor to start

    Serial.println("Setup started");

    // Setup LED pins
    pinMode(greenLEDPin, OUTPUT);
    pinMode(redLEDPin, OUTPUT);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 10) {
        delay(500);
        Serial.print(".");
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi Connected");
        digitalWrite(greenLEDPin, HIGH); // Turn on green LED
    } else {
        Serial.println("Failed to connect to WiFi");
        digitalWrite(redLEDPin, HIGH); // Turn on red LED
    }

    // Initialize EmailManager
    EmailManager::setup();

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
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for " + String(TIME_TO_SLEEP) + " seconds");

    // Go to sleep now
    Serial.println("Going to sleep now");
    delay(1000); // Allow time for messages to be sent
    Serial.println("Entering deep sleep...");
    esp_deep_sleep_start();
}


void loop() {
  // The ESP32 will never reach here. Loop code is not needed because the ESP32 will sleep and wake up based on timer.
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
