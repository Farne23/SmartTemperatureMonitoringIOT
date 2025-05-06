#include "WindowRegTask.h"

WindowRegTask::WindowRegTask(WindowController *controller) {
    this->controller = controller;
}

void WindowRegTask::tick() {
    this->controller->updateData();
    if (this->controller->getMode() == AUTO) {
        if (this->controller->getSwitch()) {
            this->controller->switchMode();
            // Write switch mode code on serial line
            Serial.println("S");
        }
        else {
            this->controller->setPerc(this->controller->getAutoPerc());
        }
        this->controller->displayAuto(this->controller->getAutoPerc());
        // Write open percantege on serial line [1, 0]
        Serial.println(this->controller->getAutoPerc());
    }
    else if (this->controller->getMode() == MANUAL) {
        double temp = this->controller->getTemp();
        double perc = this->controller->getManPerc();


        // if perc is a defined value
        if (!isnan(perc)) {
            this->controller->setDashboardOn();
        }

        if (!this->controller->getDashboardComm()) {
            perc = this->controller->getTunerPerc();
        }
        this->controller->setPerc(perc);
        this->controller->displayMan(perc, temp);
        // Write open percantege on serial line [1, 0]
        Serial.println(perc);
    }
}