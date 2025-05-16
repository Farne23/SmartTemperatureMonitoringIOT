package communicationMQTT;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.buffer.Buffer;
import io.vertx.mqtt.MqttClient;
import io.vertx.mqtt.MqttClientOptions;

public class TempMonitoringCommunicationClient extends AbstractVerticle{
	
	private static final String CONFIG_FILE_PATH = "/MQTTconfig.csv";
	private static final int CONNECTION_KEEP_UP_TIME = 5000; //The period of the periodic meessage for communicating that connection is still on
	private static String FREQUENCY_BUS_ADDRESS = "frequency.line";
	
	private static final int MAX_RETRIES = 5;
	private static final long RETRY_DELAY_MS = 5000;
	private int retryCount = 0;
	
	private final String brokerAddress;
    private final int brokerPort;
    private final String username;
    private final String password;
	private String temperatureTopic; 
	private String periodsTopic;
	private String connectionTopic;
	private MqttClient client;
	
	public TempMonitoringCommunicationClient() {
		Map<String, String> config = new HashMap<>();

        try(
        	InputStream inputStream = TempMonitoringCommunicationClient.class.getResourceAsStream(CONFIG_FILE_PATH);
            BufferedReader br = new BufferedReader(new InputStreamReader(inputStream))) {
	            String header = br.readLine(); 
	            String line = br.readLine();   
	
	            if (line != null) {
	                String[] values = line.split(",");
	                String[] keys = header.split(",");
	
	                for (int i = 0; i < keys.length; i++) {
	                    config.put(keys[i].trim(), values[i].trim());
	                }
	            }
        } catch (Exception e) {
            System.err.println("Errore nella lettura del file di configurazione: " + e.getMessage());
        }
        
        this.brokerAddress=config.get("broker_address");
        this.brokerPort=Integer.parseInt(config.get("port"));
        this.username=config.get("username");
        this.password=config.get("password");
        this.temperatureTopic = config.get("topic_temperature");
        this.periodsTopic = config.get("topic_period");
        this.connectionTopic = config.get("topic_connection");
	}
	
	public void start(){
		//MQTT Client configuration
        MqttClientOptions options = new MqttClientOptions()
            .setUsername(this.username)
            .setPassword(this.password)
            .setSsl(true)
            .setTrustAll(true);  //Standard HiveMQV version doesn't allow certificates
        
        log("Connesso a " + this.brokerAddress);
        //log(Integer.toString( this.brokerPort));
        //log(this.username);
        //log(this.password);

        this.client = MqttClient.create(vertx, options);
        attemptConnection();
        
      
	}
	
	/*
	 * Function used to attempt connection to the MQTT broker, 
	 * gets called repeatedly in case the connection keeps failing
	 */
	private void attemptConnection() {
		  log("Trying to connect to " + this.brokerAddress + ":" + this.brokerPort);

		    client.connect(brokerPort, brokerAddress, c -> {
		        if (c.succeeded()) {
		            log("Connected successfully to MQTT broker.");
		            retryCount = 0;

		            client.publishHandler(s -> {
		                vertx.eventBus().request("temperatures.line", s.payload(), reply -> {
		                    if (!reply.succeeded()) {
		                        System.err.println("SenderVerticle failed to receive reply.");
		                    }
		                });
		            }).subscribe(temperatureTopic, 2);

		            vertx.eventBus().consumer(FREQUENCY_BUS_ADDRESS, message -> {
		                setFrequency(Integer.parseInt(message.body().toString()));
		                message.reply("Success");
		            });

		            vertx.setPeriodic(CONNECTION_KEEP_UP_TIME, id -> {
		                signalConnectionWorking();
		            });
		        } else {
		            log("Connection failed: " + c.cause().getMessage());

		            if (retryCount < MAX_RETRIES) {
		                retryCount++;
		                long delay = RETRY_DELAY_MS * retryCount;
		                log("Retrying in " + delay + "ms (attempt " + retryCount + "/" + MAX_RETRIES + ")...");

		                vertx.setTimer(delay, id -> attemptConnection());
		            } else {
		                log("Max retry attempts reached. Giving up.. ");
		            }
		        }
		    });
		
	}


	/*
	 * Publishes a message in the periods'topic,
	 * containing the new sampling period the esp32 environment has to 
	 * work with.
	 * 
	 * @param period New sampling period to be sent.
	 */
	private void setFrequency(int period) {
		log("Pubblico messaggio");
		publishMessage(periodsTopic, Integer.toString(period),MqttQoS.EXACTLY_ONCE);
	}
	
	/*
	 * Publishes a message in the conection topic signalingthat connewction
	 * is still on
	 */
	private void signalConnectionWorking() {
		publishMessage(connectionTopic, "Ok",MqttQoS.EXACTLY_ONCE);
	}

	/*
	 * Publishes a message, as a string, on a given topic,
	 * and with a given quality of service.
	 * 
	 * @param topic
	 * @param message
	 * @param QoS
	 */
	private void publishMessage(String topic, String message, MqttQoS QoS) {
    	//log("Publishing a message: " + message);
        client.publish(topic,
            Buffer.buffer(message),
            QoS,
            false,
            false);
    }
	
	/*
	 * Simple log function used for debug
	 * 
	 * @param msg The message to be printed
	 */
	private void log(String msg) {
        System.out.println("[MQTT AGENT] " + msg);
    }
	
}
