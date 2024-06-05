#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include "WhatsAppManager.h"

// NotificationManager class to handle notifications
class NotificationManager {
public:
  NotificationManager(WhatsAppManager* whatsappManager);
  void sendNotification(const String& message);

private:
  WhatsAppManager* whatsappManager;
};

#endif // NOTIFICATION_MANAGER_H
