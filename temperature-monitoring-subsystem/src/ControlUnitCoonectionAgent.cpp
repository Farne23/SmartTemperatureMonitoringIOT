#include "ControlUnitConnectionAgent.h"
#include <time.h>
#include <Arduino.h>

ControlUnitConnectionAgent::ControlUnitConnectionAgent(const char *ssid, const char *password, const char *server, const char *username, const char *mqtt_password)
{
    mqttClient = new MQTTClientConnection(ssid, password, server, username, mqtt_password);
    mqttClient->begin();

    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");

    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        Serial.println(&timeinfo, "Now it's %A, %B %d %Y %H:%M:%S");
    }
    else
    {
        Serial.println("Failed to obtain time");
    }
}

ControlUnitConnectionAgent::~ControlUnitConnectionAgent()
{
    delete mqttClient; //
}

void ControlUnitConnectionAgent::setTopics(const char *temperature_topic, const char *connection_topic, const char *periods_topic)
{
    this->temperature_topic = temperature_topic;
    mqttClient->subscribeToTopics(periods_topic, connection_topic);
}

void ControlUnitConnectionAgent::sendTemperature(double temperature)
{
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        char payload[128]; 

        snprintf(payload, sizeof(payload),
                 "{\"time\":\"%04d-%02d-%02d %02d:%02d:%02d\", \"temperature\": %.2f}",
                 timeinfo.tm_year + 1900,
                 timeinfo.tm_mon + 1,
                 timeinfo.tm_mday,
                 timeinfo.tm_hour,
                 timeinfo.tm_min,
                 timeinfo.tm_sec,
                 temperature);

        mqttClient->publishMessage(temperature_topic, payload);
        Serial.println(payload); 
    }
    else
    {
        Serial.println("Failed to obtain time");
    }
}

void ControlUnitConnectionAgent::ensureConnected()
{
    mqttClient->ensureConnected();
}

bool ControlUnitConnectionAgent::newPeriodAvailable()
{
    return mqttClient->newPeriodAvailable();
}

int ControlUnitConnectionAgent::getPeriod()
{
    return mqttClient->getPeriod();
}

bool ControlUnitConnectionAgent::getConnectionStatus()
{
    return mqttClient->getConnectionStatus();
}