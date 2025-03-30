package communication;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Handler;
import io.vertx.core.buffer.Buffer;
import io.vertx.mqtt.MqttClient;
import io.vertx.mqtt.MqttClientOptions;
import io.vertx.mqtt.messages.MqttPublishMessage;

/*
 * MQTT Agent implementation, enabling to handle the connection to an MQTT broker,
 * to publish messages on a certain topic and to read message from it, defining an handler for 
 * the messages.
 */
public class MQTTAgentImpl extends AbstractVerticle implements MQTTAgent {
    
	/*
	 * Broker configuration parameters.
	 */
    private final String brokerAddress;
    private final int brokerPort;
    private final String username;
    private final String password;
    
    MqttClient client;

    public MQTTAgentImpl() {
    	this.brokerAddress =  "d4d5510d5dd54ceea93cb08348cdbb8d.s1.eu.hivemq.cloud";
        this.brokerPort = 8883;
        this.username = "MicheleFarneti";
        this.password = "12345678aA";
    }
    
    public MQTTAgentImpl(String brokerAddress, int brokerPort, String username, String password) {
        this.brokerAddress = brokerAddress;
        this.brokerPort = brokerPort;
        this.username = username;
        this.password = password;

    }

    @Override
    public void start() {    
    	//MQTT Client configuration
        MqttClientOptions options = new MqttClientOptions()
            .setUsername(this.username)
            .setPassword(this.password)
            .setSsl(true);
            // .setTrustAll(true); 

        this.client = MqttClient.create(vertx, options);
        
        client.connect(brokerPort, brokerAddress, c -> {
            if (c.succeeded()) {
                log("Connected successfully");
            } else {
                log("Connection failed: " + c.cause().getMessage());
            }
        });
    }
    
    /*
     * Allows to subscribe, once it's set up, the client to a topic, specifying 
     * the handler of the messages.
     */
    public void subscribeToTopic(String topic, Handler<MqttPublishMessage> handler) {
    	log("Subscribing to topic...");
    	/*
        client.publishHandler(s -> {
            System.out.println("New message in topic: " + s.topicName());
            System.out.println("Content (as string): " + s.payload().toString());
            System.out.println("QoS: " + s.qosLevel());
        }).subscribe(TOPIC_NAME, 2);
        */
        client.publishHandler(handler).subscribe(topic, 2);
    }
    
    /*
     * Allows to send a string as a message to a topic the client is
     * subscribed to.
     */
    public void publishMessage(String topic, String message) {
    	log("Publishing a message");
        client.publish(topic,
            Buffer.buffer(message),
            MqttQoS.AT_LEAST_ONCE,
            false,
            false);
    }

    private void log(String msg) {
        System.out.println("[MQTT AGENT] " + msg);
    }
}