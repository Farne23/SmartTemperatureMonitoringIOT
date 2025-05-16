import io.vertx.core.Vertx;
import communicationMQTT.TempMonitoringCommunicationClient;
import communicationHTTP.HTTPServer;
import mainUnit.ControlUnit;

public class Main {
    public static void main(String[] args) {
    	
    	Vertx vertx = Vertx.vertx();
    	
    	//Control Unit Core Initialization
        log("## Starting control unit core ...");
		ControlUnit controlUnit = new ControlUnit();
		vertx.deployVerticle(controlUnit);
		
		//MQTT communication module initialization
		log("## Starting MQTT comunication module ...");
		TempMonitoringCommunicationClient MQTTModule = new TempMonitoringCommunicationClient();
		vertx.deployVerticle(MQTTModule);
		
		//HTTP communication module initialization
		log("## Starting HTTP comunication module ...");
		HTTPServer HTTPModule = new HTTPServer();
		vertx.deployVerticle(HTTPModule);
    }
    
    private static void log(String message) {
    	System.out.println(message);
    }
}