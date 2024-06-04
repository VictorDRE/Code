#include "wifi_utils.h"
#include "thingspeak_utils.h"
#include "sensors.h"
#include "url_utils.h"
#include "whatsapp_utils.h"
#include "config.h"

// Variables globales
unsigned long lastSendTime = 0;
unsigned long updateInterval = 20000; // 20 secondes
float temperatureThreshold = 30.0; // Seuil de température en degrés Celsius
int lightThreshold = 200; // Seuil de luminosité
int moistureThreshold = 300; // Seuil d'humidité du sol
int lowMoistureCount = 0;
int lowMoistureLimit = 5; // Nombre de lectures de faible humidité avant notification
unsigned long lastUpdateTime = 0;
unsigned long summaryUpdateInterval = 120000; // 2 minutes
unsigned long lastMoistureMessageTime = 0; // Temps du dernier message d'arrosage
unsigned long moistureMessageInterval = 3600000; // 1 heure en millisecondes

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  setupThingSpeak();
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
    if (temperature < 10) {
      message_to_whatsapp("Attention à votre plante. Il fait très froid !");
    } else if (temperature > 30) {
      message_to_whatsapp("Attention à votre plante. Température élevée !");
    }

    if (moisture < 300) {
      message_to_whatsapp("Attention à votre plante. Besoin d'un arrosage automatique !");
    } else if (moisture > 900) {
      // Vérifiez si assez de temps s'est écoulé depuis le dernier message d'arrosage
      if (currentTime - lastMoistureMessageTime >= moistureMessageInterval) {
        message_to_whatsapp("Arrosage automatique réalisé pour votre plante.");
        lastMoistureMessageTime = currentTime; // Mettre à jour le temps du dernier message d'arrosage
      }
    }

    if (light < 50) {
      message_to_whatsapp("Attention à votre plante. Placez-la au soleil !");
    } else if (light > 15000) {
      message_to_whatsapp("Attention à votre plante. Placez-la un peu plus à l'ombre !");
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
