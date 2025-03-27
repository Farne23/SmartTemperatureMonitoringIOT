#include "devices/actuators/Window.h"

Window::Window(ServoMotor* servo){
    this->servo = servo;
    this->servo->on();
    this->close();
}

void Window:: open(){
    this->servo->setPosition(OPEN_DEG);
}

void Window::close(){
    this->servo->setPosition(CLOSE_DEG);
}

/**
 * perc must be in the [0, 1] real interval.
 */
void Window::set(double perc) {
    if (perc == 1.0) {
        open();
    } else if (perc == 0.0) {
        close();
    } else {
        this->servo->setPosition((perc * OPEN_DEG));
    }
}