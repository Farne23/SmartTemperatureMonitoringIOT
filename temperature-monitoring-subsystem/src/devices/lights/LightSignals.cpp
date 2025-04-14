#include "LightSignals.h"
#include "Led.h"

/*Implementation of LigthSignal interface */

LightSignals::LightSignals(Led *greenLed, Led *redLed)
{
    this->greenLed = greenLed;
    this->redLed = redLed;
}

void LightSignals::signalWorking()
{
    greenLed->switchOn();
    redLed->switchOff();
}

void LightSignals::signalProblems()
{
    greenLed->switchOff();
    redLed->switchOn();
}