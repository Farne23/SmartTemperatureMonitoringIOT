#include "MQTTClientConnection.h"

/* Configurazione WiFi e MQTT */
const char* wifi_ssid = "iPhone (4)";
const char* wifi_password = "12345678";
const char* mqtt_server = "d4d5510d5dd54ceea93cb08348cdbb8d.s1.eu.hivemq.cloud";
const char* mqtt_topic = "esiot-2024";
const char* mqtt_username = "MicheleFarneti";
const char* mqtt_password = "12345678aA";

/* Creazione dell'istanza MQTT */
MQTTClientConnection mqttClient(wifi_ssid, wifi_password, mqtt_server, mqtt_topic, mqtt_username, mqtt_password);

void setup() {
    mqttClient.begin();
}

void loop() {
    mqttClient.ensureConnected(); // Mantiene attiva la connessione MQTT

    static unsigned long lastMsgTime = 0;
    unsigned long now = millis();
    
    if (now - lastMsgTime > 10000) {
        lastMsgTime = now;

        char message[50];
        snprintf(message, sizeof(message), "Hello world #%lu", now / 1000);
        mqttClient.publishMessage(message);
    }
}