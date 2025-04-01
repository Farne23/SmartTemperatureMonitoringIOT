#include "WindowController.h"
#include "devices/displays/DisplayImpl.h"

WindowController::WindowController(int btnPin, int tunePin, int winPin) {
    this->mode = AUTO;
    this->button = new SwitchButton(btnPin);
    this->display = new DisplayImpl();
    this->tuner = new TunerImpl(tunePin);
    this->window = new Window(winPin);
    this->line = new SerialLine();
    this->dashboardComm = false;
}

ControlMode WindowController::getMode() {
    return this->mode;
}

void WindowController::switchMode() {
    this->mode = this->mode == AUTO ? MANUAL : AUTO;
    if (mode == AUTO) {
        this->dashboardComm = false;
    }
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

bool WindowController::switchReq() {
    return this->button->switchRequested();
}

double WindowController::getTunerPerc() {
    double value = this->tuner->getValue();
    // convertion from value [0, 1023] to [0, 1]
    value /= 1023;
    return value;
}

bool WindowController::getDashboardComm() {
    return this->dashboardComm;
}

void WindowController::setDashboardOn() {
    this->dashboardComm = true;
}