#include "tasks/ControlPanelTask.h"

ControlPanelTask::ControlPanelTask(WindowController *controller) {
    this->controller = controller;
}

void ControlPanelTask::tick() {
    if (this->controller->switchReq()) {
        this->controller->switchMode();
        // Write mode switch on serial line
        this->controller->sendData("S");
    } 
}