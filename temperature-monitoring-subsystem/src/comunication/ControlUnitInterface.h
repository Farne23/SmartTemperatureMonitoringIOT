#ifndef CONTROLUNITINTERFACE
#define CONTROLUNITINTERFACE

class ControlUnitInterface
{
public:
    //Comunicates to the control unit a new sampled temperature.
    virtual void sendTemperature(double temperature) = 0;
    //Returns the last sampling period sent by the control unit.
    virtual int getPeriod() = 0;
    //Returns true if the control unit has sent a new sampling period.
    virtual bool newPeriodAvailable() = 0;
    //Ensures that the connection to the MQTT server is still on and handles asynchrouns operations.
    virtual void ensureConnected() = 0;
    //Returns true if the connection is still on
    virtual bool getConnectionStatus() = 0;
    //Sets the topic for MQTT messages.
    virtual void setTopics(const char *temperature_topic, const char *connection_topic, const char *periods_topic);

    virtual ~ControlUnitInterface() {}
};

#endif