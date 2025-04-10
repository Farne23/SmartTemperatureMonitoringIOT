#include "MQTTClientConnection.h"

MQTTClientConnection::MQTTClientConnection(const char* ssid, const char* password, const char* server, const char* username, const char* mqtt_password)
    : ssid(ssid), password(password), mqtt_server(server), mqtt_user(username), mqtt_pass(mqtt_password), client(espClient) {}

void MQTTClientConnection::begin() {
    Serial.begin(115200);
    connectWiFi();

    // Disabilita la verifica del certificato (solo per test!)
    espClient.setInsecure(); 
    client.setServer(mqtt_server, 8883);
}

void MQTTClientConnection::connectWiFi() {
    Serial.println(String("Connecting to ") + ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected. IP address: " + WiFi.localIP().toString());
}

void MQTTClientConnection::ensureConnected() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();
}

void MQTTClientConnection::reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "esiot-client-" + String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
            Serial.println("connected");
            //client.subscribe(topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void MQTTClientConnection::publishMessage(const char* topic, const char* message) {
    Serial.println(String("Publishing: ") + message);
    client.publish(topic, message);
}


void MQTTClientConnection::callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(String("Message arrived on [") + topic + "] len: " + length );
}

void MQTTClientConnection::subscripeToTopics(char* topic_periods, char* topic_connection){
    client.subscribe(topic_periods);
    client.subscribe(topic_connection);
}