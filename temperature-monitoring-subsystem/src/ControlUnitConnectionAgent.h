#ifndef CONTROLUNITCONNECTIONAGENT_H
#define CONTROLUNITCONNECTIONAGENT_H

#include "ControlUnitInterface.h"
#include "MQTTClientConnection.h"

class ControlUnitConnectionAgent : public ControlUnitInterface {
public:
    ControlUnitConnectionAgent(const char* ssid, const char* password, const char* server, const char* username, const char* mqtt_password, const int port);
    ~ControlUnitConnectionAgent();

    void sendTemperature(double temperature) override;
    int getPeriod() override;
    bool newPeriodAvailable() override;
    void ensureConnected() override;
    bool getConnectionStatus() override;
    void setTopics(const char* temperature_topic,const char* connection_topic,const char* periods_topic) override;

private:
    MQTTClientConnection* mqttClient;
    const char* temperature_topic;
    const long gmtOffset_sec = 3600; 
    const int daylightOffset_sec = 3600;
};

#endif
