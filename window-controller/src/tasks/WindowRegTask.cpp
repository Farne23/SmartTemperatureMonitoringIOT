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
            this->controller->setPerc(this->controller->getAutoPerc());
        }
    }
    else if (this->controller->getMode() == MANUAL) {
        double temp = this->controller->getTemp();
        double perc = this->controller->getManPerc();

        // if perc is a defined value
        if (!isnan(perc)) {
            this->controller->setDashboardOn();
        }

        if (this->controller->getDashboardComm()) {
            this->controller->setPerc(perc);
        } else {
            this->controller->setPerc(this->controller->getTunerPerc());
        }
    }
}