import io.vertx.core.Vertx;
import communicationMQTT.TempMonitoringCommunicationClient;
import communicationSerial.WindowControlCommunicationClient;
import communicationHTTP.HTTPServer;
import mainUnit.ControlUnit;

/**
 * Main.java
 * 
 * Main entry point for the system.
 * Initializes and deploys:
 *  - Control Unit Core
 *  - MQTT Communication Module
 *  - HTTP Communication Module
 *  -
 * using the Vert.x framework.
 * 
 * Authors: Michele Farneti, Samuele Casadei
 * Mails: michele.farneti@studio.unibo.it, samuele.casadei3@studio.unibo.it
 */

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
		
		//HTTP communication module initialization
		log("## Starting Serial communication module ...");
		WindowControlCommunicationClient serialClient = new WindowControlCommunicationClient();
		vertx.deployVerticle(serialClient);
		
    }
    
    private static void log(String message) {
    	System.out.println(message);
    }
}