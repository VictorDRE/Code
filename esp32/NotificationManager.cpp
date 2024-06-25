#include "NotificationManager.h"

NotificationManager::NotificationManager(EmailManager* emailManager) : emailManager(emailManager) {}

// Comment or remove this function if it's no longer needed
void NotificationManager::sendNotification(const String& message) {
    // emailManager->sendEmail("Alerte Arrosage Automatique", message);
}
