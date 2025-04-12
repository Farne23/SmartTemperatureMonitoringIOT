#ifndef __LIGHTSIGNALS__
#define __LIGHTSIGNALS___

#include "Led.h"

/*Interface for the management of the ligth signals given to the users.
The instructions given to this component trough its methods are then 
converted as switch on or switch off signals for the leds */

class LightSignals {
public:
    LightSignals(Led* greenLed, Led* redLed);
    void signalWorking();
    void signalProblems(); 
private:
    Led* greenLed;
    Led* redLed;
};

#endif
