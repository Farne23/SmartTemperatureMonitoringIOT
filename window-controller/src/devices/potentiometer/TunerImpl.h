#ifndef __TUNER__
#define __TUNER__

class TunerImpl {
    public:
        TunerImpl(int pin);
        int getValue();
    
    private:
        int pin;
};

#endif