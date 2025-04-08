#include "MQTTClientConnection.h"

/* Wifi and MQTT Configuration, here it will be needed to set the credentials of the Wifi network the ESP32 
is going to be connected to as well as the addess of the HiveMQ Server that's going to be used in order to
publish the messages. It's also needed to specify an username and a password of the HiveMQ Client */

const char* wifi_ssid = "iPhone (4)";
const char* wifi_password = "12345678";
const char* mqtt_server = "d4d5510d5dd54ceea93cb08348cdbb8d.s1.eu.hivemq.cloud";
const char* mqtt_username = "MicheleFarneti";
const char* mqtt_password = "12345678aA";

/*Topic in wich the temperature monitroing subsytem is going
 to publish the temperature values sensed*/
const char* topic_temperatures = "esiot-2024";
/*Topic where the temperature monitroing subsytem is going 
to receive the frequency to wich the temperature will have to be measuerd*/
const char* topic_periods = "esiot-2024";
/*Topic where the temperature monitroing subsytem is going to 
periodically receive messages in oreder to check the connection status*/
const char* topic_connection = "esiot-2024";

unsigned long lastMsgTime = 0;

TaskHandle_t Task1;

/* Creation of an MQTT client instance */
MQTTClientConnection* mqttClient;

void setup() {
    Serial.begin(115200);
    mqttClient = new MQTTClientConnection(
        wifi_ssid, 
        wifi_password, 
        mqtt_server, 
        mqtt_username, 
        mqtt_password);
    mqttClient.begin();
    xTaskCreatePinnedToCore(Task1code,"Task1",10000,NULL,1,&Task1,0);  
}

void Task1code(void* parameter){
  for(;;){
    //Keeps alive MQTT Connection
    mqttClient->ensureConnected(); 
    unsigned long now = millis();
    
    if (now - lastMsgTime > 10000) {
        lastMsgTime = now;
        char message[50];
        snprintf(message, sizeof(message), "Hello world #%lu", now / 1000);
        mqttClient->publishMessage(topic_temperatures, message);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);  // 100 ms delay
  } 
}