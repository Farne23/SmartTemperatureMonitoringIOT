#ifndef __SERIALLINE__
#define __SERIALLINE

#include "MsgService.h"

class SerialLine {
    public:
        SerialLine();
        void sendData(double openPerc);
        void getData();
        bool getChangeStatus();
        double getTemp();
        double getManPerc();
        double getAutoPerc();
    private:
        bool changeStatus;
        double temp;
        double manPerc;
        double autoPerc;
};

#endif