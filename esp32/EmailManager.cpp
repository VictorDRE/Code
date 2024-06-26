#include "EmailManager.h"
#include <WiFi.h>
#include <time.h>

// Replace with your own email and application password
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "siibourges@gmail.com"
#define AUTHOR_PASSWORD "nwgr wzrz qiwe nwxe" // Use the generated application password here
#define RECIPIENT_EMAIL "ethan.lefreteur@sii.fr"

// Define the NTP server and time zone offset
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 7200 // GMT+2
#define DAYLIGHT_OFFSET_SEC 0 // No daylight saving time

SMTPSession smtp;
ESP_Mail_Session mailSession;

// Set up email and time configurations
void EmailManager::setup() {
    smtp.debug(1); // Enable debug

    // Set mail server and login credentials
    mailSession.server.host_name = SMTP_HOST;
    mailSession.server.port = SMTP_PORT;
    mailSession.login.email = AUTHOR_EMAIL;
    mailSession.login.password = AUTHOR_PASSWORD;

    // Configure time
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);

    // Wait for time to be set
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time"); // Log if time setup fails
        return;
    }
}

// Function to send email with given subject and message
void EmailManager::sendEmail(const String& subject, const String& message) {
    SMTP_Message msg;

    // Set email sender and recipient
    msg.sender.name = "ESP32";
    msg.sender.email = AUTHOR_EMAIL;
    msg.subject = subject;
    msg.addRecipient("Recipient", RECIPIENT_EMAIL);
    msg.text.content = message.c_str();

    // Connect to SMTP server
    if (!smtp.connect(&mailSession)) {
        Serial.println("Failed to connect to SMTP server."); // Log if connection fails
        return;
    }

    // Send email and check for errors
    if (!MailClient.sendMail(&smtp, &msg)) {
        Serial.println("Error sending Email, " + smtp.errorReason()); // Log if sending fails
    } else {
        Serial.println("Email sent successfully."); // Log if sending succeeds
    }

    smtp.closeSession(); // Close SMTP session
}

// Functions to send specific types of emails
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