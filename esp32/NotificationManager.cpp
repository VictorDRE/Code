#include "NotificationManager.h"

NotificationManager::NotificationManager(EmailManager* emailManager) : emailManager(emailManager) {}

void NotificationManager::sendNotification(const String& message) {
    emailManager->sendEmail("Alerte Arrosage Automatique", message);
}
