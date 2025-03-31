#include "WindowController.h"
#include "devices/displays/DisplayImpl.h"

WindowController::WindowController(int btnPin, int tunePin, int winPin) {
    this->mode = AUTO;
    this->button = new ButtonImpl(btnPin);
    this->display = new DisplayImpl();
    this->tuner = new TunerImpl(tunePin);
    this->window = new Window(winPin);
    this->line = new SerialLine();
}

ControlMode WindowController::getMode() {
    return this->mode;
}
void WindowController::switchMode() {
    this->mode = this->mode == AUTO ? MANUAL : AUTO;
}
void WindowController::updateData() {
    this->line->getData();
}

double WindowController::getAutoPerc() {
    return this->line->getAutoPerc();
}

double WindowController::getManPerc() {
    return this->line->getManPerc();
}

double WindowController::getTemp() {
    return this->line->getTemp();
}

bool WindowController::getSwitch() {
    return this->line->getChangeStatus();
}

void WindowController::setPerc(double perc) {
    this->window->set(perc);
}