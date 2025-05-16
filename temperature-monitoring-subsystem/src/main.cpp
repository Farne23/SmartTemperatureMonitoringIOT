#include "comunication/ControlUnitConnectionAgent.h"
#include "comunication/ControlUnitInterface.h"
#include "settings/HwInterfaces.h"
#include "devices/sensors/TempSensor.h"
#include "devices/lights/LightSignals.h"

/**
 *Entry point for the Temperature Monitoring Subsystem.
 *
 * This file initializes the Wi-Fi connection and sets up MQTT communication
 * to publish temperature readings and receive configuration commands.
 *
 * @authors
 * - Michele Farneti : michele.farneti@studio.unibo.it
 * - Samuele Casadei : samuele.casadei3@studio.unibo.it
 *
 * ESIOT-2024
 */

/* Wifi and MQTT Configuration, here it will be needed to set the credentials of the Wifi network the ESP32
is going to be connected to as well as the addess of the HiveMQ Server that's going to be used in order to
publish the messages. It's also needed to specify an username and a password of the HiveMQ Client */

const char *wifi_ssid = "iPhone (4)";
const char *wifi_password = "12345678";
const char *mqtt_server = "d4d5510d5dd54ceea93cb08348cdbb8d.s1.eu.hivemq.cloud";
const char *mqtt_username = "temperature-monitoring-subsystem";
const char *mqtt_password = "12345678aA";
const int mqtt_port = 8883;

/*Topic in wich the temperature monitroing subsytem is going
 to publish the temperature values sensed*/
const char *temperatures_topic = "temperatures-topic-esiot-2024";
/*Topic where the temperature monitroing subsytem is going
to receive the frequency to wich the temperature will have to be measured*/
const char *periods_topic = "periods-topic-esiot-2024";
/*Topic where the temperature monitroing subsytem is going to
periodically receive messages in oreder to check the connection status*/
const char *connection_topic = "connection-topic-esiot-2024";

/*Devices*/
Sensor *tempSensor;
Sensor *sonar;
LightSignals *lightSignals;

double temperature;
int period = 1000; //Default sampling period is 1 seconds

SemaphoreHandle_t mqttMutex;
TaskHandle_t TaskTemperature;
TaskHandle_t TaskCheckConnection;

/* Creation of an MQTT client instance */
ControlUnitInterface *mqttClient;

void log(const char *message)
{
    Serial.println(message);
}

/**
 * Task responsible for reading the temperature sensor,
 * ensuring the MQTT connection is active, sending the current
 * temperature to the broker, and updating the measurement period.
 *
 * This task runs indefinitely, acquiring the MQTT mutex before
 * accessing the MQTT client to ensure thread-safe operations.
 */
void TaskTemperaturecode(void *parameter)
{
    for (;;)
    {
        temperature = tempSensor->sense();
        if (xSemaphoreTake(mqttMutex, portMAX_DELAY))
        {
            mqttClient->ensureConnected();
            if (mqttClient->getConnectionStatus())
            {
                mqttClient->sendTemperature(temperature);
                period = mqttClient->getPeriod();
            }else{
                log("Control unit is not responding, sampling paused...");
            }
            xSemaphoreGive(mqttMutex);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * Task responsible for periodically checking the MQTT
 * connection status and signaling system health using light signals.
 *
 * This task runs indefinitely, acquiring the MQTT mutex to safely
 * check the connection state, and then activating visual indicators
 * (LEDs) to reflect whether the system is working properly or facing
 * connectivity problems.
 */
void TaskCheckConnectionCode(void *parameter)
{
    for (;;)
    {
        if (xSemaphoreTake(mqttMutex, portMAX_DELAY))
        {
            if (!mqttClient->getConnectionStatus())
            {
                lightSignals->signalProblems();
            }
            else
            {
                lightSignals->signalWorking();
            }
            xSemaphoreGive(mqttMutex);
        }
        vTaskDelay((period / 2) / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(115200);
    log("Temperature monitoring subsystem setup started...");

    // Initiliazation of the HW devices
    tempSensor = new TempSensor(TEMP_SENSOR_PIN);
    lightSignals = new LightSignals(new Led(GREEN_LED_PIN), new Led(RED_LED_PIN));
    lightSignals->signalProblems();

    // Creation of the mutex protecting the MQTT client.
    mqttMutex = xSemaphoreCreateMutex();

    // Creation of the MQTT client interface and topics setup.
    mqttClient = new ControlUnitConnectionAgent(
        wifi_ssid,
        wifi_password,
        mqtt_server,
        mqtt_username,
        mqtt_password,
        mqtt_port);
    mqttClient->setTopics(temperatures_topic, connection_topic, periods_topic);
    xTaskCreatePinnedToCore(TaskTemperaturecode, "TaskTemperature", 10000, NULL, 1, &TaskTemperature, 0);
    xTaskCreatePinnedToCore(TaskCheckConnectionCode, "TaskCheckConnection", 10000, NULL, 1, &TaskCheckConnection, 0);
    log("Setup completed!");
}

void loop()
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
