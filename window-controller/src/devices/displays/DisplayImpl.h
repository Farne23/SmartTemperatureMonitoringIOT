#ifndef __DISPLAYIMPL__
#define __DISPLAYIMPL__

#include "Display.h"
#include "LCDisplay.h"

class DisplayImpl : public Display{
    private:
        LCDisplay* lcd;
        virtual void init();
    public:
        DisplayImpl();
        virtual void displayReadyToOpen();//Press open to enter waste
        virtual void displayReadyToClose();  //Press close when done
        virtual void displayReceived(); //Waste received
        virtual void displayFull(); //Container full
        virtual void displayProblem(); //Problem detected
};

#endif