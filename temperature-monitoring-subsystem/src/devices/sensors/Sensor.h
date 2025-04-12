#ifndef __SENSOR__
#define __SENSOR__

class Sensor {
    public:
        /*
         * detect a certain quantity
         * of a phenomenon
         */
        virtual double sense() = 0;
};

#endif