#ifndef __WINDOWREGTASK__
#define __WINDOWREGTASK__

#include "Task.h"
#include "controller/WindowController.h"

class WindowRegTask : public Task {
    private:
        WindowController *controller;
        SerialLine *line;
    
    public:
        WindowRegTask(WindowController *controller);
        void init();
        void tick();
};

#endif