#include "WindowRegTask.h"
#include "settings/DomainDefaults.h"

WindowRegTask::WindowRegTask(WindowController *controller) {
    this->controller = controller;
    // tuner perc should never be invalid.
    this->lastPotValue = this->controller->getTunerPerc();
    this->lastPerc = this->lastPotValue;
}

void WindowRegTask::tick() {
    double perc, temp, dashboardPerc, tunerPerc;
    // fetch data from serial line.
    this->controller->fetch();

    // eventually switch mode, if asked from dashboard.
    char switchRq = this->controller->getSwitch();
    if (switchRq == 'M') {
        this->controller->clearPerc();
        this->controller->switchToMan();
    } else if (switchRq == 'A') {
        this->controller->switchToAuto();
    }

    ControlMode mode = this->controller->getMode();
    switch(mode) {
        case MANUAL:
            // get temperature from serial line.
            temp = this->controller->getTemp();
            // if no temperature is sent, then is zero.
            temp = (temp == DEF_TEMP) ? 0 : temp;
            // set a backup value of perc, in case nothing is sent (last percentage used).
            perc = this->lastPerc;
            // try to read percentages from dashboard and also from potentiometer.
            tunerPerc = this->controller->getTunerPerc();
            dashboardPerc = this->controller->getPerc();
            // if tuner perc significally changed from the last value,
            // then save it.
            if (fabs(this->lastPotValue - tunerPerc) >= GAP)
            {
                perc = tunerPerc;
            }
            // update last potentiemeter value.
            this->lastPotValue = tunerPerc;
            // if the percentage from dahsboard is valid, then save it
            if (dashboardPerc != DEF_PERC)
            {
                perc = dashboardPerc;
                // clean up
                this->controller->clearPerc();
            }
            // show all on the display
            this->controller->displayMan(perc, temp);
            this->controller->setPerc(perc);
            // update last percentage
            this->lastPerc = perc;
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