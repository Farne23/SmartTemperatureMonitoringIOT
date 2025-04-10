#include "ControlUnitConnectionAgent.h"
ControlUnitConnectionAgent::ControlUnitConnectionAgent(const char* ssid, const char* password, const char* server, const char* username, const char* mqtt_password) {
    mqttClient = new MQTTClientConnection(ssid, password, server, username, mqtt_password);
    mqttClient.begin()
}

void ControlUnitConnectionAgent::setTopics(char* temperature_topic, char* connection_topic, char* periods_topic){
    char* temperature_topic = temperature_topic; 
    mqttClient.setTopics(periods_topic,connection_topic);
}

void ControlUnitConnectionAgent::sendTemperature(double temperature) {
    //QUa andrà aggiunta una formattazione coerente dell'orario.
    mqttClient.publishMessage(temperature_topic,"Temperatura");
}

void ControlUnitConnectionAgent::loop() {
    mqttClient.ensureConnected();
}

double ControlUnitConnectionAgent::newPeriodAvailable() {

}

double ControlUnitConnectionAgent::getPeriod() {

}

boolean ControlUnitConnectionAgent::getConnectionStatus() {

}