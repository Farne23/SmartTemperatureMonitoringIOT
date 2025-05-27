#ifndef __WINDOWREGTASK__
#define __WINDOWREGTASK__

#include "Task.h"
#include "controller/WindowController.h"

class WindowRegTask : public Task {
    private:
        WindowController *controller;
        bool dashComm;
    public:
        WindowRegTask(WindowController *controller);
        void tick();
};

#endif