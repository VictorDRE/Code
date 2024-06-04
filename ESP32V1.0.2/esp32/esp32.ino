#include <WiFi.h>
#include <HTTPClient.h>
#include <ThingSpeak.h>
#include "sensors.h"

// Configuration WiFi
const char* ssid = "Ekip";
const char* password = "oooooooo";

// Configuration WhatsApp
String apiKey = "9541441";              // Ajoutez votre Token ici
String phone_number = "+33778174958";   // Ajoutez votre numéro de téléphone WhatsApp ici

// Configuration ThingSpeak
const char* thingSpeakApiKey = "C6JT20CYOZIYQJZQ";
const unsigned long channelNumber = 2565602;

WiFiClient client;
unsigned long lastUpdateTime = 0;
unsigned long lastSendTime = 0;
const unsigned long updateInterval = 15 * 1000;  // Intervalle de 15 secondes
const unsigned long summaryUpdateInterval = 10 * 60 * 1000; // Intervalle de 10 minutes

// Variables pour le suivi de l'état sur une longue période
const unsigned long moistureCheckInterval = 6 * 60 * 60 * 1000; // Intervalle de 6 heures
unsigned long lastMoistureCheckTime = 0;
int lowMoistureCount = 0;

const int temperatureThreshold = 30; // Seuil de température élevée
const int lightThreshold = 50;       // Seuil de lumière faible
const int moistureThreshold = 550;   // Seuil d'humidité du sol basse
const int lowMoistureLimit = 3;      // Nombre de fois où l'humidité du sol est basse avant d'envoyer une notification

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  ThingSpeak.begin(client);
  setupSensors();
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSendTime >= updateInterval) {
    readSensors();
    printSensorData();  // Affiche les données lues
    sendDataToThingSpeak();
    lastSendTime = currentTime;

    // Envoyer une notification WhatsApp si nécessaire
    if (temperature > temperatureThreshold) {
      message_to_whatsapp("Attention à votre plante. Température élevée !");
    }
    if (light < lightThreshold) {
      message_to_whatsapp("Attention à votre plante. Lumière insuffisante !");
    }

    // Vérifiez l'humidité du sol et mettez à jour le compteur
    if (moisture < moistureThreshold) {
      lowMoistureCount++;
    } else {
      lowMoistureCount = 0; // Réinitialiser le compteur si l'humidité est suffisante
    }

    // Envoyer une notification si l'humidité du sol est basse pendant trop longtemps
    if (lowMoistureCount >= lowMoistureLimit) {
      message_to_whatsapp("Attention à votre plante. Humidité du sol insuffisante pendant une période prolongée !");
      lowMoistureCount = 0; // Réinitialiser le compteur après avoir envoyé la notification
    }
  }

  if (currentTime - lastUpdateTime >= summaryUpdateInterval) {
    sendSummaryDataToThingSpeak();
    lastUpdateTime = currentTime;
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
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
    Serial.println("---------- Summary data sent to ThingSpeak ----------");
  } else {
    Serial.println("Error sending summary data to ThingSpeak");
  }
}

void message_to_whatsapp(String message) {
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);
  postData(url);
}

void postData(String url) {
  int httpCode;
  HTTPClient http;
  http.begin(url);
  httpCode = http.POST(url);
  if (httpCode == 200) {
    Serial.println("WhatsApp message sent successfully");
  } 
  http.end();
}

String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  char code2;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }
  return encodedString;
}
