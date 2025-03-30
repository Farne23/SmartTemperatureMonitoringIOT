package communication;

import io.vertx.core.Handler;
import io.vertx.mqtt.messages.MqttPublishMessage;

public interface MQTTAgent {
	
	/*
	 * Starts the connection to a given broker
	 */
	void start();
	
	/*
	 * Publishes a message to a given topic
	 */
	void publishMessage(String topic, String message);
	
	/*
	 * Subscribes the agent to a given topic, specifying the handler
	 * for the messages being published in it
	 */
	void subscribeToTopic(String topic, Handler<MqttPublishMessage> handler);
}
