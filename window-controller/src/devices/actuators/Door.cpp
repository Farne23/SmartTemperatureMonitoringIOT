#include "devices/actuators/Door.h"

Door::Door(ServoMotor* servo){
    this->servo = servo;
    this->servo->on();
    this->close();
}

void Door:: open(){
    this->servo->setPosition(OPEN_DEG);
}

void Door::close(){
    this->servo->setPosition(CLOSE_DEG);
}

/**
 * perc must be in the [0, 1] real interval.
 */
void Door::set(double perc) {
    this->servo->setPosition((perc * OPEN_DEG));
}