#include "ButtonPanel.h"
#include "ButtonImpl.h"
#include "Arduino.h"

#define BUTTON_SYNC_INTERVAL 50

ButtonPanel::ButtonPanel(Button* openButton, Button* closeButton){
    this->openButton = openButton;
    this->closeButton = closeButton;
}

bool ButtonPanel::openRequested(){
    return actionRequested(this->openButton);
}

bool ButtonPanel::closeRequested(){
    return actionRequested(this->closeButton);
}

bool ButtonPanel::actionRequested(Button* button){
    long time = millis();
    if(button->getLastSyncTime() + BUTTON_SYNC_INTERVAL < time){
        button->sync();
    }
    return button->isPressed();
}