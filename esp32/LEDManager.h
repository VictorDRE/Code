#ifndef LEDMANAGER_H
#define LEDMANAGER_H

class LEDManager {
public:
    void setNoInternet();
    void setNormalOperation();
    void setErrorState();
    void setCriticalError();
    void setWifiSearching(); 
};

#endif // LEDMANAGER_H
