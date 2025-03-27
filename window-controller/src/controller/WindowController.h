#ifndef __CONTROLLER__
#define __CONTROLLER__

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
        // an object representing the serial line is needed
        
    public:
        WindowController();
        
};

#endif