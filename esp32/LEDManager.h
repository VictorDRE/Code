#ifndef LEDMANAGER_H
#define LEDMANAGER_H

class LEDManager {
public:
    void setup();
    void setNoInternet();
    void setNormalOperation();
    void setErrorState();
    void setCriticalError();
    void setWifiSearching(); 
    void setLightSleep();
private:
    const int greenLEDPin = 12;
    const int redLEDPin = 13;
};

#endif // LEDMANAGER_H
