#ifndef __CONTROLLER__
#define __CONTROLLER__

#include "serial/SerialLine.h"
#include "devices/potentiometer/TunerImpl.h"
#include "devices/actuators/Window.h"
#include "devices/displays/Display.h"
#include "devices/buttons/SwitchButton.h"

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
        SwitchButton* button;
        SerialLine* line;
        
    public:
        WindowController();
        ControlMode getMode();
        void switchToMan();
        void switchToAuto();
        void switchMode();
        void fetch();
        double getPerc();
        double getTemp();
        char getSwitch();
        void setPerc(double perc);
        bool switchReq();
        double getTunerPerc();
        void sendData(String msg);
        void displayMan(double perc, double temp);
        void displayAuto (double perc);
};

#endif