#ifndef WHATSAPP_MANAGER_H
#define WHATSAPP_MANAGER_H

#include <Arduino.h>

class WhatsAppManager {
public:
  void sendMessage(String message);

private:
  void postData(String url);
  String urlencode(String str);
  const String phone_number = "+33778174958"; // Assurez-vous que ce numéro est correct et bien formaté
  const String apiKey = "9541441"; // Assurez-vous que cette API Key est correcte
};

#endif // WHATSAPP_MANAGER_H
