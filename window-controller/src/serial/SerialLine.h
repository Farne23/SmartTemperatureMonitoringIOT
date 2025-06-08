#ifndef __SERIALLINE__
#define __SERIALLINE

#include "MsgService.h"

class SerialLine {
    public:
        SerialLine();
        void sendData(String msg);
        void getData();
        char getChangeStatus();
        double getTemp();
        double getPerc();
        void clearPerc();
    private:
        char changeMode;
        double temp;
        double perc;
};

#endif