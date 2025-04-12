#ifndef CONTROLUNITINTERFACE
#define CONTROLUNITINTERFACE

class ControlUnitInterface {
public:
    virtual void sendTemperature(double temperature) = 0;
    virtual int getPeriod() = 0;
    virtual bool newPeriodAvailable() = 0;
    virtual void loop() = 0;
    virtual bool getConnectionStatus() = 0;

     
    virtual ~ControlUnitInterface() {} 
};

#endif