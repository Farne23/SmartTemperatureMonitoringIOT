#include "ControlUnitConnectionAgent.h"
ControlUnitConnectionAgent::ControlUnitConnectionAgent(const char* ssid, const char* password, const char* server, const char* username, const char* mqtt_password) {
    mqttClient = new MQTTClientConnection(ssid, password, server, username, mqtt_password);
}

void ControlUnitConnectionAgent::setTopics(char* temperature_topic, char* connection_topic, char* periods_topic){
    char* temperature_topic = temperature_topic; 
    char* connection_topic = connection_topic;
    char* periods_topic = periods_topic;
}

void ControlUnitConnectionAgent::sendTemperature(double temperature) {

}

void ControlUnitConnectionAgent::loop() {

}

double ControlUnitConnectionAgent::getPeriod() {

}

boolean ControlUnitConnectionAgent::getConnectionStatus() {

}