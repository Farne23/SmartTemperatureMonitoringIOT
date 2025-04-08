#include "TempSensor.h"
#include <Arduino.h>

#define TO_VOLT 4.88
#define TO_DEG 0.1

TempSensor::TempSensor(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}

double TempSensor::sense() {
    // value read by PWM signal
    int temp_adc_val;
    double temp_val;
    /* Read Temperature */
    temp_adc_val = analogRead(this->pin);
    /* Convert adc value to equivalent voltage */
    temp_val = (temp_adc_val * TO_VOLT);
    /* LM35 gives output of 10mv/°C */
    temp_val = (temp_val * TO_DEG);
    return temp_val;
}