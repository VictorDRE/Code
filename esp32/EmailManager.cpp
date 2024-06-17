#include "EmailManager.h"

// Replace with your own email and application password
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "siibourges@gmail.com"
#define AUTHOR_PASSWORD "nwgr wzrz qiwe nwxe" // Utilisez le mot de passe d'application généré ici

#define RECIPIENT_EMAIL "victor.dreyer02@gmail.com"

SMTPSession smtp;
ESP_Mail_Session mailSession;

void EmailManager::setup() {
    smtp.debug(1);

    mailSession.server.host_name = SMTP_HOST;
    mailSession.server.port = SMTP_PORT;
    mailSession.login.email = AUTHOR_EMAIL;
    mailSession.login.password = AUTHOR_PASSWORD;
    mailSession.time.ntp_server = "pool.ntp.org";
    mailSession.time.gmt_offset = 3600;
    mailSession.time.day_light_offset = 3600;
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
    }

    smtp.closeSession();
}
