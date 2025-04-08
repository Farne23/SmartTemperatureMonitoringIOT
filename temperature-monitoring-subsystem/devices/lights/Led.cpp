#include "Led.h"
#include "Arduino.h"

#define DEFAULT_PIN 3

/*Basic implementation of a class rappresenting a Led 
wich can be managed through a specified pin*/

Led::Led(){
  this->pin = DEFAULT_PIN;
  pinMode(pin,OUTPUT);
}

Led::Led(int pin){
  this->pin = pin;
  pinMode(pin,OUTPUT);
}

void Led::switchOn(){
  digitalWrite(pin,HIGH);
}

void Led::switchOff(){
  digitalWrite(pin,LOW);
};
