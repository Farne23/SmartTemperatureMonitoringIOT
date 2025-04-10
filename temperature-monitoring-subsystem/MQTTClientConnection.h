/* Abstract class for the implementaion of an MQTT client.
This class will allow to subscribe to topics as well as to publish messages on them*/
#ifndef MQTTCLIENTCONNECTION_H
#define MQTTCLIENTCONNECTION_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class MQTTClientConnection {
public:
    //Creates an instance able to connect to an MQTT Server through given credentials after connectiong to a wifi network.
    MQTTClientConnection(const char* ssid, const char* password, const char* server, const char* username, const char* mqtt_password);
    //Joins the given wifi networks and establishes a connection with the MQTT Server.
    void begin();
    //Checks the status of the MQTT Connection, evntually rejoining the server.
    void ensureConnected();
    //Publishes a given message on a given topic
    void publishMessage( const char* topic, const char* message);
    //Enables the MQTT client to handle it'asynchronous behaviour.
    void loop();
    //Subrscribes the client to the topics from wich it's going to receive infos via MQTT
    void subscripeToTopics(const char* topic_periods, const char* topic_connection);

private:
    void connectWiFi();
    void reconnectMQTT();
    void callback();

    const char* ssid;
    const char* password;
    const char* mqtt_server;
    const char* mqtt_user;
    const char* mqtt_pass;

    
    char* connection_topic;
    char* periods_topic;

    WiFiClientSecure espClient;
    PubSubClient client;
};

#endif
