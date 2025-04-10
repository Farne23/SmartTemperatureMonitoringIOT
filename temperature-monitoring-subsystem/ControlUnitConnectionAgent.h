#ifnedf CONTROLUNITINTERFACE
#define CONTROLUNITINTERFACE

#include "ControlUnitInteface.h"
#include "MQTTClientConnection.h"

class ControlUnitConnectionAgent : public ControlUnitInteface{
    public:
        ControlUnitInterface(parametri);
        void sendTemperature(double temperature) override;
        double getPeriod() override;
        void loop() override;
        boolean getConnectionStatus() override;
        void setTopics(char* temperature_topic, char* connection_topic, char* periods_topic);
    private:
        MQTTClientConnection* mqttClient;
        char* temperature_topic; 
    }

#endif