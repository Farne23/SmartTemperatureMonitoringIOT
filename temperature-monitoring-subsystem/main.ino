#include "MQTTClientHandler.h"

/* Configurazione WiFi e MQTT */
const char* ssid = "LittleBarfly";
const char* password = "esiot-2024-2025";
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* topic = "esiot-2024";

/* Creazione dell'istanza MQTT */
MQTTClientHandler mqttClient(ssid, password, mqtt_server, topic);

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