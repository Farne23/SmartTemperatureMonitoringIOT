#ifnedf CONTROLUNITINTERFACE
#define CONTROLUNITINTERFACE

#include "ControlUnitInteface.h"

class ControlUnitConnectionAgent : public ControlUnitInteface{
    ControlUnitInterface(parametri);
    void sendTemperature(double temperature) override;
    double getPeriod() override;
    void loop() override;
    boolean getConnectionStatus() override;
}

#endif