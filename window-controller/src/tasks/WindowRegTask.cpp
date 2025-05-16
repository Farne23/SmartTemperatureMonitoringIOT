#include "WindowRegTask.h"

WindowRegTask::WindowRegTask(WindowController *controller) {
    this->controller = controller;
}

void WindowRegTask::tick() {
    this->controller->updateData();
    if (this->controller->getMode() == AUTO) {
        if (this->controller->getSwitch() == 'M') {
            this->controller->switchToMan();
        } else if (this->controller->getSwitch() == 'A') {
            this->controller->switchToAuto();
        }
        else {
            this->controller->setPerc(this->controller->getPerc());
        }
        this->controller->displayAuto(this->controller->getPerc());
        // Write open percantege on serial line [1, 0]
        MsgService.sendMsg(String(this->controller->getPerc()));
    }
    if (this->controller->getMode() == MANUAL) {
        double temp = this->controller->getTemp();
        double perc = this->controller->getPerc();
        // if perc is a defined value
        if (!isnan(perc)) {
            this->controller->setDashboardOn();
        }

        if (!this->controller->getDashboardComm()) {
            perc = this->controller->getTunerPerc();
            Serial.println("From tuner: " + String(perc));
        }
        this->controller->setPerc(perc);
        this->controller->displayMan(perc, temp);
        // Write open percantege on serial line [1, 0]
        MsgService.sendMsg(String(perc));
    }
}