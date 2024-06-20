#include "EmailManager.h"
#include <WiFi.h>
#include <time.h>

// Replace with your own email and application password
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "siibourges@gmail.com"
#define AUTHOR_PASSWORD "nwgr wzrz qiwe nwxe" // Utilisez le mot de passe d'application généré ici
#define RECIPIENT_EMAIL "victor.dreyer02@gmail.com"

// Define the NTP server and time zone offset
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 7200 // GMT+2
#define DAYLIGHT_OFFSET_SEC 0 // No daylight saving time

SMTPSession smtp;
ESP_Mail_Session mailSession;

void EmailManager::setup() {
    smtp.debug(1);

    mailSession.server.host_name = SMTP_HOST;
    mailSession.server.port = SMTP_PORT;
    mailSession.login.email = AUTHOR_EMAIL;
    mailSession.login.password = AUTHOR_PASSWORD;

    // Configure time
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);

    // Wait for time to be set
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
}

void EmailManager::sendEmail(const String& subject, const String& message) {
    SMTP_Message msg;

    msg.sender.name = "ESP32";
    msg.sender.email = AUTHOR_EMAIL;
    msg.subject = subject;
    msg.addRecipient("Recipient", RECIPIENT_EMAIL);
    msg.text.content = message.c_str();

    if (!smtp.connect(&mailSession)) {
        Serial.println("Failed to connect to SMTP server.");
        return;
    }

    if (!MailClient.sendMail(&smtp, &msg)) {
        Serial.println("Error sending Email, " + smtp.errorReason());
    } else {
        Serial.println("Email sent successfully.");
    }

    smtp.closeSession();
}

void EmailManager::sendCriticalEventEmail(const String& eventDetails) {
    sendEmail("Critical Event Notification", eventDetails);
}

void EmailManager::sendDailySummaryEmail(const String& summary) {
    sendEmail("Daily Summary", summary);
}

void EmailManager::sendWeeklySummaryEmail(const String& summary) {
    sendEmail("Weekly Summary", summary);
}

void EmailManager::sendMonthlySummaryEmail(const String& summary) {
    sendEmail("Monthly Summary", summary);
}
