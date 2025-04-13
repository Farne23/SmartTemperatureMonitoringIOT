#include "MQTTClientConnection.h"

MQTTClientConnection* MQTTClientConnection::instance = nullptr;

MQTTClientConnection::MQTTClientConnection(const char* ssid, const char* password, const char* server, const char* username, const char* mqtt_password)
    : ssid(ssid), password(password), mqtt_server(server), mqtt_user(username), mqtt_pass(mqtt_password), client(espClient) {}

void MQTTClientConnection::begin() {
    Serial.begin(115200);
    connectWiFi();

    // Disabilita la verifica del certificato (solo per test!)
    espClient.setInsecure(); 
    client.setServer(mqtt_server, 8883);
    MQTTClientConnection::instance = this;
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

void MQTTClientConnection::callback(const char* topic, byte* payload, unsigned int length) {
    Serial.println(String("Message arrived on [") + topic + "] len: " + length );
    if(strcmp(topic, this->connection_topic) == 0){
        communicationOk = true;
        lastMsgTime = millis();
    }else if(strcmp(topic, this->periods_topic) == 0){
        String payloadStr;
        for (unsigned int i = 0; i < length; i++) {
            payloadStr += (char)payload[i];
        }
        int value = payloadStr.toInt();
        period = value;
        periodAvailable = true;
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (MQTTClientConnection::instance != nullptr) {
        MQTTClientConnection::instance->callback(topic, payload, length);
    }
}

void MQTTClientConnection::subscribeToTopics(const char* periods_topic,const char* connection_topic){ 
    this->connection_topic = connection_topic;
    this->periods_topic = periods_topic;
    client.subscribe(this->periods_topic);
    client.subscribe(this->connection_topic);
    client.setCallback(mqttCallback);
}

bool MQTTClientConnection::newPeriodAvailable(){
    return this->periodAvailable;
}

int MQTTClientConnection::getPeriod(){
    this->periodAvailable=false;
    return period;
}

bool MQTTClientConnection::getConnectionStatus(){
    if(lastMsgTime + MAX_WAITING_TIME < millis()){
        return false;
    }else{
        return true;
    }
}