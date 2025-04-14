#include "ControlUnitConnectionAgent.h"
#include "ControlUnitInterface.h"
#include "settings/HwInterfaces.h"
#include "devices/sensors/TempSensor.h"
#include "devices/lights/LightSignals.h"

/* Wifi and MQTT Configuration, here it will be needed to set the credentials of the Wifi network the ESP32 
is going to be connected to as well as the addess of the HiveMQ Server that's going to be used in order to
publish the messages. It's also needed to specify an username and a password of the HiveMQ Client */

const char* wifi_ssid = "iPhone (4)";
const char* wifi_password = "12345678";
const char* mqtt_server = "d4d5510d5dd54ceea93cb08348cdbb8d.s1.eu.hivemq.cloud";
const char* mqtt_username = "MicheleFarneti";
const char* mqtt_password = "12345678aA";
const int mqtt_port = 8883;

/*Topic in wich the temperature monitroing subsytem is going
 to publish the temperature values sensed*/
const char* temperatures_topic = "esiot-2024";
/*Topic where the temperature monitroing subsytem is going 
to receive the frequency to wich the temperature will have to be measured*/
const char* periods_topic = "esiot-2024";
/*Topic where the temperature monitroing subsytem is going to 
periodically receive messages in oreder to check the connection status*/
const char* connection_topic = "esiot-2024";

/*Devices*/
Sensor* tempSensor;
Sensor* sonar;
LightSignals* lightSignals;

unsigned long lastMsgTime = 0;
double temperature;
int period;

SemaphoreHandle_t mqttMutex;
TaskHandle_t TaskTemperature;
TaskHandle_t Task2;

/* Creation of an MQTT client instance */
ControlUnitInterface* mqttClient;

void log(const char* message){
    Serial.println(message);
}

void TaskTemperaturecode(void* parameter){
    for(;;){
        temperature = tempSensor->sense();
        if (xSemaphoreTake(mqttMutex, portMAX_DELAY)) {
            mqttClient->ensureConnected();
            if(mqttClient->getConnectionStatus()){
                mqttClient->sendTemperature(temperature); 
                period = mqttClient->getPeriod();
            }
            xSemaphoreGive(mqttMutex);            
        }
        vTaskDelay(period / portTICK_PERIOD_MS); 
    } 
}

void TaskCheckConnectionCode(void *parameter){
    for(;;){
        if (xSemaphoreTake(mqttMutex, portMAX_DELAY)) {
            if(!mqttClient->getConnectionStatus()){
                lightSignals->signalProblems();
            }else{
                lightSignals->signalWorking();
            }
            xSemaphoreGive(mqttMutex);     
        }
        vTaskDelay((period/2) / portTICK_PERIOD_MS); 
    }
}

void setup() {
    Serial.begin(115200);
    log("Temperature monitoring subsystem setup started...");

    //Initiliazation of the HW devices
    tempSensor = new TempSensor(TEMP_SENSOR_PIN);
    lightSignals = new LightSignals(new Led(GREEN_LED_PIN), new Led(RED_LED_PIN));
    lightSignals->signalWorking();

    //Creation of the mutex protecting the MQTT client.
    mqttMutex = xSemaphoreCreateMutex();

    //Creation of the MQTT client interface and topics setup.
    mqttClient = new ControlUnitConnectionAgent(
        wifi_ssid, 
        wifi_password, 
        mqtt_server, 
        mqtt_username, 
        mqtt_password,
        mqtt_port
    );
    mqttClient->setTopics(temperatures_topic,connection_topic,periods_topic);
    xTaskCreatePinnedToCore(TaskTemperaturecode,"Task1",10000,NULL,1,&TaskTemperature,0); 

    //xTaskCreatePinnedToCore(TaskCheckConnectionCode,"Task2",10000,NULL,1,&Task2,0); 
    log("Setup completed!");
}

void loop() {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

