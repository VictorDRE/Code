#include <WiFi.h>
#include "WiFiManager.h"
#include "ThingSpeakManager.h"
#include "SensorManager.h"
#include "EmailManager.h"
#include "NotificationManager.h"
#include "DataManager.h"
#include "LEDManager.h"
#include "esp_sleep.h"

// Define LED pins
const int greenLEDPin = 12;
const int redLEDPin = 13;

// Instantiate managers
WiFiManager wifiManager;
ThingSpeakManager thingSpeakManager;
SensorManager sensorManager;
EmailManager emailManager;
NotificationManager notificationManager(&emailManager);
DataManager dataManager(&thingSpeakManager, &sensorManager, &notificationManager);
LEDManager ledManager(greenLEDPin, redLEDPin);

// Sleep time in seconds
#define TIME_TO_SLEEP  60  // 1 minute

RTC_DATA_ATTR unsigned long lastSummarySendTime = 0; // Last time summary data was sent

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for Serial to be ready
    }

    Serial.println("Setup started");

    // Initialize all components
    ledManager.setup(); // Setup LED pins
    wifiManager.connectToWiFi(); // Connect to WiFi network
    thingSpeakManager.setup(); // Initialize ThingSpeak communication
    sensorManager.setup(); // Setup sensors
    emailManager.setup(); // Initialize Email communication

    Serial.println("Setup completed");
    ledManager.setNormalOperation(); // Indicate normal operation after setup

    // Print the wakeup reason for debugging
    printWakeupReason();

    // Configure the timer to wake up the ESP32 every TIME_TO_SLEEP seconds
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);

    // Execute once at start
    dataManager.handleData(); // Handle data collection and sending
}

void loop() {
    // Calculate current time in millis since the last boot
    unsigned long currentTime = millis();

    // Handle data if not in sleep mode
    wifiManager.checkWiFiConnection(); // Check WiFi connection and attempt reconnect if disconnected

    if (WiFi.status() != WL_CONNECTED) {
        ledManager.setNoInternet(); // Indicate no internet connection
        Serial.println("No Internet connection");
    } else {
        ledManager.setNormalOperation(); // Indicate normal operation
    }

    try {
        dataManager.handleData(); // Handle data collection and sending
    } catch (const std::exception& e) {
        Serial.println(e.what());
    }

    // Check if it's time to send the summary
    if (currentTime - lastSummarySendTime >= 900000) { // 15 minutes
        dataManager.sendDailySummary();
        lastSummarySendTime = currentTime;
    }

    ledManager.update(); // Update LED state

    // Go to sleep now
    Serial.println("Going to sleep now");
    delay(1000); // Allow time for messages to be sent
    Serial.println("Entering deep sleep...");
    esp_deep_sleep_start();
}

void printWakeupReason() {
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
        default: Serial.println("Wakeup was not caused by deep sleep"); break;
    }
}
