#ifndef __DISPLAY__
#define __DISPLAY__

#include <map>
#include "utility.h"
#include <string>
#include "LiquidCrystal_I2C.h"
using namespace std;

class Display {
    public:
        virtual void displayReadyToOpen()= 0;//Press open to enter waste
        virtual void displayReadyToClose()= 0;  //Press close when done
        virtual void displayReceived()= 0; //Waste received
        virtual void displayFull()= 0; //Container full
        virtual void displayProblem()= 0; //Problem detected
};

#endif