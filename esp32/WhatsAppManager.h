#ifndef WHATSAPP_MANAGER_H
#define WHATSAPP_MANAGER_H

#include <Arduino.h>

// WhatsAppManager class to handle sending messages via WhatsApp
class WhatsAppManager {
public:
  void sendMessage(String message);

private:
  void postData(String url);
  String urlencode(String str);
  const String phone_number = "+33778174958"; // Ensure this number is correct and well-formatted
  const String apiKey = "9541441"; // Ensure this API Key is correct
};

#endif // WHATSAPP_MANAGER_H
