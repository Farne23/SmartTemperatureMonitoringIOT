#ifndef __DISPLAY__
#define __DISPLAY__

#include <map>
#include "utility.h"
#include <string>
#include "LiquidCrystal_I2C.h"

class Display {
    public:
        virtual void displayAuto(double perc)= 0;
        virtual void displayMan(double perc, double temp)= 0;
};

#endif