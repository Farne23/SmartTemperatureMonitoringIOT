#ifndef __TEMPSENSOR__
#define __TEMPSENSOR__

#include "Sensor.h"

class TempSensor: public Sensor {
    public:
        TempSensor(int pin);
        double sense();
    private:
        int pin;
};

#endif