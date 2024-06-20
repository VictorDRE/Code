#ifndef EMAILMANAGER_H
#define EMAILMANAGER_H

#include <Arduino.h>
#include <ESP_Mail_Client.h>

class EmailManager {
public:
    static void setup();
    static void sendCriticalEventEmail(const String& eventDetails);
    static void sendDailySummaryEmail(const String& summary);
    static void sendWeeklySummaryEmail(const String& summary);
    static void sendMonthlySummaryEmail(const String& summary);
    static void sendEmail(const String& subject, const String& message); 

private:
    // No need for private section anymore
};

#endif // EMAILMANAGER_H
