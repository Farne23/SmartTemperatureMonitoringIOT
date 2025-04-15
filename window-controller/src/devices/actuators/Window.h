#ifndef __WINDOW__
#define __WINDOW__

#define OPEN_DEG 90
#define CLOSE_DEG 0

#include "devices/actuators/ServoMotor.h"

class Window {
    public:
        Window(int pin);
        void set(double perc);
    private:
        ServoMotor* servo;
        void open();
        void close();
};

#endif