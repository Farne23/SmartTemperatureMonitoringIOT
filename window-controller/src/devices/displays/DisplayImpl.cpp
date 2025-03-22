#include "DisplayImpl.h"
#include "settings/DisplayMessages.h"

DisplayImpl :: DisplayImpl(){
    this->lcd = new LCDisplay();
    this->init();
}

void DisplayImpl :: init(){
    this->lcd->addMsg("readyToOpen",READYTOOPEN_LINE1,READYTOOPEN_LINE2);
    this->lcd->addMsg("readyToClose",READYTOCLOSE_LINE1,READYTOCLOSE_LINE2);
    this->lcd->addMsg("received",RECEIVED_LINE1,RECEIVED_LINE2);
    this->lcd->addMsg("full",FULL_LINE1,FULL_LINE2);
    this->lcd->addMsg("problem",PROBLEM_LINE1,PROBLEM_LINE2);
}

void DisplayImpl ::displayReadyToOpen(){
    this->lcd->printMsg("readyToOpen");
}
void DisplayImpl ::displayReadyToClose(){
    this->lcd->printMsg("readyToClose");
}
void DisplayImpl ::displayReceived(){
    this->lcd->printMsg("received");
}
void DisplayImpl ::displayFull(){
    this->lcd->printMsg("full");
}
void DisplayImpl ::displayProblem(){
    this->lcd->printMsg("problem");
}