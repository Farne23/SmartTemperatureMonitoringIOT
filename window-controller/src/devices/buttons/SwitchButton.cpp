#include <Arduino.h>
#include "devices/buttons/SwitchButton.h"

#define BUTTON_SYNC_INTERVAL 50

SwitchButton::SwitchButton(int pin)
{
    this->btn = new ButtonImpl(pin);
}

bool SwitchButton::switchRequested()
{
    long time = millis();
    if(this->btn->getLastSyncTime() + BUTTON_SYNC_INTERVAL < time){
        this->btn->sync();
    }
    return this->btn->isPressed();
}