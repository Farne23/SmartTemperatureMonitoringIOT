#ifndef __WINDOWREGTASK__
#define __WINDOWREGTASK__

#include "Task.h"
#include "controller/WindowController.h"

#define GAP 0.03

class WindowRegTask : public Task {
    private:
        WindowController *controller;
        double lastPotValue;
        double lastPerc;
    public:
        WindowRegTask(WindowController *controller);
        void tick();
};

#endif