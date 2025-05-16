#include "WindowController.h"
#include "devices/displays/DisplayImpl.h"
#include "settings/HwInterfaces.h"

WindowController::WindowController() {
    this->mode = AUTO;
    this->button = new SwitchButton(SWITCH_BUTTON_PIN);
    this->display = new DisplayImpl();
    this->tuner = new TunerImpl(POT_PIN);
    this->window = new Window(SERVO_MOTOR_PIN);
    this->line = new SerialLine();
    this->dashboardComm = false;
}

ControlMode WindowController::getMode() {
    return this->mode;
}

void WindowController::switchToMan() {
    this->mode = MANUAL;
}

void WindowController::switchToAuto() {
    this->mode = AUTO;
}

void WindowController::switchMode() {
    this->mode = this->mode == AUTO ? MANUAL : AUTO;
}

void WindowController::updateData() {
    this->line->getData();
}

double WindowController::getPerc() {
    return this->line->getPerc();
}

double WindowController::getTemp() {
    return this->line->getTemp();
}

char WindowController::getSwitch() {
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

void WindowController::displayMan(double perc, double temp) {
    this->display->displayMan(perc, temp);
}

void WindowController::displayAuto (double perc) {
    this->display->displayAuto(perc);
}