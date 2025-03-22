#ifndef __DOOR__
#define __DOOR__

#define OPEN_DEG 90
#define CLOSE_DEG 0

#include "devices/actuators/ServoMotor.h"

class Door {
    public:
        Door(ServoMotor* servo);
        void open();
        void close();
        void set(double perc);
    private:
        ServoMotor* servo;
};

#endif