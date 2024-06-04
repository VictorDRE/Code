#ifndef WHATSAPP_MANAGER_H
#define WHATSAPP_MANAGER_H

#include <Arduino.h>

class WhatsAppManager {
public:
  void sendMessage(String message);

private:
  void postData(String url);
  String urlencode(String str);
  const String phone_number = "+33778174958";
  const String apiKey = "9541441";
};

#endif // WHATSAPP_MANAGER_H
