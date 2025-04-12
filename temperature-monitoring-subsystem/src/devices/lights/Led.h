#ifndef __LED__
#define __LED__

#include "Light.h"

/* Interface rappresenting a simple led, manageable trough a pin,
 implementation of the Ligth interface*/
class Led: public Light { 
public:
  Led();
  Led(int pin);
  void switchOn();
  void switchOff();    
private:
  int pin;  
};

#endif