#ifnedf CONTROLUNITINTERFACE
#define CONTROLUNITINTERFACE

#include "ControlUnitInteface.h"
#include "MQTTClientConnection.h"

class ControlUnitConnectionAgent : public ControlUnitInteface{
    public:
        ControlUnitInterface(parametri);
        void sendTemperature(double temperature) override;
        int getPeriod() override;
        bool newPeriodAvailable() override;
        void loop() override;
        bool getConnectionStatus() override;
        void setTopics(char* temperature_topic, char* connection_topic, char* periods_topic);
    private:
        MQTTClientConnection* mqttClient;
        char* temperature_topic; 
        const long  gmtOffset_sec = 3600;     // adjust for your timezone
        const int   daylightOffset_sec = 3600; // summer time offset
    }

#endif