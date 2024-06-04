#include "whatsapp_utils.h"
#include "url_utils.h"
#include <HTTPClient.h>
#include "config.h"

void message_to_whatsapp(String message) {
  // Construire l'URL avec les paramètres nécessaires
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);
  
  // Appeler la fonction postData pour envoyer le message
  postData(url);
}

void postData(String url) {
  // Initialiser un client HTTP
  HTTPClient http;
  
  // Commencer une connexion HTTP
  http.begin(url);
  
  // Envoyer la requête GET et obtenir le code de réponse
  int httpCode = http.GET();
  
  // Vérifier le code de réponse
  if (httpCode == 200) {
    Serial.println("WhatsApp message sent successfully");
  } else {
    Serial.println("Error sending WhatsApp message");
  }
  
  // Libérer les ressources du client HTTP
  http.end();
}
