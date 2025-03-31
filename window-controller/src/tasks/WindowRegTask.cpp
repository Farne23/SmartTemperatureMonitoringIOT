#include "WindowRegTask.h"

WindowRegTask::WindowRegTask(WindowController *controller) {
    this->controller = controller;
}

void WindowRegTask::tick() {
    this->controller->updateData();
    if (this->controller->getMode() == AUTO) {
        if (this->controller->getSwitch()) {
            this->controller->switchMode();
        }
        else {
            this->controller->setAngle(this->controller->getAutoAngle());
        }
    }
    else if (this->controller->getMode() == MANUAL) {
        double temp = this->controller->getTemp();
        double perc = this->controller->getManAngle();

        // if perc is not null i guess(?)
        if (perc) {
            this->controller->setAngle(perc);
        } else {
            // get perc from tuner and set it.
        }
    }
}