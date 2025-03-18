package comm.mqtt;

import org.eclipse.paho.client.mqttv3.*;

/**
 * This class represents an istance of an MQTT connection from a client to a certain topic.
 */
public class MqttConnection {
	
	private final String broker;
    private final String clientId;
    private final String topic;
    private static MqttClient client;
    
    /**
     * Constructor method
     * @param broker the broker address
     * @param clientId id used by the client
     * @param topic the topic you want to subscribe to.
     */
    public MqttConnection(final String broker, final String clientId, final String topic) {
    	this.broker = broker;
    	this.clientId = clientId;
    	this.topic = topic;
    	try {
			this.client = new MqttClient(broker, clientId);
			// Sets up connection options
            MqttConnectOptions options = new MqttConnectOptions();
            options.setCleanSession(true);
            options.setAutomaticReconnect(true);
            // Sets the handlers
            client.setCallback(new MqttCallback() {

                // Handler for sudden disconnection
                @Override
                public void connectionLost(Throwable cause) {
                    System.out.println("Connection lost! Cause: " + cause.getMessage());
                }

                // Handler for a message reception
                @Override
                public void messageArrived(String topic, MqttMessage message) throws Exception {
                	var msg = new String(message.getPayload());
                    System.out.println("Message received on topic '" + topic + "': " + msg);
                    // Insert an handler for the message
                }

                // Handler for a message delivery
                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    System.out.println("Message uploaded: " + token.getMessageId());
                }
            });

            // Connect to the broker 
            client.connect(options);
            System.out.println("Connection succesfull!");

            client.subscribe(topic, 1);
		} catch (MqttException e) {
			System.out.println("Impossible to connect to the MQTT server");
		}
    }
}
