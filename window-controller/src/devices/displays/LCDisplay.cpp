#include "LCDisplay.h"
#include <Arduino.h>

#define ADDR 0x27
#define COLS 16
#define ROWS 2
#define ST 0
#define ND 1

LCDisplay::LCDisplay() 
    : lcd(ADDR, COLS, ROWS) // lcd member in the init list
{
    this->lcd.init();
    this->lcd.setBacklight(HIGH);
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
        /* ALTERNATIVE CLEAR
        clear() needs a longer "cooldown" to be called,
        50ms was too short but 200ms seems to work fine.
        If 50ms is the choice a clear variant could also be
        like this:

        #define BLANK "                "
        ...
        this->lcd.setCursor(ST, ST);
        this->lcd.print(BLANK);
        this->lcd.setCursor(ST, ND);
        this->lcd.print(BLANK);
        */

    }  
}