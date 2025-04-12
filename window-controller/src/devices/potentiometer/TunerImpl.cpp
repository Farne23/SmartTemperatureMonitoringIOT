#include "devices/potentiometer/TunerImpl.h"
#include <Arduino.h>

TunerImpl::TunerImpl(int pin) {
    this->pin = pin;
}

int TunerImpl::getValue() {
    return analogRead(this->pin);
}