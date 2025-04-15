import io.vertx.core.Vertx;
import mainUnit.ControlUnit;

public class Main {
    public static void main(String[] args) {
        log("Avvio della control unit in corso...");
        Vertx vertx = Vertx.vertx();
		ControlUnit controlUnit = new ControlUnit();
		vertx.deployVerticle(controlUnit);
    }
    
    private static void log(String message) {
    	System.out.println(message);
    }
}