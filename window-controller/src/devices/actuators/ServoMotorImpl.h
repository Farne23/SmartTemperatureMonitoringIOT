#ifndef __SERVOMOTORIMPL__
#define __SERVOMOTORIMPL__

#include "devices/actuators/ServoMotor.h"
#include "timer/ServoTimer2.h"

class ServoMotorImpl: public ServoMotor {

public:
  ServoMotorImpl(int pin);
  void on();
  void setPosition(int angle);
  void off();
    
private:
  int pin; 
  ServoTimer2 motor; 
};

#endif
