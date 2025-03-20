package communication;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.buffer.Buffer;
import io.vertx.mqtt.MqttClient;
import io.vertx.mqtt.MqttClientOptions;

/*
 * MQTT Agent with Authentication
 */
public class MQTTAgent extends AbstractVerticle {
    
    private static final String BROKER_ADDRESS = "broker.mqtt-dashboard.com";
    private static final int BROKER_PORT = 1883;
    private static final String TOPIC_NAME = "esiot-2024";
    private static final String USERNAME = "your-username";
    private static final String PASSWORD = "your-password";

    public MQTTAgent() {}

    @Override
    public void start() {    
        // Configure MQTT client with authentication
        MqttClientOptions options = new MqttClientOptions()
            .setUsername(USERNAME)
            .setPassword(PASSWORD);
        
        MqttClient client = MqttClient.create(vertx, options);
        
        client.connect(BROKER_PORT, BROKER_ADDRESS, c -> {
            if (c.succeeded()) {
                log("Connected successfully");
                
                log("Subscribing to topic...");
                client.publishHandler(s -> {
                    System.out.println("New message in topic: " + s.topicName());
                    System.out.println("Content (as string): " + s.payload().toString());
                    System.out.println("QoS: " + s.qosLevel());
                }).subscribe(TOPIC_NAME, 2);

                log("Publishing a message");
                client.publish(TOPIC_NAME,
                    Buffer.buffer("hello"),
                    MqttQoS.AT_LEAST_ONCE,
                    false,
                    false);
            } else {
                log("Connection failed: " + c.cause().getMessage());
            }
        });
    }

    private void log(String msg) {
        System.out.println("[MQTT AGENT] " + msg);
    }
}