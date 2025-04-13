#ifndef CONTROLUNITINTERFACE
#define CONTROLUNITINTERFACE

class ControlUnitInterface {
public:
    virtual void sendTemperature(double temperature) = 0;
    virtual int getPeriod() = 0;
    virtual bool newPeriodAvailable() = 0;
    virtual void ensureConnected() = 0;
    virtual bool getConnectionStatus() = 0;
    virtual void setTopics(const char* temperature_topic,const char* connection_topic,const char* periods_topic);

    virtual ~ControlUnitInterface() {} 
};

#endif