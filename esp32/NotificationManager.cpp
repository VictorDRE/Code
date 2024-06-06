#include "NotificationManager.h"

NotificationManager::NotificationManager(WhatsAppManager* whatsappManager) : whatsappManager(whatsappManager) {}

void NotificationManager::sendNotification(const String& message) {
  whatsappManager->sendMessage(message);
}
