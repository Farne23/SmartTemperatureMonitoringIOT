#include "ControlUnitConnectionAgent.h"
ControlUnitConnectionAgent::ControlUnitConnectionAgent(const char* ssid, const char* password, const char* server, const char* username, const char* mqtt_password) {
    mqttClient = new MQTTClientConnection(ssid, password, server, username, mqtt_password);
    mqttClient.begin()
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
    Serial.println(&timeinfo, "Now it's %A, %B %d %Y %H:%M:%S");
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

bool ControlUnitConnectionAgent::newPeriodAvailable() {
    return mqttClient.newPeriodAvailable();
}

int ControlUnitConnectionAgent::getPeriod() {
    return mqttClient.getPeriod();
}

bool ControlUnitConnectionAgent::getConnectionStatus() {
    return mqttClient.getConnectionStatus();
}