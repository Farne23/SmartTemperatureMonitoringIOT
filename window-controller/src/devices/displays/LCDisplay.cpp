#include "LCDisplay.h"

#define ADDR 0x27
#define COLS 16
#define ROWS 2
#define ST 0
#define ND 1

LCDisplay::LCDisplay() 
    : lcd(ADDR, COLS, ROWS) // lcd member in the init list
{
    this->lcd.init();
    this->lcd.backlight();
}

void LCDisplay::printMsg(string msgName) {
    /*
     * lines is an entry (key, value) structured as a pair
     * lines.first -> key
     * lines.second -> value
     */
    auto lines = this->msgs.find(msgName);
    if (lines != msgs.end()) {
        this->lcd.clear();
        // first row, first column
        this->lcd.setCursor(ST, ST);
        this->lcd.print(lines->second.first.c_str());
        // second row, first column
        this->lcd.setCursor(ST, ND);
        this->lcd.print(lines->second.second.c_str());
    }    
}

bool LCDisplay::addMsg(string name, string line1, string line2) {
    /*
     * if both lines can be contained in the display and
     * name doesn't already exist
     */
    if (this->msgs.find(name) == this->msgs.end() &&
        line1.size() <= COLS && line2.size() <= COLS) {
        this->msgs.insert({name, {line1, line2}});
        return true;
    }
    return false;
}