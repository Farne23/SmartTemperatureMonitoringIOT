#include "ControlUnitConnectionAgent.h"
ControlUnitConnectionAgent::ControlUnitConnectionAgent() {
    // inizializza tutto qui
}

void ControlUnitConnectionAgent::begin() {
    // implementazione WiFi/MQTT setup
}

void ControlUnitConnectionAgent::loop() {
    // chiamata client.loop() ecc.
}

void ControlUnitConnectionAgent::publishMessage(const char* topic, const char* message) {
    // chiamata client.publish()
}