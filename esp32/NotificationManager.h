#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include "EmailManager.h"

class NotificationManager {
public:
    NotificationManager(EmailManager* emailManager);
    void sendNotification(const String& message);

private:
    EmailManager* emailManager;
};

#endif // NOTIFICATION_MANAGER_H
