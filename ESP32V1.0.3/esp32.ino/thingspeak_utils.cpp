#include <WiFi.h>
#include <ThingSpeak.h>
#include "thingspeak_utils.h"
#include "sensors.h"

WiFiClient client;
const char* thingSpeakApiKey = "C6JT20CYOZIYQJZQ";
const unsigned long channelNumber = 2565602;

void setupThingSpeak() {
  ThingSpeak.begin(client);
}

void sendDataToThingSpeak() {
  Serial.println("Sending data to ThingSpeak...");
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, light);
  ThingSpeak.setField(4, moisture);
  int x = ThingSpeak.writeFields(channelNumber, thingSpeakApiKey);
  if (x == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Error sending data to ThingSpeak");
  }
}

void sendSummaryDataToThingSpeak() {
  Serial.println("Sending summary data to ThingSpeak...");
  ThingSpeak.setField(5, temperature);
  ThingSpeak.setField(6, humidity);
  ThingSpeak.setField(7, light);
  ThingSpeak.setField(8, moisture);
  int x = ThingSpeak.writeFields(channelNumber, thingSpeakApiKey);
  if (x == 200) {
    Serial.println("Summary data sent to ThingSpeak");
  } else {
    Serial.println("Error sending summary data to ThingSpeak");
  }
}
