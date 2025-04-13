/* Abstract class for the implementaion of an MQTT client.
This class will allow to subscribe to topics as well as to publish messages on them*/
#ifndef MQTTCLIENTCONNECTION_H
#define MQTTCLIENTCONNECTION_H

#define MAX_WAITING_TIME 10000

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
    void subscribeToTopics(const char* periods_topic, const char* connection_topic);
    //Returns true if a new sampling period has been comunicated from the controlunit
    bool newPeriodAvailable();
    //Returns the last period sent my the control unit.
    int getPeriod();
    //Returns true if the connection is certified to be still on, false otherwise.
    bool getConnectionStatus();

    void callback(const char* topic, byte* payload, unsigned int length);

    static MQTTClientConnection* instance; 
private:
    void connectWiFi();
    void reconnectMQTT();

    const char* ssid;
    const char* password;
    const char* mqtt_server;
    const char* mqtt_user;
    const char* mqtt_pass;

    
    const char* connection_topic;
    const char* periods_topic;

    long lastMsgTime;
    int period;
    bool periodAvailable = false;
    bool communicationOk = false;

    WiFiClientSecure espClient;
    PubSubClient client;
};

#endif
