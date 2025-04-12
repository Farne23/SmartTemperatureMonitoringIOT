#include "ControlUnitConnectionAgent.h"
#include <time.h>
#include <Arduino.h>

ControlUnitConnectionAgent::ControlUnitConnectionAgent(const char* ssid, const char* password, const char* server, const char* username, const char* mqtt_password) {
    mqttClient = new MQTTClientConnection(ssid, password, server, username, mqtt_password);
    mqttClient->begin();

    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        Serial.println(&timeinfo, "Now it's %A, %B %d %Y %H:%M:%S");
    } else {
        Serial.println("Failed to obtain time");
    }
}

ControlUnitConnectionAgent::~ControlUnitConnectionAgent() {
    delete mqttClient; // 
}

void ControlUnitConnectionAgent::setTopics(char* temperature_topic,char* connection_topic,char* periods_topic) {
    this->temperature_topic = temperature_topic;
    mqttClient->subscribeToTopics(periods_topic, connection_topic);
}

void ControlUnitConnectionAgent::sendTemperature(double temperature) {
    char payload[32];
    snprintf(payload, sizeof(payload), "Temperatura: %.2f", temperature);
    mqttClient->publishMessage(temperature_topic, payload);
}

void ControlUnitConnectionAgent::loop() {
    mqttClient->ensureConnected();
}

bool ControlUnitConnectionAgent::newPeriodAvailable() {
    return mqttClient->newPeriodAvailable();
}

int ControlUnitConnectionAgent::getPeriod() {
    return mqttClient->getPeriod();
}

bool ControlUnitConnectionAgent::getConnectionStatus() {
    return mqttClient->getConnectionStatus();
}