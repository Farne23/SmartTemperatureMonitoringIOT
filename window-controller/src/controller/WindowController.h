#ifndef __CONTROLLER__
#define __CONTROLLER__

#include "serial/SerialLine.h"
#include "devices/potentiometer/TunerImpl.h"
#include "devices/actuators/Window.h"
#include "devices/displays/Display.h"
#include "devices/buttons/ButtonImpl.h"

enum ControlMode {
    MANUAL,
    AUTO,
};

class WindowController {
    private:
        ControlMode mode;
        TunerImpl* tuner;
        Window* window;
        Display* display;
        ButtonImpl* button;
        SerialLine* line;
        
    public:
        WindowController(int btnPin, int tunePin, int winPin);
        ControlMode getMode();
        void switchMode();
        void updateData();
        double getAutoPerc();
        double getManPerc();
        double getTemp();
        bool getSwitch();
        void setPerc(double perc);
        
};

#endif