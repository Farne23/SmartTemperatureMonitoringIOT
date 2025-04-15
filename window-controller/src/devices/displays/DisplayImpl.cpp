#include "devices/displays/DisplayImpl.h"

#define AUTO_LINE1 "XXX% AUTO"
#define AUTO_LINE2 ""
#define MAN_LINE1 "XXX% MAN"
#define MAN_LINE2 "XXX °C"

DisplayImpl::DisplayImpl(){
    this->lcd = new LCDisplay();
}

void DisplayImpl::displayAuto(double perc){
    string line1 = AUTO_LINE1;
    line1.replace(0, 3, String(perc * 100, 0).c_str());
    this->lcd->printMsg(line1, AUTO_LINE2);
}

void DisplayImpl::displayMan(double perc, double temp){
    string line1 = MAN_LINE1;
    string line2 = MAN_LINE2;
    line1.replace(0, 3, String(perc * 100, 0).c_str());
    line2.replace(0, 3, String(temp, 2).c_str());
    this->lcd->printMsg(line1, line2);
}