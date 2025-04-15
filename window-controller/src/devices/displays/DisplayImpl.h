#ifndef __DISPLAYIMPL__
#define __DISPLAYIMPL__

#include "Display.h"
#include "LCDisplay.h"

class DisplayImpl : public Display{
    private:
        LCDisplay* lcd;
    public:
        DisplayImpl();
        void displayAuto (double perc);
        void displayMan(double perc, double temp);
};

#endif