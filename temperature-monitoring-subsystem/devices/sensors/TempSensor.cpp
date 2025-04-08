#include "TempSensor.h"
#include <Arduino.h>

/* A temperature sensor implementation working for ESP32 and it's VMax of 3,33V */
#define TO_C (3.33 / 4095.0 * 100.0)    // ≈ 0.0813 °C per step

TempSensor::TempSensor(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}

double TempSensor::sense() {
    int temp_adc_val = analogRead(this->pin);
    double temp_val = temp_adc_val * TO_C;
    return temp_val;
}