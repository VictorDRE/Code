#ifndef EMAIL_MANAGER_H
#define EMAIL_MANAGER_H

#include <WiFi.h>
#include <ESP_Mail_Client.h>

class EmailManager {
public:
    void setup();
    void sendEmail(const String& subject, const String& message);
};

#endif // EMAIL_MANAGER_H
