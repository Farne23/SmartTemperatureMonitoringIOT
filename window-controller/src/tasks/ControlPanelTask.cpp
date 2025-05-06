#include "tasks/ControlPanelTask.h"

#define BUTTON_SYNC_INTERVAL 50

ControlPanelTask::ControlPanelTask(WindowController *controller) {
    this->controller = controller;
}

void ControlPanelTask::tick() {
    if (this->controller->switchReq()) {
        this->controller->switchMode();
        // Write mode switch on serial line
        Serial.println("S");
    } 
}