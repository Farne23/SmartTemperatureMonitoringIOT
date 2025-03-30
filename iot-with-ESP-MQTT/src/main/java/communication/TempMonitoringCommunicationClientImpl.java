package communication;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

public class TempMonitoringCommunicationClientImpl implements TempMonitoringCommunicationClient{
	
	private static final String CONFIG_FILE_PATH = "/MQTTconfig.csv";
	
	private static MQTTAgent agent; 
	
	public TempMonitoringCommunicationClientImpl(){
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
        

        agent = new MQTTAgentImpl(
        		config.get("broker_address"),
        		Integer.parseInt(config.get("port")),
        		config.get("username"),
        		config.get("password")
        		);
	}

	@Override
	public void initialize() {
		agent.start();
		agent.subscribeToTopic(
				"TOPIC NAME", 
				s -> {
		            System.out.println("New message in topic: " + s.topicName());
		            System.out.println("Content (as string): " + s.payload().toString());
		            System.out.println("QoS: " + s.qosLevel());}
				);
		
	}

	@Override
	public synchronized void sendFrequency() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public synchronized void getTemperatures() {
		// TODO Auto-generated method stub
		
	}
	
}
