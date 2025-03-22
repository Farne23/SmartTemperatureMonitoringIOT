#ifndef __LCDISPLAY__
#define __LCDISPLAY__

#include <map>
#include "utility.h"
#include <string>
#include "LiquidCrystal_I2C.h"
using namespace std;

class LCDisplay {
    public:
        LCDisplay();
        void printMsg(string msgName);
        bool addMsg(string name, string line1, string line2);
    private:
        std::map<string, pair<string, string>> msgs;
        LiquidCrystal_I2C lcd;
};

#endif