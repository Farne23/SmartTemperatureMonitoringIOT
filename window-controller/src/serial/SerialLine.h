#ifndef __SERIALLINE__
#define __SERIALLINE

#include "MsgService.h"

class SerialLine {
    public:
        SerialLine();
        void sendData(double openPerc);
        void getData();
        char getChangeStatus();
        double getTemp();
        double getPerc();
    private:
        char changeMode;
        double temp;
        double perc;
};

#endif