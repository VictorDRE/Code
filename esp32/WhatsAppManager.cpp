#include "WhatsAppManager.h"
#include <HTTPClient.h>

// Send a message via WhatsApp using CallMeBot service
void WhatsAppManager::sendMessage(String message) {
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);
  try {
    postData(url);
    Serial.println("WhatsApp message sent: " + message);
  } catch (const std::exception& e) {
    Serial.println("Error sending WhatsApp message: " + String(e.what()));
  }
}

// Post data to the URL created in sendMessage
void WhatsAppManager::postData(String url) {
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode == 200) {
    Serial.println("WhatsApp message sent successfully");
  } else {
    Serial.println("Error sending WhatsApp message, code: " + String(httpCode));
  }
  http.end();
}

// Encode the message to be URL-safe
String WhatsAppManager::urlencode(String str) { 
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
