#ifndef LEDMANAGER_H
#define LEDMANAGER_H

class LEDManager {
public:
    void setNoInternet();
    void setNormalOperation();
    void setErrorState();
    void setCriticalError();
    void setWifiSearching(); 
    void setLightSleep();
};

#endif // LEDMANAGER_H
