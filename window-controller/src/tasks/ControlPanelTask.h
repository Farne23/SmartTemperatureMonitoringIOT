#ifndef __CONTROLPANELTASK__
#define __CONTROLPANELTASK__

#include "controller/WindowController.h"
#include "tasks/Task.h"

class ControlPanelTask : public Task {
    private:
        WindowController *controller;

    public:
        ControlPanelTask(WindowController *controller);
        void tick(); 

};

#endif