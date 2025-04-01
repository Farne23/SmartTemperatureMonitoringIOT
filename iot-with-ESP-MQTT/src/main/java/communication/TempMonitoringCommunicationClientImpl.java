package communication;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.json.JSONException;
import org.json.JSONObject;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.buffer.Buffer;
import io.vertx.mqtt.MqttClient;
import io.vertx.mqtt.MqttClientOptions;
import mainUnit.TempSensorDataReceiver;

public class TempMonitoringCommunicationClientImpl extends AbstractVerticle implements TempMonitoringCommunicationClient{
	
	private static final String CONFIG_FILE_PATH = "/MQTTconfig.csv";
    private final String brokerAddress;
    private final int brokerPort;
    private final String username;
    private final String password;
	private String topicTemperature; 
	private String topicPeriod;
	private MqttClient client;
	
	public TempMonitoringCommunicationClientImpl() {
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
        this.topicTemperature = config.get("topic_temperature");
        this.topicPeriod = config.get("topic-period");
	}
	
	public void start(){
		//MQTT Client configuration
        MqttClientOptions options = new MqttClientOptions()
            .setUsername(this.username)
            .setPassword(this.password)
            .setSsl(true);
            // .setTrustAll(true); 

        System.out.println(vertx);
        this.client = MqttClient.create(vertx, options);
        
        client.connect(brokerPort, brokerAddress, c -> {
            if (c.succeeded()) {
                log("Connected successfully");
                client.publishHandler(
                		s -> {				
  						  System.out.println("New message in topic: " + s.topicName());
  				          System.out.println("Content (as string): " + s.payload().toString());
  				          System.out.println("QoS: " + s.qosLevel());
  				          
  				          JSONObject json = new JSONObject(s.payload());
  				          SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
  							try {
  								Date timestamp = sdf.parse(json.getString("timestamp"));
  								double temperature = json.getDouble("temperature");
  							} catch (ParseException | JSONException e) {
  								System.out.println("JSON non riconoscibile");
  								e.printStackTrace();
      							}		                
      				}).subscribe(topicTemperature, 2);
            } else {
                log("Connection failed: " + c.cause().getMessage());
            }
        });        
	}


	@Override
	public void setFrequency(int period) {
		//agent.publishMessage(topicPeriod, Integer.toString(period));
	}

	@Override
	public synchronized void getTemperatures() {
		// TODO Auto-generated method stub
		
	}
	
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
