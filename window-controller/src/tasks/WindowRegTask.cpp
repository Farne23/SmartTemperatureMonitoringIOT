#include "WindowRegTask.h"
#include "settings/DomainDefaults.h"

WindowRegTask::WindowRegTask(WindowController *controller) {
    this->controller = controller;
    this->dashComm = false;
}

void WindowRegTask::tick() {
    double perc, temp;
    // fetch data from serial line.
    this->controller->fetch();

    // eventually switch mode, if asked from dashboard.
    char switchRq = this->controller->getSwitch();
    if (switchRq == 'M') {
        this->controller->switchToMan();
    } else if (switchRq == 'A') {
        this->controller->switchToAuto();
    }
    // if a switch request has been sent from dashboard,
    // then dashboard communication is active.
    dashComm = (switchRq == 'M');

    ControlMode mode = this->controller->getMode();
    switch(mode) {
        case MANUAL:
            temp = this->controller->getTemp();
            // if no temperature is sent, then is zero.
            temp = (temp == DEF_TEMP) ? 0 : temp;
            // if dashboard communication is active,
            // then, expect opening level from serial line.
            if (dashComm)
            {
                perc = this->controller->getPerc();
                // if no percentage is sent, then it's zero
                perc = (perc == DEF_PERC) ? 0 : perc;
                // show all on the display.
                this->controller->displayMan(perc, temp);
                this->controller->setPerc(perc);
                return;
            }
            // this percentage should always be valid
            perc = this->controller->getTunerPerc();
            // show all on the display
            this->controller->displayMan(perc, temp);
            this->controller->setPerc(perc);
            // send informations on the serial line
            this->controller->sendData(String(perc));
            break;

        case AUTO:
            perc = this->controller->getPerc();
            // if no percentage is sent, then it's zero
            perc = (perc == DEF_PERC) ? 0 : perc;
            this->controller->displayAuto(perc);
            this->controller->setPerc(perc);
            break;
    }
}

    // this->controller->updateData();

    // if (this->controller->getMode() == AUTO) {
    //     if (this->controller->getSwitch() == 'M') {
    //         this->controller->switchToMan();
    //         return;
    //     }
    //     else {
    //         double perc = this->controller->getPerc();
    //         MsgService.sendMsg("Perc from backend: " + String(perc));
    //         // check if perc is undefined
    //         // if not, perc is 0
    //         perc = (!isnan(perc) && perc < 1.0 && perc >= 0.0) ? perc : 0.0;
    //         this->controller->setPerc(perc);
    //         this->controller->displayAuto(perc);
    //         // Write open percantege on serial line [1, 0]
    //         MsgService.sendMsg(String(perc));
    //     }
    // }
    // else if (this->controller->getMode() == MANUAL) {
    //     if (this->controller->getSwitch() == 'A') {
    //         this->controller->switchToAuto();
    //         return;
    //     }
    //     double temp = this->controller->getTemp();
    //     double perc = this->controller->getPerc();
    //     // if perc is a defined value
    //     if (perc != DEF_PERC) {
    //         this->controller->setDashboardOn();
    //     }
    //     if (!this->controller->getDashboardComm()) {
    //         perc = this->controller->getTunerPerc();
    //     }
    //     this->controller->setPerc(perc);
    //     this->controller->displayMan(perc, temp);
    //     // Write open percantege on serial line [1, 0]
    //     MsgService.sendMsg(String(perc));
    // }