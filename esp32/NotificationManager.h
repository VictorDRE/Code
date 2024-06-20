#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "EmailManager.h"

class NotificationManager {
public:
    NotificationManager(EmailManager* emailManager);
    void sendNotification(const String& message);

private:
    EmailManager* emailManager;
};

#endif // NOTIFICATIONMANAGER_H
