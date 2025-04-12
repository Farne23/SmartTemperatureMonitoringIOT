#ifndef __SWITCHBUTTON__
#define __SWITCHBUTTON__

#include "devices/buttons/ButtonImpl.h"

class SwitchButton {
    private:
        ButtonImpl *btn;

    public:
        SwitchButton(int pin);
        bool switchRequested();
};

#endif