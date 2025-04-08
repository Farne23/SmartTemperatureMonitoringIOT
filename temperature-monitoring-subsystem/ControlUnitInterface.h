#ifndef CONTROLUNITINTERFACE
#define CONTROLUNITINTERFACE

class ControlUnitInterface {
public:
    virtual void sendTemperature(double temperature) = 0;
    virtual double getPeriod() = 0;
    virtual void loop();
    virtual booleangetConnectionStatus() = 0;
    virtual ~ControlUnitInterface() {}
};

#endif