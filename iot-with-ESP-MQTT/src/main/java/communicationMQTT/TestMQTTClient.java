package communicationMQTT;

import communicationHTTP.HTTPServer;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import mainUnit.ControlUnit;

public class TestMQTTClient extends AbstractVerticle {
	
	public static void main(String[] args) throws Exception {
		
		Vertx vertx = Vertx.vertx();
		//ControlUnit controlUnit = new ControlUnit();
		//TempMonitoringCommunicationClientImpl tempClient = new TempMonitoringCommunicationClientImpl();
		//vertx.deployVerticle(tempClient);
		HTTPServer server = new HTTPServer();
		vertx.deployVerticle(server);
	}
		
}
