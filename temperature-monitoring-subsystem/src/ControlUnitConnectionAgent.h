#ifndef CONTROLUNITCONNECTIONAGENT_H
#define CONTROLUNITCONNECTIONAGENT_H

#include "ControlUnitInterface.h"
#include "MQTTClientConnection.h"

class ControlUnitConnectionAgent : public ControlUnitInterface {
public:
    ControlUnitConnectionAgent(const char* ssid, const char* password, const char* server, const char* username, const char* mqtt_password);
    ~ControlUnitConnectionAgent();

    void sendTemperature(double temperature) override;
    int getPeriod() override;
    bool newPeriodAvailable() override;
    void loop() override;
    bool getConnectionStatus() override;
    void setTopics(char* temperature_topic, char* connection_topic, char* periods_topic);

private:
    MQTTClientConnection* mqttClient;
    const char* temperature_topic;
    const long gmtOffset_sec = 3600; 
    const int daylightOffset_sec = 3600;
};

#endif
