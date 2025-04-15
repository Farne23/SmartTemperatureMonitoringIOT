#include "devices\actuators\ServoMotorImpl.h"
#include <Arduino.h>

ServoMotorImpl::ServoMotorImpl(int pin){
  this->pin = pin;  
} 

void ServoMotorImpl::on(){
  motor.attach(pin);    
}

void ServoMotorImpl::setPosition(int angle){
  // angle should be in [750 - 2250]
  float coeff = (2250.0-750.0)/180;
  motor.write(750 + angle*coeff);      
}

void ServoMotorImpl::off(){
  motor.detach();    
}