#ifndef MQTTCLIENTCONNECTION_H
#define MQTTCLIENTCONNECTION_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class MQTTClientConnection {
public:
    MQTTClientConnection(const char* ssid, const char* password, const char* server, const char* topic, const char* username, const char* mqtt_password);
    void begin();
    void ensureConnected();
    void publishMessage(const char* message);

private:
    void connectWiFi();
    void reconnectMQTT();

    const char* ssid;
    const char* password;
    const char* mqtt_server;
    const char* topic;
    const char* mqtt_user;
    const char* mqtt_pass;

    WiFiClientSecure espClient;
    PubSubClient client;
};

#endif
