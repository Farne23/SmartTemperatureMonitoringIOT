#ifndef __LCDISPLAY__
#define __LCDISPLAY__

#include "string"
#include "LiquidCrystal_I2C.h"
using namespace std;

class LCDisplay {
    public:
        LCDisplay();
        void printMsg(string line1, string line2);
    private:
        LiquidCrystal_I2C lcd;
};

#endif