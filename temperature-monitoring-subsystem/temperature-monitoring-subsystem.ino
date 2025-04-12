#include "MQTTClientConnection.h"

/* Wifi and MQTT Configuration, here it will be needed to set the credentials of the Wifi network the ESP32 
is going to be connected to as well as the addess of the HiveMQ Server that's going to be used in order to
publish the messages. It's also needed to specify an username and a password of the HiveMQ Client */

const char* wifi_ssid = "iPhone (4)";
const char* wifi_password = "12345678";
const char* mqtt_server = "d4d5510d5dd54ceea93cb08348cdbb8d.s1.eu.hivemq.cloud";
const char* mqtt_username = "MicheleFarneti";
const char* mqtt_password = "12345678aA";

const char* topic = "esiot-2024";

/* Creation of an MQTT client instance */
MQTTClientConnection mqttClient(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password);

void setup() {
    mqttClient.begin();
}

void loop() {
    //Keeps alive MQTT Connection
    mqttClient.ensureConnected(); 

    static unsigned long lastMsgTime = 0;
    unsigned long now = millis();
    
    if (now - lastMsgTime > 10000) {
        lastMsgTime = now;

        char message[50];
        snprintf(message, sizeof(message), "Hello world #%lu", now / 1000);
        mqttClient.publishMessage(topic, message);
    }
}