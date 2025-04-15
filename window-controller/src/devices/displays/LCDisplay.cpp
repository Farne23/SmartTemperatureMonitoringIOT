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

void LCDisplay::printMsg(string line1, string line2) {
    /*
     * if both lines can be contained in the display
     */
    if (line1.size() <= COLS && line2.size() <= COLS) {
        this->lcd.clear();
        // first row, first column
        this->lcd.setCursor(ST, ST);
        this->lcd.print(line1.c_str());
        // second row, first column
        this->lcd.setCursor(ST, ND);
        this->lcd.print(line2.c_str());
    }  
}